/*
    Assembler
*/

#include "assembler.h"
#include "lexer.h"
#include "parser.h"
#include "encoder.h"
#include "symbols.h"

extern int outPos;
extern int symbolCount;
int assemblerPass;

void Assemble(char* source)
{
    Lexer lexer;
    Parser parser;

    /* Primeira passada */
    assemblerPass = 1;
    outPos = 0;
    symbolCount = 0;

    LexerInit(&lexer, source);
    ParserInit(&parser, &lexer);
    ParserFunc(&parser);

    /* Segunda passada */
    assemblerPass = 2;
    outPos = 0;

    LexerInit(&lexer, source);
    ParserInit(&parser, &lexer);
    ParserFunc(&parser);
}