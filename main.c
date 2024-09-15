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

int main(int argc, const char * argv[]) 
{
    printf("Testing DFA that recognizes exactly 'xyzzy':\n");
    DFA* dfa_a = DFA_xyzzy();
    DFA_repl(dfa_a);

    for (int i = 0; i<dfa_a->set_of_states; i++) free(dfa_a->transition_function[i]); //free memory?
    free(dfa_a->transition_function);
    free(dfa_a);
    
    return 0;
}
