#include <stdio.h>
#include <string.h>
#include <ctype.h>

void generate_assembly(char *line) {
    char result[10], operand1[10], operator, operand2[10];

    if (sscanf(line, "%s = %s %c %s", result, operand1, &operator, operand2) == 4) {
        printf("\t; %s\n", line);  
        
        if (isalpha(operand1[0])) {
            printf("\tMOV AX, [%s]\n", operand1);
        } else {
            printf("\tMOV AX, %s\n", operand1);
        }
        
        switch (operator) {
            case '+':
                if (isalpha(operand2[0])) {
                    printf("\tADD AX, [%s]\n", operand2);
                } else {
                    printf("\tADD AX, %s\n", operand2);
                }
                break;
                
            case '-':
                if (isalpha(operand2[0])) {
                    printf("\tSUB AX, [%s]\n", operand2);
                } else {
                    printf("\tSUB AX, %s\n", operand2);
                }
                break;
                
            case '*':
                if (isalpha(operand2[0])) {
                    printf("\tMOV BX, [%s]\n", operand2);
                } else {
                    printf("\tMOV BX, %s\n", operand2);
                }
                printf("\tMUL BX\n");
                break;
                
            case '/':
                printf("\tMOV DX, 0\n");
                if (isalpha(operand2[0])) {
                    printf("\tMOV BX, [%s]\n", operand2);
                } else {
                    printf("\tMOV BX, %s\n", operand2);
                }
                printf("\tDIV BX\n");
                break;
        }
        
        printf("\tMOV [%s], AX\n", result);
        printf("\n");
    }

    else if (sscanf(line, "%s = %s", result, operand1) == 2) {
        printf("\t; %s\n", line);
        if (isalpha(operand1[0])) {
            printf("\tMOV AX, [%s]\n", operand1);
            printf("\tMOV [%s], AX\n", result);
        } else {
            printf("\tMOV [%s], %s\n", result, operand1);
        }
        printf("\n");
    }
    else if (strncmp(line, "if", 2) == 0) {
        char var1[10], op[3], var2[10], label[10];
        if (sscanf(line, "if %s %s %s goto %s", var1, op, var2, label) == 4) {
            printf("\t; %s\n", line);
            printf("\tMOV AX, [%s]\n", var1);
            printf("\tCMP AX, [%s]\n", var2);
            
            if (strcmp(op, "==") == 0) {
                printf("\tJE %s\n", label);
            } else if (strcmp(op, "!=") == 0) {
                printf("\tJNE %s\n", label);
            } else if (strcmp(op, "<") == 0) {
                printf("\tJL %s\n", label);
            } else if (strcmp(op, ">") == 0) {
                printf("\tJG %s\n", label);
            } else if (strcmp(op, "<=") == 0) {
                printf("\tJLE %s\n", label);
            } else if (strcmp(op, ">=") == 0) {
                printf("\tJGE %s\n", label);
            }
            printf("\n");
        }
    }
    else if (strncmp(line, "goto", 4) == 0) {
        char label[10];
        if (sscanf(line, "goto %s", label) == 1) {
            printf("\t; %s\n", line);
            printf("\tJMP %s\n", label);
            printf("\n");
        }
    }
    else if (line[strlen(line) - 1] == ':') {
        printf("%s\n", line);
    }
}

int main() {
    int n;
    char line[100][100]; 
    
    printf("Enter number of three-address code lines: ");
    scanf("%d", &n);
    getchar();
    
    printf("Enter the three-address code:\n");
    
    for (int i = 0; i < n; i++) {
        fgets(line[i], sizeof(line[i]), stdin);
        line[i][strcspn(line[i], "\n")] = 0; 
    }


    printf("\n8086 Assembly Code:\n");
    printf(".MODEL SMALL\n");
    printf(".DATA\n");
    printf("\t; Variable declarations will be here\n");
    printf(".CODE\n");
    printf("MAIN PROC\n");
    printf("\tMOV AX, @DATA\n");
    printf("\tMOV DS, AX\n\n");


    for (int i = 0; i < n; i++) {
        if (strlen(line[i]) > 0) {
            generate_assembly(line[i]);
        }
    }

    printf("\tMOV AH, 4CH\n");
    printf("\tINT 21H\n");
    printf("MAIN ENDP\n");
    printf("END MAIN\n");

    return 0;
}



