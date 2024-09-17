#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int set_of_states;
  int *accept_states;        // array of accepting states
  int **transition_function; // 2D array of transitions
  int initial_state;
  int set_accepting_states;
} DFA;

DFA *CreateDFA(int set_of_states, int *accept_states, int set_accepting_states,
               int initial_state) {
  DFA *dfa = (DFA *)malloc(sizeof(DFA));
  dfa->set_of_states = set_of_states;
  // dfa->accept_states = accept_states;
  dfa->set_accepting_states = set_accepting_states;
  dfa->initial_state = initial_state;

  dfa->accept_states = (int *)malloc(set_accepting_states * sizeof(int));
  for (int i = 0; i < set_accepting_states; i++) {
    dfa->accept_states[i] = accept_states[i];
  }

  dfa->transition_function = (int **)malloc(set_of_states * sizeof(int *));
  for (int i = 0; i < set_of_states;
       i++) // Allocate memory for each state in the transition function
  {
    dfa->transition_function[i] =
        (int *)malloc(128 * sizeof(int)); // ASCII(0 to 128)
    for (int j = 0; j < 128; j++)
      dfa->transition_function[i][j] = -1; //-1 means invalid
  }
  return dfa;
}

void setTransition(DFA *dfa, char input, int state, int nextState) {
  dfa->transition_function[state][(int)input] = nextState;
  // The transition function determines the next state given the current state
  // and input
}

bool checkState(DFA *dfa, int state) {
  for (int i = 0; i < dfa->set_accepting_states; i++) {
    if (dfa->accept_states[i] == state)
      return true;
    // printf("%d\n", state);
  }
  return false;
}

bool DFA_run(DFA *dfa, char *input) {
  int current_state = dfa->initial_state;
  for (int i = 0; i < strlen(input); i++) {
    char symbol = input[i]; // Get the current symbol from the input
    current_state = dfa->transition_function[current_state][(int)symbol];
    // Get the next state from the transition function
    if (current_state == -1)
      return false;
  }
  return checkState(dfa, current_state);
}

void DFA_repl(DFA *dfa) {
  char input[256]; // Define a char array to store the input (which is a string)
  while (true) {
    printf("Enter an input (or 'quit' to exit):\n");
    fgets(input, sizeof(input), stdin);
    // Get input from the user (char *str, int n, FILE *stream)
    input[strcspn(input, "\n")] = '\0';
    // delete "enter"
    if (strcmp(input, "quit") == 0)
      break;
    // "strcmp" returns 0 if the string is equal to "quit"
    if (DFA_run(dfa, input))
      printf("true\n"); // Check if the input is accepted by the DFA
    else
      printf("false\n");
  }
}

DFA *DFA_xyzzy(void) {
  int accept_states[] = {5};
  DFA *dfa = CreateDFA(6, accept_states, 1, 0);
  setTransition(dfa, 'x', 0, 1);
  setTransition(dfa, 'y', 1, 2);
  setTransition(dfa, 'z', 2, 3);
  setTransition(dfa, 'z', 3, 4);
  setTransition(dfa, 'y', 4, 5);
  return dfa;
}

DFA *DFA_987(void) {
  int accept_states[] = {3}; // State 3 is the accepting state
  DFA *dfa = CreateDFA(4, accept_states, 1, 0);

  // State 0
  setTransition(dfa, '9', 0, 1); // Move to state 1 on '9'
  for (int i = 0; i < 128; i++)
    if (i != '9')
      setTransition(dfa, (char)i, 0,
                    0); // Stay in state 0 on any input except '9'

  // State 1
  setTransition(dfa, '8', 1, 2); // Move to state 2 on '8'
  setTransition(dfa, '9', 1, 1); // Stay in state 1 on '9'
  for (int i = 0; i < 128; i++)
    if (i != '8' && i != '9')
      setTransition(dfa, (char)i, 1,
                    1); // Stay in state 1 on any input except '8'

  // State 2
  setTransition(dfa, '7', 2, 3); // Move to state 3 (accepting) on '7'
  setTransition(dfa, '9', 2, 1); // Move back to state 1 on '9'
  setTransition(dfa, '8', 2, 2); // Stay in state 2 on '8'
  for (int i = 0; i < 128; i++)
    if (i != '7' && i != '9' && i != '8')
      setTransition(dfa, (char)i, 2,
                    2); // Stay in state 2 on any input except '7', '9', '8'

  // State 3
  for (int i = 0; i < 128; i++)
    setTransition(dfa, (char)i, 3, 3); // Stay in state 3 on any input
  return dfa;
}

