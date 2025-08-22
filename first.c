#include <stdio.h>
#include <ctype.h>
#include <string.h>

void FIRST(char[], char, char[]);
void addToResultSet(char[], char);
int numOfProductions;
char productionSet[10][10];

void main()
{
    int i, j;
    char c;
    char result[20];
    char visited[20]; // To track visited non-terminals
    
    printf("How many number of productions ? :");
    scanf(" %d", &numOfProductions);
    
    for (i = 0; i < numOfProductions; i++)
    {
        printf("Enter production Number %d : ", i + 1);
        scanf(" %s", productionSet[i]);
    }
    
    printf("FIRST OF : \n");
    
    // Get unique non-terminals
    char nonTerminals[10];
    int ntCount = 0;
    
    for (i = 0; i < numOfProductions; i++)
    {
        c = productionSet[i][0];
        int found = 0;
        for (j = 0; j < ntCount; j++)
        {
            if (nonTerminals[j] == c)
            {
                found = 1;
                break;
            }
        }
        if (!found)
        {
            nonTerminals[ntCount++] = c;
        }
    }
    
    // Calculate FIRST for each unique non-terminal
    for (i = 0; i < ntCount; i++)
    {
        c = nonTerminals[i];
        visited[0] = '\0'; // Reset visited array
        FIRST(result, c, visited);
        
        printf("FIRST(%c) = {", c);
        for (j = 0; result[j] != '\0'; j++)
        {
            printf("%c", result[j]);
            if (result[j + 1] != '\0') printf(", ");
        }
        printf("}\n");
    }
}

void FIRST(char *Result, char c, char *visited)
{
    int i, j, k;
    char subResult[20];
    int foundEpsilon;
    
    Result[0] = '\0';
    
    // Check if we've already visited this non-terminal (prevents infinite recursion)
    for (i = 0; visited[i] != '\0'; i++)
    {
        if (visited[i] == c)
            return;
    }
    
    // If X is terminal, FIRST(X) = {X}
    if (!(isupper(c)))
    {
        addToResultSet(Result, c);
        return;
    }
    
    // Add current non-terminal to visited list
    int visitedLen = strlen(visited);
    visited[visitedLen] = c;
    visited[visitedLen + 1] = '\0';
    
    // If X is non-terminal, read each production
    for (i = 0; i < numOfProductions; i++)
    {
        // Find production with X as LHS
        if (productionSet[i][0] == c)
        {
            // If X → ε is a production, then add ε to FIRST(X)
            if (productionSet[i][2] == '$')
            {
                addToResultSet(Result, '$');
            }
            else
            {
                j = 2;
                while (productionSet[i][j] != '\0')
                {
                    foundEpsilon = 0;
                    subResult[0] = '\0';
                    
                    FIRST(subResult, productionSet[i][j], visited);
                    
                    // Add all symbols from subResult to Result except epsilon
                    for (k = 0; subResult[k] != '\0'; k++)
                    {
                        if (subResult[k] != '$')
                            addToResultSet(Result, subResult[k]);
                    }
                    
                    // Check if epsilon is in FIRST of current symbol
                    for (k = 0; subResult[k] != '\0'; k++)
                    {
                        if (subResult[k] == '$')
                        {
                            foundEpsilon = 1;
                            break;
                        }
                    }
                    
                    // If no epsilon found, stop processing this production
                    if (!foundEpsilon)
                        break;
                        
                    j++;
                }
                
                // If epsilon was found in all symbols of the production, add epsilon to result
                if (foundEpsilon && j > 2)
                    addToResultSet(Result, '$');
            }
        }
    }
    
    // Remove current non-terminal from visited list
    visited[visitedLen] = '\0';
    return;
}

void addToResultSet(char Result[], char val)
{
    int k;
    for (k = 0; Result[k] != '\0'; k++)
        if (Result[k] == val)
            return;
    Result[k] = val;
    Result[k + 1] = '\0';
}
