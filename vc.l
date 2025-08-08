%{
#include <stdio.h>

int vowels = 0, consonants = 0;
%}

%%
[aeiouAEIOU]    { vowels++; }
[b-df-hj-np-tv-zB-DF-HJ-NP-TV-Z] { consonants++; }
.|\n           { /* Ignore others */ }
%%

int main() {
    printf("Enter a string (Ctrl+D to end):\n");
    yylex();
    printf("Vowels: %d\nConsonants: %d\n", vowels, consonants);
    return 0;
}

int yywrap() {
    return 1;
}

