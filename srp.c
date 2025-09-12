#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input[40], stack[40];
int input_ptr, stack_top;

// Grammar:
// E -> T E'
// E' -> + T E'
// E' -> ε
// T -> F T'
// T' -> * F T'
// T' -> ε
// F -> ( E )
// F -> id

void print_action(char* action) {
    printf("%-20s%-20s%-20s\n", stack, input + input_ptr, action);
}

int try_reduce() {
    int len = stack_top + 1;
    
    // Check all possible reductions in order of preference
    
    // F -> id
    if (len >= 2 && stack[stack_top-1] == 'i' && stack[stack_top] == 'd') {
        stack_top -= 2;
        stack[++stack_top] = 'F';
        stack[stack_top + 1] = '\0';
        print_action("Reduce F->id");
        return 1;
    }
    
    // F -> (E)
    if (len >= 3 && stack[stack_top-2] == '(' && stack[stack_top-1] == 'E' && stack[stack_top] == ')') {
        stack_top -= 3;
        stack[++stack_top] = 'F';
        stack[stack_top + 1] = '\0';
        print_action("Reduce F->(E)");
        return 1;
    }
    
    // T' -> *FT'
    if (len >= 3 && stack[stack_top-2] == '*' && stack[stack_top-1] == 'F' && stack[stack_top] == '\'') {
        stack_top -= 3;
        stack[++stack_top] = '\'';
        stack[stack_top + 1] = '\0';
        print_action("Reduce T'->*FT'");
        return 1;
    }
    
    // T' -> ε (when we have F and need to make it part of T)
    if (len >= 1 && stack[stack_top] == 'F' && 
        (input[input_ptr] == '+' || input[input_ptr] == '$' || input[input_ptr] == ')')) {
        stack[++stack_top] = '\'';
        stack[stack_top + 1] = '\0';
        print_action("Reduce T'->ε");
        return 1;
    }
    
    // T -> FT'
    if (len >= 2 && stack[stack_top-1] == 'F' && stack[stack_top] == '\'') {
        stack_top -= 2;
        stack[++stack_top] = 'T';
        stack[stack_top + 1] = '\0';
        print_action("Reduce T->FT'");
        return 1;
    }
    
    // E' -> +TE'
    if (len >= 3 && stack[stack_top-2] == '+' && stack[stack_top-1] == 'T' && stack[stack_top] == '\'') {
        stack_top -= 3;
        stack[++stack_top] = '\'';
        stack[stack_top + 1] = '\0';
        print_action("Reduce E'->+TE'");
        return 1;
    }
    
    // E' -> ε (when we have T and input is $ or ))
    if (len >= 1 && stack[stack_top] == 'T' && 
        (input[input_ptr] == '$' || input[input_ptr] == ')')) {
        stack[++stack_top] = '\'';
        stack[stack_top + 1] = '\0';
        print_action("Reduce E'->ε");
        return 1;
    }
    
    // E -> TE'
    if (len >= 2 && stack[stack_top-1] == 'T' && stack[stack_top] == '\'') {
        stack_top -= 2;
        stack[++stack_top] = 'E';
        stack[stack_top + 1] = '\0';
        print_action("Reduce E->TE'");
        return 1;
    }
    
    return 0;
}

int main() {
    input_ptr = 0;
    stack_top = -1;
    
    printf("Grammar:\n");
    printf("E -> T E'\n");
    printf("E' -> + T E'\n");
    printf("E' -> ε\n");
    printf("T -> F T'\n");
    printf("T' -> * F T'\n");
    printf("T' -> ε\n");
    printf("F -> ( E )\n");
    printf("F -> id\n\n");
    
    printf("Enter the input string: ");
    scanf("%s", input);
    strcat(input, "$");
    
    printf("\n%-20s%-20s%-20s\n", "Stack", "Input", "Action");
    printf("%-20s%-20s%-20s\n", "", input, "Initial");
    
    while (1) {
        // Try to reduce as much as possible
        while (try_reduce()) {
            // Check for acceptance after each reduction
            if (stack_top == 0 && stack[0] == 'E' && input[input_ptr] == '$') {
                printf("\nString ACCEPTED!\n");
                return 0;
            }
        }
        
        // Check for acceptance
        if (stack_top == 0 && stack[0] == 'E' && input[input_ptr] == '$') {
            printf("\nString ACCEPTED!\n");
            return 0;
        }
        
        // If we can't reduce and input is finished, reject
        if (input[input_ptr] == '$') {
            printf("\nString REJECTED!\n");
            return 0;
        }
        
        // Shift
        stack[++stack_top] = input[input_ptr++];
        stack[stack_top + 1] = '\0';
        print_action("Shift");
    }
    
    return 0;
}
