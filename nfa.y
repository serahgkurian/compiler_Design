%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_ALPHABET 26
#define MAX_TRANSITIONS 1000
#define MAX_SET_SIZE 100

typedef struct {
    int from_state;
    char symbol;
    int to_state;
} Transition;

typedef struct {
    int states[MAX_SET_SIZE];
    int count;
} StateSet;

int num_states;
char alphabet[MAX_ALPHABET];
int alphabet_size;
Transition transitions[MAX_TRANSITIONS];
int num_transitions;
int start_state;
int accept_states[MAX_STATES];
int num_accept_states;

StateSet dfa_states[MAX_STATES];
int dfa_transitions[MAX_STATES][MAX_ALPHABET];
int dfa_accept[MAX_STATES];
int num_dfa_states;

extern int yylex();
extern int yyparse();
void yyerror(const char *s);

void print_nfa();
void nfa_to_dfa();
void print_dfa();
StateSet epsilon_closure(StateSet set);
StateSet move(StateSet set, char symbol);
int find_or_add_state(StateSet set);
void print_state_set(StateSet set);
int is_accept_state(StateSet set);
%}

%union {
    int num;
    char ch;
}

%token <num> NUMBER
%token <ch> SYMBOL
%token STATES ALPHABET TRANSITIONS START ACCEPT ARROW
%token '(' ')' ','

%%
nfa_spec:
    states_def alphabet_def transitions_def start_def accept_def
    {
        printf("NFA parsed successfully!\n");
        print_nfa();
        nfa_to_dfa();
        print_dfa();
    }
    ;

states_def:
    STATES NUMBER { num_states = $2; }
    ;

alphabet_def:
    ALPHABET '(' symbol_list ')'
    ;

transitions_def:
    TRANSITIONS transition_list
    ;

start_def:
    START NUMBER { start_state = $2; }
    ;

accept_def:
    ACCEPT '(' accept_list ')'
    ;

symbol_list:
    SYMBOL { alphabet[alphabet_size++] = $1; }
    | symbol_list ',' SYMBOL { alphabet[alphabet_size++] = $3; }
    ;

transition_list:
    transition
    | transition_list transition
    ;

transition:
    NUMBER SYMBOL ARROW NUMBER
    {
        transitions[num_transitions].from_state = $1;
        transitions[num_transitions].symbol = $2;
        transitions[num_transitions].to_state = $4;
        num_transitions++;
    }
    ;

