//
//  main.c
//  DFA
//
//  Created by Maki Nampei on 2024/9/8.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    int set_of_states;
    int* accept_states; // array of accepting states
    int** transition_function; // 2D array of transitions
    int initial_state;
    int set_accepting_states;
} DFA;

DFA* CreateDFA(int set_of_states, int* accept_states, int set_accepting_states, int initial_state)
{
    DFA* dfa = (DFA*)malloc(sizeof(DFA));
    dfa->set_of_states = set_of_states;
    //dfa->accept_states = accept_states;
    dfa->set_accepting_states = set_accepting_states;
    dfa->initial_state = initial_state;
   
    dfa->accept_states = (int*)malloc(set_accepting_states * sizeof(int));
        for (int i = 0; i < set_accepting_states; i++) {
            dfa->accept_states[i] = accept_states[i];
        }
    
    dfa->transition_function = (int**)malloc(set_of_states * sizeof(int*));
    for (int i=0; i<set_of_states; i++) // Allocate memory for each state in the transition function
    {
        dfa->transition_function[i] = (int*)malloc(128 * sizeof(int)); //ASCII(0 to 128)
        for (int j=0; j<128; j++) dfa->transition_function[i][j] = -1; //-1 means invalid
    }
    return dfa;
}

void setTransition(DFA* dfa, char input, int state, int nextState)
{
    dfa->transition_function[state][(int)input]=nextState;
    // The transition function determines the next state given the current state and input
}

bool checkState(DFA* dfa, int state) 
{
    for (int i=0; i<dfa->set_accepting_states; i++)
    {
        if (dfa->accept_states[i]==state) return true;
        //printf("%d\n", state);
    }
    return false;
}

bool DFA_run(DFA* dfa, char* input) 
{
    int current_state=dfa->initial_state;
    for (int i = 0; i < strlen(input); i++)
    {
        char symbol = input[i]; // Get the current symbol from the input
        current_state = dfa->transition_function[current_state][(int)symbol];
        // Get the next state from the transition function
        if (current_state == -1) return false;
    }
    return checkState(dfa, current_state);
}

void DFA_repl(DFA* dfa) 
{
    char input[256]; // Define a char array to store the input (which is a string)
    while (true)
    {
        printf("Enter an input (or 'quit' to exit):\n");
        fgets(input, sizeof(input), stdin);
        // Get input from the user (char *str, int n, FILE *stream)
        input[strcspn(input, "\n")] = '\0';
        //delete "enter"
        if (strcmp(input, "quit") == 0) break;
        // "strcmp" returns 0 if the string is equal to "quit"
        if (DFA_run(dfa, input)) printf("true\n"); // Check if the input is accepted by the DFA
        else printf("false\n");
    }
}

DFA* DFA_xyzzy(void)
{
    int accept_states[] = {5};
    DFA* dfa = CreateDFA(6, accept_states, 1, 0);
    setTransition(dfa, 'x', 0, 1);
    setTransition(dfa, 'y', 1, 2);
    setTransition(dfa, 'z', 2, 3);
    setTransition(dfa, 'z', 3, 4);
    setTransition(dfa, 'y', 4, 5);
    return dfa;
}

DFA* DFA_987(void) 
{
    int accept_states[] = {3}; // State 3 is the accepting state
    DFA* dfa = CreateDFA(4, accept_states, 1, 0);

    //State 0
    setTransition(dfa, '9', 0, 1); // Move to state 1 on '9'
    for (int i = 0; i < 128; i++) if (i != '9') setTransition(dfa, (char)i, 0, 0); // Stay in state 0 on any input except '9'

    //State 1
    setTransition(dfa, '8', 1, 2); // Move to state 2 on '8'
    setTransition(dfa, '9', 1, 1); // Stay in state 1 on '9'
    for (int i = 0; i < 128; i++) if (i != '8' && i != '9') setTransition(dfa, (char)i, 1, 1); // Stay in state 1 on any input except '8'

    //State 2
    setTransition(dfa, '7', 2, 3); // Move to state 3 (accepting) on '7'
    setTransition(dfa, '9', 2, 1); // Move back to state 1 on '9'
    setTransition(dfa, '8', 2, 2); // Stay in state 2 on '8'
    for (int i = 0; i < 128; i++) if (i != '7' && i != '9' && i != '8') setTransition(dfa, (char)i, 2, 2); // Stay in state 2 on any input except '7', '9', '8'

    //State 3
    for (int i = 0; i < 128; i++) setTransition(dfa, (char)i, 3, 3); // Stay in state 3 on any input
    return dfa;
}

