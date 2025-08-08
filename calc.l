%{
#include "y.tab.h"
#include <stdio.h>
%}

%%

[0-9]+      { yylval = atoi(yytext); return NUMBER; }
[+\-*/()]   { return yytext[0]; }
\n          { return '\n'; }
[ \t]       ;  // skip whitespace
.           { printf("Unknown character: %s\n", yytext); }

%%
int yywrap() {
    return 1;
}