DFA *DFA_4(void) {
  int accept_states[] = {2, 3};                 // Accept state 2 and 3
  DFA *dfa = CreateDFA(5, accept_states, 2, 0); // 5 states in total

  setTransition(dfa, '4', 0, 1); // Transitions for state 0 (no '4' yet)
  for (int i = 0; i < 128; i++)
    if (i != '4')
      setTransition(dfa, (char)i, 0, 0);

  setTransition(dfa, '4', 1, 2); // second '4'
  for (int i = 0; i < 128; i++)
    if (i != '4')
      setTransition(dfa, (char)i, 1, 1);

  setTransition(dfa, '4', 2, 3); // third '4'
  for (int i = 0; i < 128; i++)
    if (i != '4')
      setTransition(dfa, (char)i, 2, 2);

  // three '4's
  setTransition(dfa, '4', 3, 4);
  for (int i = 0; i < 128; i++)
    if (i != '4')
      setTransition(dfa, (char)i, 3, 3);

  // more than three '4's
  for (int i = 0; i < 128; i++)
    setTransition(dfa, (char)i, 4, 4); // Stay in state 4+not accepting
  return dfa;
}

typedef struct {
  int num_states;
  bool transition_function
      [100][128][100];        // transition_function[state][char][target_state]
  bool accepting_states[100]; // Array to track which states are accepting
} NFA;

// Function to create a new empty NFA with a given number of states
NFA *NFA_create(int num_states) {
  NFA *nfa = malloc(sizeof(NFA));

  // Check if memory allocation was successful
  if (nfa == NULL) {
    return NULL;
  }

  nfa->num_states = num_states;

  // Initialize the transition function to false
  for (int i = 0; i < num_states; i++) {
    for (int j = 0; j < 128; j++) {
      for (int k = 0; k < num_states; k++) {
        nfa->transition_function[i][j][k] = false;
      }
    }
  }

  // Initialize the accepting states to false
  for (int i = 0; i < num_states; i++) {
    nfa->accepting_states[i] = false;
  }

  return nfa;
}

void NFA_add_transition(NFA *nfa, int current_state, char symbol,
                        int target_state) {
  nfa->transition_function[current_state][(int)symbol][target_state] = true;
}

void NFA_set_accepting(NFA *nfa, int state, bool is_accepting) {
  nfa->accepting_states[state] = is_accepting;
}

bool NFA_is_accepting(NFA *nfa, int state) {
  return nfa->accepting_states[state];
}

// NFA ends with 'gh'
NFA *NFA_a() {
  NFA *nfa = NFA_create(3);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'g') {
      NFA_add_transition(nfa, 0, c, 0); // stay in state 0 if not 'g'
    }
  }

  NFA_add_transition(nfa, 0, 'g', 1); // enter state 1 if detects 'g'
  NFA_add_transition(nfa, 1, 'h', 2); // enter   state 2 if detects 'h'

  for (char c = 'a'; c <= 'z'; c++) {
    NFA_add_transition(
        nfa, 2, c, 0); // return to state 0 from state 2 if not end of string
  }

  NFA_set_accepting(nfa, 2, true); // Accepting state is 2
  return nfa;
}

NFA *NFA_b() {
  NFA *nfa = NFA_create(4);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'm') {
      NFA_add_transition(nfa, 0, c, 0); // stay in state 0 if not 'm'
    }
  }

  NFA_add_transition(nfa, 0, 'm', 1); // enter state 1 if detects 'm'
  NFA_add_transition(nfa, 1, 'o', 2); // enter state 2 if detects 'o'
  NFA_add_transition(nfa, 2, 'o', 3); // enter state 2 if detects 'o'

  for (char c = 'a'; c <= 'z'; c++) {
    NFA_add_transition(nfa, 3, c, 3); // stay in state 3 if not end of string
  }

  NFA_set_accepting(nfa, 3, true); // Accepting state is 3
  return nfa;
}

