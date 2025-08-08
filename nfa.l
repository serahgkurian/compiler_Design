%{
#include "y.tab.h"
#include <stdlib.h>
%}

%%
[0-9]+          { yylval.num = atoi(yytext); return NUMBER; }
"states"        { return STATES; }
"alphabet"      { return ALPHABET; }
"transitions"   { return TRANSITIONS; }
"start"         { return START; }
"accept"        { return ACCEPT; }
"->"            { return ARROW; }
[a-zA-Z]        { yylval.ch = yytext[0]; return SYMBOL; }
[(),]           { return yytext[0]; }
[ \t\n]         { /* ignore whitespace */ }
.               { return yytext[0]; }
%%

int yywrap() {
    return 1;
}
