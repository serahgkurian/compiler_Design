/*
Constant propagation
The RHS of the expression can only have one operator and two operands, e.g. a=b+c
The input program MUST NOT HAVE ANY SPACES AND SEMICOLONS
The variable name CANNOT have more than one character, e.g. a=20 - valid, abc=20 - invalid
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// The values of number assignment statements are stored in this array
// If the statement is a=50, the number 50 will be stored at index 97 (ASCII value of 'a')
int values[150];

void main()
{
    int no_of_lines;
    char lines[20][20];
    printf("Enter no. of lines of the program: ");
    scanf("%d", &no_of_lines);
    printf("Enter the program:\n");
    for (int i = 0; i < no_of_lines; i++)
        scanf("%s", lines[i]);
    
    for (int i = 0; i < no_of_lines; i++)
    {
        // Check if line is a number assignment statement (eg:- a=2); flag is set to 1 if not
        int flag = 0;
        for (int j = 2; j < strlen(lines[i]); j++)
        {
            if (isalpha(lines[i][j]))
                flag = 1;
        }
        
        // Extract and store the number from the line
        // j = 2: RHS of the expression begins at index 2
        if (flag == 0)
        {
            int j;
            char temp[20];
            for (j = 2; j < strlen(lines[i]); j++)
                temp[j - 2] = lines[i][j];
            temp[j - 2] = '\0';
            int value = atoi(temp);
            values[lines[i][0]] = value;
        }
        // Arithmetic expression (eg:- b=a+c)
        else
        {
            int operand1, operand2;
            char operator;
            int pos = 2; // Start from position 2 (after '=')
            
            // Parse first operand
            if (isalpha(lines[i][pos]))
            {
                operand1 = values[lines[i][pos]];
                pos++;
            }
            else
            {
                char temp[20];
                int index = 0;
                while (pos < strlen(lines[i]) && isdigit(lines[i][pos]))
                {
                    temp[index++] = lines[i][pos++];
                }
                temp[index] = '\0';
                operand1 = atoi(temp);
            }
            
            // Get operator
            operator = lines[i][pos];
            pos++;
            
            // Parse second operand
            if (isalpha(lines[i][pos]))
            {
                operand2 = values[lines[i][pos]];
                pos++;
            }
            else
            {
                char temp[20];
                int index = 0;
                while (pos < strlen(lines[i]) && isdigit(lines[i][pos]))
                {
                    temp[index++] = lines[i][pos++];
                }
                temp[index] = '\0';
                operand2 = atoi(temp);
            }
            
            // Check if there's more to the expression (like -a in c=b*2-a)
            int result;
            switch (operator)
            {
                case '+':
                    result = operand1 + operand2;
                    break;
                case '-':
                    result = operand1 - operand2;
                    break;
                case '*':
                    result = operand1 * operand2;
                    break;
                case '/':
                    result = operand1 / operand2;
                    break;
            }
            
            // Check if there's another operator after the first calculation
            if (pos < strlen(lines[i]))
            {
                char second_operator = lines[i][pos];
                pos++;
                int operand3;
                
                if (isalpha(lines[i][pos]))
                {
                    operand3 = values[lines[i][pos]];
                }
                else
                {
                    char temp[20];
                    int index = 0;
                    while (pos < strlen(lines[i]) && isdigit(lines[i][pos]))
                    {
                        temp[index++] = lines[i][pos++];
                    }
                    temp[index] = '\0';
                    operand3 = atoi(temp);
                }
                
                printf("Line %d after constant propagation: %c=%d%c%d%c%d\n", 
                       i + 1, lines[i][0], operand1, operator, operand2, second_operator, operand3);
                
                // Apply second operation
                switch (second_operator)
                {
                    case '+':
                        result = result + operand3;
                        break;
                    case '-':
                        result = result - operand3;
                        break;
                    case '*':
                        result = result * operand3;
                        break;
                    case '/':
                        result = result / operand3;
                        break;
                }
            }
            else
            {
                printf("Line %d after constant propagation: %c=%d%c%d\n", 
                       i + 1, lines[i][0], operand1, operator, operand2);
            }
            
            printf("Result: %d\n", result);
            values[lines[i][0]] = result;
        }
    }
}