NFA *NFA_c() {
  NFA *nfa = NFA_create(16);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'a') {
      NFA_add_transition(nfa, 0, c, 0); // stay in state 0 if not 'a'
    }
  }

  NFA_add_transition(nfa, 0, 'a', 1);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'a') {
      NFA_add_transition(nfa, 1, c, 1); // stay in state 1 if not 'a'
    }
  }

  NFA_add_transition(nfa, 1, 'a', 2);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'a') {
      NFA_add_transition(nfa, 2, c, 2); // stay in state 2 if not 'a'
    }
  }

  NFA_set_accepting(nfa, 2, true); // String that has more than one 'a'

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'i') {
      NFA_add_transition(nfa, 0, c, 0); // stay in state 0 if not 'i'
    }
  }

  NFA_add_transition(nfa, 0, 'i', 3);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'i') {
      NFA_add_transition(nfa, 3, c, 3); // stay in state 3 if not 'i'
    }
  }

  NFA_add_transition(nfa, 3, 'i', 4);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'i') {
      NFA_add_transition(nfa, 4, c, 4); // stay in state 4 if not 'i'
    }
  }

  NFA_set_accepting(nfa, 4, true); // String that has more than one 'i'

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'y') {
      NFA_add_transition(nfa, 0, c, 0); // stay in state 0 if not 'y'
    }
  }

  NFA_add_transition(nfa, 0, 'y', 5);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'y') {
      NFA_add_transition(nfa, 5, c, 5); // stay in state 5 if not 'y'
    }
  }

  NFA_add_transition(nfa, 5, 'y', 6);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'y') {
      NFA_add_transition(nfa, 6, c, 6); // stay in state 6 if not 'y'
    }
  }

  NFA_add_transition(nfa, 6, 'y', 7);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'y') {
      NFA_add_transition(nfa, 7, c, 7); // stay in state 7 if not 'y'
    }
  }

  NFA_set_accepting(nfa, 7, true); // String that has more than two 'y'

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'c') {
      NFA_add_transition(nfa, 0, c, 0); // stay in state 0 if not 'c'
    }
  }

  NFA_add_transition(nfa, 0, 'c', 8);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'c') {
      NFA_add_transition(nfa, 8, c, 8); // stay in state 8 if not 'c'
    }
  }

  NFA_add_transition(nfa, 8, 'c', 9);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'c') {
      NFA_add_transition(nfa, 9, c, 9); // stay in state 9 if not 'c'
    }
  }

  NFA_add_transition(nfa, 9, 'c', 10);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'c') {
      NFA_add_transition(nfa, 10, c, 10); // stay in state 10 if not 'c'
    }
  }

  NFA_add_transition(nfa, 10, 'c', 11);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'c') {
      NFA_add_transition(nfa, 11, c, 11); // stay in state 11 if not 'c'
    }
  }

  NFA_set_accepting(nfa, 11, true); // String that has more than three 'c'

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'l') {
      NFA_add_transition(nfa, 0, c, 0); // stay in state 0 if not 'l'
    }
  }

  NFA_add_transition(nfa, 0, 'l', 12);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'l') {
      NFA_add_transition(nfa, 12, c, 12); // stay in state 12 if not 'l'
    }
  }

  NFA_add_transition(nfa, 12, 'l', 13);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'l') {
      NFA_add_transition(nfa, 13, c, 13); // stay in state 13 if not 'l'
    }
  }

  NFA_add_transition(nfa, 13, 'l', 14);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'l') {
      NFA_add_transition(nfa, 14, c, 14); // stay in state 14 if not 'l'
    }
  }

  NFA_add_transition(nfa, 14, 'l', 15);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'l') {
      NFA_add_transition(nfa, 15, c, 15); // stay in state 15 if not 'l'
    }
  }

  NFA_set_accepting(nfa, 15, true); // String that has more than three 'l'

  return nfa;
}

