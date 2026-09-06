#ifndef LEXER_H
#define LEXER_H

typedef enum
{
    TOKEN_EOF,
    TOKEN_REGISTER,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_COLON,
    TOKEN_COMMA,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_NEWLINE,
    TOKEN_STAR,
    TOKEN_SLASH
} TokenType;

typedef struct
{
    TokenType type;
    char lexeme[64];
    int num;
} Token;

typedef struct
{
    char* source;
    int position;
    char current;
} Lexer;

void LexerInit(Lexer* lexer, char* text);
void Advance(Lexer* lexer);
void SkipWhitespace(Lexer* lexer);

Token ReadIdentifier(Lexer* lexer);
Token ReadNumber(Lexer *lexer);
Token NextToken(Lexer *lexer);
Token ReadString(Lexer *lexer);

#endif