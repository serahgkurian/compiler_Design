#include <stdio.h>
#include <string.h>

char exprsn[20];
char temp_var = 'Z';

void main()
{
    printf("Enter the expression: ");
    scanf("%s", exprsn);
    printf("Intermediate code:\n");
    
    // The loop runs until the RHS of the expression is reduced to exactly one operand
    // e.g. a=U - strlen(exprsn) = 3
    while (strlen(exprsn) >= 3)  // Changed from > 3 to >= 3
    {
        // Un-comment the below statement to see how the original string is modified
        // printf("%s\n", exprsn);
        // flag is set to 1 when a subexpression is reduced
        int flag = 0;
        
        // First pass: handle '/' and '*' (higher precedence)
        if (flag != 1)
        {
            for (int i = 0; i < strlen(exprsn); i++)
            {
                // '/' and '*' given together for equal precedence
                if (exprsn[i] == '/' || exprsn[i] == '*')
                {
                    printf("%c=%c%c%c\n", temp_var, exprsn[i - 1], exprsn[i], exprsn[i + 1]);
                    
                    // Substitute a new operand in the place of the subexpression
                    exprsn[i - 1] = temp_var;
                    temp_var--;
                    
                    // Shift and replace with the characters to the right of the subexpression 
                    for (int j = i; exprsn[j] != '\0'; j++)
                        exprsn[j] = exprsn[j + 2]; 
                    
                    flag = 1;
                    break;
                }
            }
        }
        
        // Second pass: handle '+' and '-' (lower precedence)
        if (flag != 1)
        {
            for (int i = 0; i < strlen(exprsn); i++)
            {
                // '+' and '-' given together for equal precedence
                if (exprsn[i] == '+' || exprsn[i] == '-')
                {
                    printf("%c=%c%c%c\n", temp_var, exprsn[i - 1], exprsn[i], exprsn[i + 1]);
                    
                    exprsn[i - 1] = temp_var;
                    temp_var--;
                    
                    for (int j = i; exprsn[j] != '\0'; j++)
                        exprsn[j] = exprsn[j + 2]; 
                    
                    flag = 1;
                    break;
                }
            }
        }
        
        // If no operation was processed, break to avoid infinite loop
        if (flag != 1)
            break;
    }
    
    printf("%s\n", exprsn);
}