bool NFA_run(NFA *nfa, char *input) {
  // bool current_states: determine whether the current state is active or not
  // active: true, inactive: false
  bool current_states[100] = {false};

  // Set the initial state to active
  current_states[0] = true;

  // Loop through each character in the input string
  for (int i = 0; i < strlen(input); i++) {
    char current_symbol = input[i];  // Read the current symbol
    bool next_states[100] = {false}; // Initialize the next states to inactive

    // Loop through each state in the NFA
    for (int state = 0; state < nfa->num_states; state++) {
      // Check if the current state is active
      if (current_states[state]) {
        // If it is, loop through each possible next state and check if there is
        // a transition for the current symbol
        for (int next_state = 0; next_state < nfa->num_states; next_state++) {
          // If there is a transition, set the next state to active
          if (nfa->transition_function[state][(int)current_symbol]
                                      [next_state]) {
            next_states[next_state] = true;
          }
        }
      }
    }

    // Update the current states to the next states
    for (int state = 0; state < nfa->num_states; state++) {
      current_states[state] = next_states[state];
    }
  }

  // Check if any of the current states are accepting states
  // If so, the NFA accepts the input string
  // If not, the NFA rejects the input string
  for (int state = 0; state < nfa->num_states; state++) {
    if (current_states[state] && nfa->accepting_states[state]) {
      return true;
    }
  }

  return false;
}

void NFA_repl(NFA *nfa) {
  char input[256]; // Stores the input string

  while (true) {
    printf("Enter a string (or 'quit' to quit):\n");

    if (fgets(input, sizeof(input), stdin) == NULL) {
      break;
    }

    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "quit") == 0) {
      break;
    }

    bool result = NFA_run(nfa, input);
    if (result) {
      printf("The NFA accepted the string: %s\n", input);
    } else {
      printf("The NFA rejected the string: %s\n", input);
    }
  }
}


// Helper function prototypes for epsilon closure and move
int* epsilon_closure(NFA* nfa, int* states, int num_states, int* result_size);
int* move(NFA* nfa, int* states, int num_states, char symbol, int* result_size);

// Convert NFA to DFA using subset construction
DFA* NFA_to_DFA(NFA* nfa) {
    // Create a new DFA
    DFA* dfa = CreateDFA(nfa->num_states * MAX_STATES, NULL, 0, 0); // estimate max DFA states

    // Track DFA states created from NFA state sets
    int dfa_state_count = 0;
    int* dfa_states[MAX_STATES]; // Array to hold NFA state sets
    int dfa_state_sizes[MAX_STATES]; // Number of states in each set
    bool dfa_accepting[MAX_STATES]; // Track accepting DFA states

    // Worklist to process DFA states
    int worklist[MAX_STATES];
    int worklist_size = 0;

    // Start with the epsilon closure of NFA's start state
    int start_set[MAX_STATES];
    int start_set_size;
    int* start_closure = epsilon_closure(nfa, &nfa->initial_state, 1, &start_set_size);
    dfa_states[dfa_state_count] = start_closure;
    dfa_state_sizes[dfa_state_count] = start_set_size;
    worklist[worklist_size++] = dfa_state_count;

    // Mark DFA state as accepting if any NFA state in the set is accepting
    dfa_accepting[dfa_state_count] = false;
    for (int i = 0; i < start_set_size; i++) {
        if (NFA_is_accepting(nfa, start_closure[i])) {
            dfa_accepting[dfa_state_count] = true;
            break;
        }
    }
    dfa_state_count++;

    // Process each DFA state in the worklist
    while (worklist_size > 0) {
        int current_dfa_state = worklist[--worklist_size];
        int* current_set = dfa_states[current_dfa_state];
        int current_set_size = dfa_state_sizes[current_dfa_state];

        // For each symbol, compute the DFA transition
        for (int symbol = 0; symbol < MAX_SYMBOLS; symbol++) {
            int next_set[MAX_STATES];
            int next_set_size;
            int* next_closure = move(nfa, current_set, current_set_size, symbol, &next_set_size);

            if (next_set_size > 0) {
                // Check if the new set of NFA states already exists in the DFA
                int next_dfa_state = -1;
                for (int i = 0; i < dfa_state_count; i++) {
                    if (dfa_state_sizes[i] == next_set_size && memcmp(dfa_states[i], next_closure, next_set_size * sizeof(int)) == 0) {
                        next_dfa_state = i;
                        break;
                    }
                }

                // If the new DFA state doesn't exist, add it
                if (next_dfa_state == -1) {
                    next_dfa_state = dfa_state_count++;
                    dfa_states[next_dfa_state] = next_closure;
                    dfa_state_sizes[next_dfa_state] = next_set_size;
                    worklist[worklist_size++] = next_dfa_state;

                    // Mark DFA state as accepting if any NFA state in the set is accepting
                    dfa_accepting[next_dfa_state] = false;
                    for (int i = 0; i < next_set_size; i++) {
                        if (NFA_is_accepting(nfa, next_closure[i])) {
                            dfa_accepting[next_dfa_state] = true;
                            break;
                        }
                    }
                }
                setTransition(dfa, symbol, current_dfa_state, next_dfa_state);
            }
        }
    }
    return dfa;
}

