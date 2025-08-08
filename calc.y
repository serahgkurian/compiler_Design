%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex(void);
%}

%token NUMBER

%left '+' '-'
%left '*' '/'
%right UMINUS

%%

input:
    /* empty */
    | input line
    ;

line:
      expr '\n' { printf("Result = %d\n", $1); }
    | '\n'
    ;

expr:
      NUMBER             { $$ = $1; }
    | expr '+' expr      { $$ = $1 + $3; }
    | expr '-' expr      { $$ = $1 - $3; }
    | expr '*' expr      { $$ = $1 * $3; }
    | expr '/' expr      {
                            if ($3 == 0) {
                                yyerror("Division by zero");
                                exit(1);
                            }
                            $$ = $1 / $3;
                         }
    | '-' expr %prec UMINUS { $$ = -$2; }
    | '(' expr ')'       { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter expressions:\n");
    yyparse();
    return 0;
}

