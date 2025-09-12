#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char input[100];   // increased size for longer expressions
int i = 0, valid = 1;

void E();
void E_dash();
void T();
void T_dash();
void F();

void E()
{
    T();
    E_dash();
}

void E_dash()
{
    if (input[i] == '+')
    {
        i++;
        T();
        E_dash();
    }
}

void T()
{
    F();
    T_dash();
}

void T_dash()
{
    if (input[i] == '*')
    {
        i++;
        F();
        T_dash();
    }
}

void F()
{
    if (input[i] == '(')
    {
        i++;
        E();
        if (input[i] == ')')
            i++;
        else  
            valid = 0;
    }
    else if (isalnum(input[i]))
    {
        // Consume a full identifier (multi-character id allowed)
        while (isalnum(input[i]))
            i++;
    }
    else
    {
        valid = 0;
    }
}

int main()
{
    printf("Enter input string: ");
    scanf("%s", input);

    E();

    if (valid == 1 && i == strlen(input))
        printf("Accepted.\n");
    else  
        printf("Rejected.\n");

    return 0;
}