// Epsilon closure helper function
int* epsilon_closure(NFA* nfa, int* states, int num_states, int* result_size) {
    static int closure[MAX_STATES];
    bool visited[MAX_STATES] = {false};

    int closure_size = 0;
    for (int i = 0; i < num_states; i++) {
        closure[closure_size++] = states[i];
        visited[states[i]] = true;
    }
    *result_size = closure_size;
    return closure;
}

// Move function to compute the set of NFA states reachable by a symbol
int* move(NFA* nfa, int* states, int num_states, char symbol, int* result_size) {
    static int next_set[MAX_STATES];
    int next_size = 0;

    for (int i = 0; i < num_states; i++) {
        for (int j = 0; j < nfa->num_states; j++) {
            if (nfa->transition_function[states[i]][(int)symbol][j]) {
                next_set[next_size++] = j;
            }
        }
    }

    *result_size = next_size;
    return next_set;
}


int main() {
  // DFA (a) exactly the string "xyzzy"
  printf("Testing DFA that recognizes exactly 'xyzzy':\n");
  DFA *dfa_a = DFA_xyzzy();
  DFA_repl(dfa_a);
  printf("\n");

  for (int i = 0; i < dfa_a->set_of_states; i++)
    free(dfa_a->transition_function[i]); // free memory
  free(dfa_a->transition_function);
  free(dfa_a);

  // DFA (b) contains 9, 8, 7 in order
  printf("Testing DFA that recognizes '9, 8, and 7 in order':\n");
  DFA *dfa_b = DFA_987();
  DFA_repl(dfa_b);
  printf("\n");

  for (int i = 0; i < dfa_b->set_of_states; i++)
    free(dfa_b->transition_function[i]);
  free(dfa_b->transition_function);
  free(dfa_b);

  // DFA (c) contains two or three 4
  printf("Testing DFA that contains two or three 4:\n");
  DFA *dfa_c = DFA_4();
  DFA_repl(dfa_c);
  printf("\n");

  for (int i = 0; i < dfa_c->set_of_states; i++) {
    free(dfa_c->transition_function[i]);
  }
  free(dfa_c->transition_function);
  free(dfa_c);

  // DFA (d) binary input
  printf("Testing DFA that are an odd numbe of '0's and also an odd number of "
         "'1's :\n");
  DFA *dfa_d = DFA_bit();
  DFA_repl(dfa_d);
  printf("\n");

  for (int i = 0; i < dfa_d->set_of_states; i++) {
    free(dfa_d->transition_function[i]);
  }
  free(dfa_d->transition_function);
  free(dfa_d);

  // NFA (a) ends with 'gh'
  NFA *nfa_a = NFA_a();

  printf("Testing NFA for strings ending with 'gh':\n");
  NFA_repl(nfa_a);
  printf("\n");

  free(nfa_a);

  // NFA (b) contains 'moo'
  NFA *nfa_b = NFA_b();

  printf("Testing NFA for strings containing 'moo':\n");
  NFA_repl(nfa_b);
  printf("\n");

  free(nfa_b);

  // NFA (c) contains more than one 'a' or 'i', or more than two 'y's, or more
  // than 3 'c's or 'l's
  NFA *nfa_c = NFA_c();

  printf("Testing NFA for strings that have more than one 'a' or 'i', or more "
         "than two "
         "'y's, or more than three 'c's or 'l's:\n");
  NFA_repl(nfa_c);
  printf("\n");

  free(nfa_c);

  return 0;
}
