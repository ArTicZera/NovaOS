#ifndef PARSER_H
#define PARSER_H

#include "../Include/stdint.h"
#include "encoder.h"

typedef struct
{
    Lexer* lexer;
    Token current;
    DWORD bits;
    DWORD org;
} Parser;

int Match(Parser* parser, TokenType type);

void ParserInit(Parser* parser, Lexer* lexer);
void ParserAdvance(Parser* parser);
void ParserFunc(Parser *parser);
void ParseExpression(Parser* parser);
void ParseStatement(Parser *parser);
void ParseInstruction(Parser* parser, Token mnemonic);

Operand ParseOperand(Parser* parser);
Register ParseRegister(char* name);

#endif