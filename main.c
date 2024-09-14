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
    int* accept_states;
    int** transition_function;
    int initial_state;
    int set_accepting_states;
} DFA;

DFA* CreateDFA(int set_of_states, int* accept_states, int set_accepting_states, int initial_state)
{
    DFA* dfa = (DFA*)malloc(sizeof(DFA));
    dfa->set_of_states = set_of_states;
    dfa->accept_states = accept_states;
    dfa->set_accepting_states = set_accepting_states;
    dfa->initial_state = initial_state;
   
    dfa->transition_function = (int**)malloc(set_of_states * sizeof(int*));
    for (int i=0; i<set_of_states; i++)
    {
        dfa->transition_function[i] = (int*)malloc(128 * sizeof(int)); //ASCII inputs(0 to 128)
        for (int j=0; j<128; j++) dfa->transition_function[i][j] = -1; //-1 means invalid
    }
    return dfa;
}

void setTransition(DFA* dfa, char input, int state, int nextState)
{
    dfa->transition_function[state][(int)input]=nextState;
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
        printf("Enter an input (or 'quit' to exit):\n");
        fgets(input, sizeof(input), stdin);

        if (strcmp(input, "quit") == 0) break;
        if (DFA_run(dfa, input)) printf("true\n");
        else printf("false\n");
    }
}

DFA* DFA_xyzzy(void)
{
    int accept_states[] = {6};
    DFA* dfa = CreateDFA(6, accept_states, 1, 0);

    setTransition(dfa, 'x', 1, 0);
    setTransition(dfa, 'y', 2, 1);
    setTransition(dfa, 'z', 3, 2);
    setTransition(dfa, 'z', 4, 3);
    setTransition(dfa, 'y', 5, 4);
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