DFA* DFA_4(void)
{ 
    int accept_states[] = {2, 3}; // Accept state 2 and 3
    DFA* dfa = CreateDFA(5, accept_states, 2, 0); // 5 states in total

    setTransition(dfa, '4', 0, 1); // Transitions for state 0 (no '4' yet)
    for (int i = 0; i < 128; i++) if (i != '4') setTransition(dfa, (char)i, 0, 0);

    setTransition(dfa, '4', 1, 2); //second '4'
    for (int i = 0; i < 128; i++) if (i != '4') setTransition(dfa, (char)i, 1, 1);

    setTransition(dfa, '4', 2, 3); //third '4'
    for (int i = 0; i < 128; i++) if (i != '4') setTransition(dfa, (char)i, 2, 2);

    //three '4's
    setTransition(dfa, '4', 3, 4);
    for (int i = 0; i < 128; i++) if (i != '4') setTransition(dfa, (char)i, 3, 3);

    //more than three '4's
    for (int i = 0; i < 128; i++) setTransition(dfa, (char)i, 4, 4); // Stay in state 4+not accepting
    return dfa;

}

DFA* DFA_bit(void)
{
    int accept_states[] = {3}; //"11" is the accepting state
    DFA* dfa = CreateDFA(4, accept_states, 1, 0); // 4 states: "00", "01", "10", "11"

    // State 0 -- "00" (even 0's & even 1's)
    setTransition(dfa, '0', 0, 2); // Move to state "10" on '0'
    setTransition(dfa, '1', 0, 1); // Move to state "01" on '1'

    // State 1 -- "01" (even 0's & odd 1's)
    setTransition(dfa, '0', 1, 3); // Move to state "11" on '0'
    setTransition(dfa, '1', 1, 0); // Move to state "00" on '1'

    // State 2 -- "10" (odd 0's & even 1's)
    setTransition(dfa, '0', 2, 0); // Move to state "00" on '0'
    setTransition(dfa, '1', 2, 3); // Move to state "11" on '1'

    // State 3 -- "11" (odd 0's & odd 1's) -> accepting state
    setTransition(dfa, '0', 3, 1); // Move to state "01" on '0'
    setTransition(dfa, '1', 3, 2); // Move to state "10" on '1'
    return dfa;
}

int main(int argc, const char * argv[])
{
    //DFA (a) Exactly the string "xyzzy"
    
    printf("Testing DFA that recognizes exactly 'xyzzy':\n");
    DFA* dfa_a = DFA_xyzzy();
    DFA_repl(dfa_a);

    for (int i = 0; i<dfa_a->set_of_states; i++) free(dfa_a->transition_function[i]); //free memory
    free(dfa_a->transition_function);
    free(dfa_a);
    
    //DFA (b) contains 9, 8, 7 in order
    printf("Testing DFA that recognizes '9, 8, and 7 in order':\n");
    DFA* dfa_b = DFA_987();
    DFA_repl(dfa_b);

    for (int i = 0; i<dfa_b->set_of_states; i++) free(dfa_b->transition_function[i]);
    free(dfa_b->transition_function);
    free(dfa_b);
    
    //DFA (c) contains two or three 4
    printf("Testing DFA that contains two or three 4:\n");
    DFA* dfa_c = DFA_4();
    DFA_repl(dfa_c);

    for (int i = 0; i<dfa_c->set_of_states; i++) free(dfa_c->transition_function[i]);
    free(dfa_c->transition_function);
    free(dfa_c);
    
    //DFA (d) binary input
    printf("Testing DFA that are an odd numbe of '0's and also an odd number of '1's :\n");
    DFA* dfa_d = DFA_bit();
    DFA_repl(dfa_d);

    for (int i = 0; i<dfa_d->set_of_states; i++) free(dfa_d->transition_function[i]);
    free(dfa_d->transition_function);
    free(dfa_d);
    
    return 0;
}
