#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LEN 100
#define MAX_IDS 100

char symbolTable[MAX_IDS][MAX_LEN];
int symbolCount = 0;

// Keywords list
const char *keywords[] = {"int", "float", "char", "if", "else", "return", "while", "void"};
int keywordCount = sizeof(keywords) / sizeof(keywords[0]);

int isKeyword(const char *word) {
    for (int i = 0; i < keywordCount; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

void addToSymbolTable(const char *id) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i], id) == 0) return;
    }
    strcpy(symbolTable[symbolCount++], id);
}

void lexicalAnalyzer(FILE *fp) {
    int state = 0;
    char ch;
    char buffer[MAX_LEN];
    int index = 0;

    while ((ch = fgetc(fp)) != EOF) {
        switch (state) {
            case 0:
                if (isspace(ch)) {
                    state = 0;  // Stay
                } else if (isalpha(ch) || ch == '_') {
                    buffer[0] = ch;
                    index = 1;
                    state = 1;  // Identifier or keyword
                } else if (isdigit(ch)) {
                    buffer[0] = ch;
                    index = 1;
                    state = 2;  // Number
                } else if (ch == '/' && fp->_IO_read_ptr[0] == '/') {
                    while ((ch = fgetc(fp)) != '\n' && ch != EOF);  // Skip comment
                    state = 0;
                } else if (ch == '/' && fp->_IO_read_ptr[0] == '*') {
                    fgetc(fp);
                    while ((ch = fgetc(fp)) != EOF) {
                        if (ch == '*' && fgetc(fp) == '/') break;
                    }
                    state = 0;
                } else if (ch == '<' || ch == '>' || ch == '=' || ch == '!') {
                    buffer[0] = ch;
                    index = 1;
                    state = 3;  // Relational op
                } else if (ch == '+' || ch == '-' || ch == '*' || ch == '%' || ch == '/') {
                    printf("%c -> arithmetic operator\n", ch);
                    state = 0;
                } else if (ch == ';' || ch == ',' || ch == '(' || ch == ')') {
                    printf("%c -> punctuation\n", ch);
                    state = 0;
                } else {
                    printf("%c -> unknown\n", ch);
                    state = 0;
                }
                break;

            case 1:  // Identifier / keyword
                if (isalnum(ch) || ch == '_') {
                    buffer[index++] = ch;
                } else {
                    buffer[index] = '\0';
                    ungetc(ch, fp);
                    if (isKeyword(buffer)) {
                        printf("%s -> keyword\n", buffer);
                    } else {
                        printf("%s -> id\n", buffer);
                        addToSymbolTable(buffer);
                    }
                    state = 0;
                }
                break;

            case 2:  // Number
                if (isdigit(ch)) {
                    buffer[index++] = ch;
                } else {
                    buffer[index] = '\0';
                    ungetc(ch, fp);
                    printf("%s -> number\n", buffer);
                    state = 0;
                }
                break;

            case 3:  // Relational op
                if (ch == '=') {
                    buffer[index++] = ch;
                    buffer[index] = '\0';
                    printf("%s -> relational operator\n", buffer);
                } else {
                    buffer[index] = '\0';
                    ungetc(ch, fp);
                    printf("%s -> relational operator\n", buffer);
                }
                state = 0;
                break;

            default:
                printf("Invalid state\n");
                return;
        }
    }

    printf("\nSymbol Table:\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("%s\n", symbolTable[i]);
    }
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        printf("Failed to open input.txt\n");
        return 1;
    }

    lexicalAnalyzer(fp);
    fclose(fp);
    return 0;
}