accept_list:
    NUMBER { accept_states[num_accept_states++] = $1; }
    | accept_list ',' NUMBER { accept_states[num_accept_states++] = $3; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

void print_nfa() {
    printf("\n=== NFA ===\n");
    printf("States: %d\n", num_states);
    printf("Alphabet: ");
    for (int i = 0; i < alphabet_size; i++) {
        printf("%c ", alphabet[i]);
    }
    printf("\nStart state: %d\n", start_state);
    printf("Accept states: ");
    for (int i = 0; i < num_accept_states; i++) {
        printf("%d ", accept_states[i]);
    }
    printf("\nTransitions:\n");
    for (int i = 0; i < num_transitions; i++) {
        printf("  %d ---%c---> %d\n", 
               transitions[i].from_state, 
               transitions[i].symbol, 
               transitions[i].to_state);
    }
}

StateSet epsilon_closure(StateSet set) {
    // For simplicity, this implementation doesn't handle epsilon transitions
    // In a full implementation, you would add all states reachable via epsilon
    return set;
}

StateSet move(StateSet set, char symbol) {
    StateSet result;
    result.count = 0;
    
    for (int i = 0; i < set.count; i++) {
        int state = set.states[i];
        for (int j = 0; j < num_transitions; j++) {
            if (transitions[j].from_state == state && 
                transitions[j].symbol == symbol) {
                // Add to result if not already present
                int already_present = 0;
                for (int k = 0; k < result.count; k++) {
                    if (result.states[k] == transitions[j].to_state) {
                        already_present = 1;
                        break;
                    }
                }
                if (!already_present) {
                    result.states[result.count++] = transitions[j].to_state;
                }
            }
        }
    }
    
    return epsilon_closure(result);
}

int find_or_add_state(StateSet set) {
    // Check if this state set already exists
    for (int i = 0; i < num_dfa_states; i++) {
        if (dfa_states[i].count == set.count) {
            int same = 1;
            for (int j = 0; j < set.count; j++) {
                int found = 0;
                for (int k = 0; k < dfa_states[i].count; k++) {
                    if (set.states[j] == dfa_states[i].states[k]) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    same = 0;
                    break;
                }
            }
            if (same) return i;
        }
    }
    
    // Add new state
    dfa_states[num_dfa_states] = set;
    dfa_accept[num_dfa_states] = is_accept_state(set);
    return num_dfa_states++;
}

int is_accept_state(StateSet set) {
    for (int i = 0; i < set.count; i++) {
        for (int j = 0; j < num_accept_states; j++) {
            if (set.states[i] == accept_states[j]) {
                return 1;
            }
        }
    }
    return 0;
}

void nfa_to_dfa() {
    num_dfa_states = 0;
    
    // Initialize DFA transitions table
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_ALPHABET; j++) {
            dfa_transitions[i][j] = -1;
        }
    }
    
    // Start with the epsilon closure of start state
    StateSet start_set;
    start_set.count = 1;
    start_set.states[0] = start_state;
    start_set = epsilon_closure(start_set);
    
    find_or_add_state(start_set);
    
    // Process each DFA state
    int processed = 0;
    while (processed < num_dfa_states) {
        StateSet current = dfa_states[processed];
        
        // For each symbol in alphabet
        for (int sym = 0; sym < alphabet_size; sym++) {
            StateSet next = move(current, alphabet[sym]);
            
            if (next.count > 0) {
                int next_state = find_or_add_state(next);
                dfa_transitions[processed][sym] = next_state;
            }
        }
        processed++;
    }
}

void print_state_set(StateSet set) {
    printf("{");
    for (int i = 0; i < set.count; i++) {
        printf("%d", set.states[i]);
        if (i < set.count - 1) printf(",");
    }
    printf("}");
}

void print_dfa() {
    printf("\n=== DFA ===\n");
    printf("States: %d\n", num_dfa_states);
    
    printf("State mapping:\n");
    for (int i = 0; i < num_dfa_states; i++) {
        printf("  q%d = ", i);
        print_state_set(dfa_states[i]);
        if (dfa_accept[i]) printf(" (accept)");
        printf("\n");
    }
    
    printf("\nTransition table:\n");
    printf("State\t");
    for (int i = 0; i < alphabet_size; i++) {
        printf("%c\t", alphabet[i]);
    }
    printf("\n");
    
    for (int i = 0; i < num_dfa_states; i++) {
        printf("q%d\t", i);
        for (int j = 0; j < alphabet_size; j++) {
            if (dfa_transitions[i][j] != -1) {
                printf("q%d\t", dfa_transitions[i][j]);
            } else {
                printf("-\t");
            }
        }
        printf("\n");
    }
    
    printf("\nStart state: q0\n");
    printf("Accept states: ");
    for (int i = 0; i < num_dfa_states; i++) {
        if (dfa_accept[i]) {
            printf("q%d ", i);
        }
    }
    printf("\n");
}

int main() {
    printf("Enter NFA specification:\n");
    printf("Format:\n");
    printf("states <number>\n");
    printf("alphabet (<symbol>, <symbol>, ...)\n");
    printf("transitions\n");
    printf("<from> <symbol> -> <to>\n");
    printf("...\n");
    printf("start <state>\n");
    printf("accept (<state>, <state>, ...)\n\n");
    
    return yyparse();
}
