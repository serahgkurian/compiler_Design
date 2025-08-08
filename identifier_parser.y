/* C Declarations Section */
%{
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int yylex(void);
void yyerror(const char *s);

char identifier_buffer[256];
int buffer_index = 0;
%}

/* YACC Declarations Section */
%union {
    char char_val;
}
%token <char_val> LETTER
%token <char_val> DIGIT

%%
/* Grammar Rules Section */

/*
  FIX: The new 'input_stream' start rule allows the parser
  to process multiple lines in a loop.
*/
input_stream:
    /* empty */
    | input_stream line
    ;

/* A line contains an identifier, is empty, or has an error */
line:
    identifier '\n' {
        identifier_buffer[buffer_index] = '\0';
        /* Emoji-free version for compatibility */
        printf("Valid Identifier: %s\n> ", identifier_buffer);
        buffer_index = 0; // Reset for next input
    }
    | '\n' { printf("> "); }
    | error '\n' {
        yyerrok; /* On error, recover and wait for next line */
    }
    ;

/* An identifier starts with a LETTER */
identifier:
    LETTER {
        identifier_buffer[buffer_index++] = $1;
    }
    | identifier LETTER {
        identifier_buffer[buffer_index++] = $2;
    }
    | identifier DIGIT {
        identifier_buffer[buffer_index++] = $2;
    }
    ;

%%
/* C Code Section */

int main(void) {
    printf("Enter an identifier (or Ctrl+D to exit).\n> ");
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Invalid input. Identifiers must start with a letter and contain only letters or digits.\n> ");
    buffer_index = 0; // Reset buffer on error
}

int yylex(void) {
    int c = getchar();

    if (isalpha(c)) {
        yylval.char_val = (char)c;
        return LETTER;
    }
    if (isdigit(c)) {
        yylval.char_val = (char)c;
        return DIGIT;
    }
    if (c == EOF) {
        return 0;
    }
    return c;
}
