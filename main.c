#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//#include "dfa.h"

typedef struct 
{
    int set_of_states;              // set of states in DFA
    int* accept_states;          // Array of accept states
    int** transition_function;      // Transition function (** for 2D array)
    int initial_state;             // Initial state
    int set_accepting_states;       // Number of accept states
} DFA;

DFA* CreateDFA(int set_of_states, int* accept_states, int set_accepting_states, int initial_state)
{
    DFA* dfa = (DFA*)malloc(sizeof(DFA));
    dfa->set_of_states = set_of_states;
    dfa->accept_states = accept_states;
    dfa->set_accepting_states = set_accepting_states;
    dfa->initial_state = initial_state;
   
    dfa->transition_function = (int**)malloc(set_of_states * sizeof(int*));
    for (int i = 0; i < set_of_states; i++)
    {
        dfa->transition_function[i] = (int*)malloc(128 * sizeof(int)); //ASCII inputs(0 to 128)
        for (int j = 0; j < 128; j++) dfa->transition_function[i][j] = -1; //-1 means invalid
    }
    return dfa;
}

void setTransition(DFA* dfa, int state, char input, int nextState)
{
    dfa->transition_function[state][(int)input] = nextState;
}

bool checkState(DFA* dfa, int state) 
{
    for (int i=0; i<dfa->set_accepting_states; i++)
    {
        if (dfa->accept_states[i]==state) return true;
    }
    return false;
}

bool DFA_run(DFA* dfa, char* input) 
{
    int current_state = dfa->initial_state;
    for (int i = 0; i < strlen(input); i++) 
    {
        char symbol = input[i];
        current_state = dfa->transition_function[current_state][(int)symbol];
        if (current_state == -1) return false;
    }
    return checkState(dfa, current_state);
}

void DFA_repl(DFA* dfa) 
{
    char input[256];
    while (true) 
    {
        printf("Testing DFA that recognizes exactly '' ");
        printf("Enter an input (or 'quit' to exit): ");
        fgets(input, sizeof(input), stdin);

        if (strcmp(input, "quit") == 0) break;
        if (DFA_run(dfa, input)) printf("true\n");
        else printf("false\n");
    }
}

int main(int argc, const char * argv[]) {
    
    return 0;
}
