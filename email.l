%{
#include <stdio.h>
%}

%%
[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,} {
    printf("Valid Email Address: %s\n", yytext);
    exit(0);  // Exit after first match
}
.|\n {
    printf("Invalid Email Address\n");
    exit(0);  // Exit after one line
}
%%

int main() {
    printf("Enter an email address to validate:\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}

