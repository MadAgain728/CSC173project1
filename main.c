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
  // Initialize the DFA using the parameters
  dfa->set_of_states = set_of_states;
  dfa->accept_states = accept_states;
  dfa->set_accepting_states = set_accepting_states;
  dfa->initial_state = initial_state;

  dfa->transition_function = (int **)malloc(set_of_states * sizeof(int *));
  // Initialize the transition function
  for (int i = 0; i < set_of_states; i++) {
    // Allocate memory for each state in the transition function
    // (an array for 128 integers corresponding to ASCII values)
    dfa->transition_function[i] = (int *)malloc(128 * sizeof(int));
    // Initialize each element in the transition function to -1(invalid)
    for (int j = 0; j < 128; j++)
      dfa->transition_function[i][j] = -1;
  }
  return dfa;
}

// Set the transition function for the DFA
// The transition function determines the next state given the current state and
// input
void setTransition(DFA *dfa, char input, int state, int nextState) {
  dfa->transition_function[state][(int)input] = nextState;
}

bool checkState(DFA *dfa, int state) {
  for (int i = 0; i < dfa->set_accepting_states; i++) {
    if (dfa->accept_states[i] == state)
      return true;
  }
  return false;
}

bool DFA_run(DFA *dfa, char *input) {
  int current_state = dfa->initial_state;
  for (int i = 0; i < strlen(input); i++) {
    // Get the current symbol from the input
    char symbol = input[i];
    // Get the next state from the transition function
    current_state = dfa->transition_function[current_state][(int)symbol];
    if (current_state == -1) // Invalid transition
      return false;
  }
  return checkState(dfa, current_state);
}

void DFA_repl(DFA *dfa) {
  // Define a char array to store the input (which is a string)
  char input[256];

  // Loop to get input from the user
  while (true) {
    printf("Enter an input (or 'quit' to exit):\n");
    // Get input from the user (char *str, int n, FILE *stream)
    fgets(input, sizeof(input), stdin);

    // "strcmp" returns 0 if the string is equal to "quit"
    if (strcmp(input, "quit") == 0)
      break;

    // Check if the input is accepted by the DFA
    if (DFA_run(dfa, input))
      printf("true\n");
    else
      printf("false\n");
  }
}

DFA *DFA_xyzzy(void) {
  int accept_states[] = {6};
  DFA *dfa = CreateDFA(6, accept_states, 1, 0);

  setTransition(dfa, 'x', 1, 0);
  setTransition(dfa, 'y', 2, 1);
  setTransition(dfa, 'z', 3, 2);
  setTransition(dfa, 'z', 4, 3);
  setTransition(dfa, 'y', 5, 4);
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
  nfa->transition_function[current_state][symbol][target_state] = true;
}

void NFA_set_accepting(NFA *nfa, int state, bool is_accepting) {
  nfa->accepting_states[state] = is_accepting;
}

bool NFA_is_accepting(NFA *nfa, int state) {
  return nfa->accepting_states[state];
}

NFA *NFA_for_ends_with_gh() {
  NFA *nfa = NFA_create(3);

  for (char c = 'a'; c <= 'z'; c++) {
    if (c != 'g') {
      NFA_add_transition(nfa, 0, c, 0); // stay in state 0 if not 'g'
    }
  }

  NFA_add_transition(nfa, 0, 'g', 1);
  NFA_add_transition(nfa, 1, 'h', 2);

  for (char c = 'a'; c <= 'z'; c++) {
    NFA_add_transition(
        nfa, 2, c, 0); // return to state 0 from state 2 if not end of string
  }

  NFA_set_accepting(nfa, 2, true); // Accepting state is 2
  return nfa;
}

bool NFA_run(NFA *nfa, char *input) {
  bool current_states[100] = {false};
  current_states[0] = true;

  for (int i = 0; i < strlen(input); i++) {
    char symbol = input[i];
    bool next_states[100] = {false};

    for (int state = 0; state < nfa->num_states; state++) {
      if (current_states[state]) {
        for (int next_state = 0; next_state < nfa->num_states; next_state++) {
          if (nfa->transition_function[state][symbol][next_state]) {
            next_states[next_state] = true;
          }
        }
      }
    }

    for (int state = 0; state < nfa->num_states; state++) {
      current_states[state] = next_states[state];
    }
  }

  for (int state = 0; state < nfa->num_states; state++) {
    if (current_states[state] && nfa->accepting_states[state]) {
      return true;
    }
  }

  return false;
}

void NFA_repl(NFA *nfa) {
  char input[256];
  while (true) {
    printf("Enter a string (or 'exit' to quit): ");
    scanf("%s", input);
    if (strcmp(input, "exit") == 0) {
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

int main() {
  NFA *nfa_ends_with_gh = NFA_for_ends_with_gh();

  printf("NFA for strings ending with 'gh':\n");
  NFA_repl(nfa_ends_with_gh);

  free(nfa_ends_with_gh);

  return 0;
}
