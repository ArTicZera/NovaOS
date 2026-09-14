/*
    Parser
*/

#include "../Include/stdint.h"
#include "../Font/text.h"
#include "../Memory/mem.h"

#include "lexer.h"
#include "encoder.h"
#include "parser.h"
#include "symbols.h"

extern int assemblerPass;

Symbol symbols[256];
int symbolCount = 0;

void ParserInit(Parser* parser, Lexer* lexer)
{
    parser->lexer = lexer;
    parser->current = NextToken(lexer);
    parser->bits = 32;
    parser->org = 0x04000000;
}

void ParserAdvance(Parser* parser)
{
    parser->current = NextToken(parser->lexer);
}

int Match(Parser* parser, TokenType type)
{
    if (parser->current.type == type)
    {
        ParserAdvance(parser);
        return 1;
    }
    return 0;
}

void ParserFunc(Parser* parser)
{
    while (parser->current.type != TOKEN_EOF)
    {
        ParseStatement(parser);
    }
}

void ParseStatement(Parser* parser)
{
    while (Match(parser, TOKEN_NEWLINE));

    if (parser->current.type == TOKEN_EOF)
        return;

    if (parser->current.type != TOKEN_IDENTIFIER)
    {
        Print("Parser Error.\n", 0xFFFF0000);
        ParserAdvance(parser);
        return;
    }

    /* BITS */
    if (strcmp(parser->current.lexeme, "BITS") == 0)
    {
        ParserAdvance(parser);

        if (parser->current.type != TOKEN_NUMBER)
        {
            Print("Expected number after BITS\n",0xFFFF0000);
            return;
        }

        parser->bits = parser->current.num;

        ParserAdvance(parser);
        Match(parser, TOKEN_NEWLINE);
        return;
    }

    Token ident = parser->current;
    ParserAdvance(parser);

    /* Label */
    if (Match(parser, TOKEN_COLON))
    {
        if (assemblerPass == 1)
        {
            if (FindSymbol(ident.lexeme))
            {
                Print("Duplicate label\n", 0xFFFF0000);
                return;
            }

            strcpy(symbols[symbolCount].name, ident.lexeme);
            symbols[symbolCount].addr = parser->org + outPos;
            symbolCount++;
        }
        
        while (Match(parser, TOKEN_NEWLINE));

        if (parser->current.type == TOKEN_EOF)
            return;

        if (parser->current.type != TOKEN_IDENTIFIER)
            return;

        ident = parser->current;
        ParserAdvance(parser);
    }

    /* DB */
    if (strcmp(ident.lexeme, "db") == 0)
    {
        if (parser->current.type == TOKEN_STRING)
        {
            int i = 0;

            while (parser->current.lexeme[i])
                OutputByte(parser->current.lexeme[i++]);
        }
        else if (parser->current.type == TOKEN_NUMBER)
        {
            OutputByte((BYTE)parser->current.num);
        }
        else
        {
            Print("Expected string/number after db\n",0xFFFF0000);
            return;
        }

        ParserAdvance(parser);
        Match(parser, TOKEN_NEWLINE);

        return;
    }

    ParseInstruction(parser, ident);
}

void ParseInstruction(Parser* parser, Token mnemonic)
{
    Instruction inst;

    memset(&inst, 0, sizeof(Instruction));
    //inst.operandCount = 0;

    if (strcmp(mnemonic.lexeme, "mov") == 0)
        inst.opcode = OP_MOV;
    else if (strcmp(mnemonic.lexeme, "add") == 0)
        inst.opcode = OP_ADD;
    else if (strcmp(mnemonic.lexeme, "sub") == 0)
        inst.opcode = OP_SUB;
    else if (strcmp(mnemonic.lexeme, "int") == 0)
        inst.opcode = OP_INT;
    else if (strcmp(mnemonic.lexeme, "ret") == 0)
        inst.opcode = OP_RET;
    else
    {
        Print("\n\n", 0x00);
        Print(mnemonic.lexeme, 0xFFFFFFFF);
        Print("Unknown instruction\n", 0xFFFF0000);
        return;
    }

    if (parser->current.type != TOKEN_NEWLINE &&
        parser->current.type != TOKEN_EOF)
    {
        inst.operands[inst.operandCount++] =
            ParseOperand(parser);

        while (Match(parser, TOKEN_COMMA))
        {
            inst.operands[inst.operandCount++] =
                ParseOperand(parser);
        }
    }

    Match(parser, TOKEN_NEWLINE);

    EncodeInstruction(&inst);
}

Operand ParseOperand(Parser* parser)
{
    Operand op;

    memset(&op, 0, sizeof(op));

    switch (parser->current.type)
    {

        case TOKEN_REGISTER:

            op.type = OPERAND_REGISTER;
            op.reg = ParseRegister(parser->current.lexeme);

            ParserAdvance(parser);
            break;

        case TOKEN_NUMBER:

            op.type = OPERAND_IMMEDIATE;
            op.imm = parser->current.num;

            ParserAdvance(parser);
            break;

        case TOKEN_IDENTIFIER:

            op.type = OPERAND_LABEL;

            strcpy(op.label, parser->current.lexeme);
            ParserAdvance(parser);
            break;

        default:

            Print("Invalid operand\n", 0xFFFF0000);
            break;
    }

    return op;
}

Register ParseRegister(char* name)
{
    if (strcmp(name, "eax") == 0) return REG_EAX;
    if (strcmp(name, "ecx") == 0) return REG_ECX;
    if (strcmp(name, "edx") == 0) return REG_EDX;
    if (strcmp(name, "ebx") == 0) return REG_EBX;
    if (strcmp(name, "esp") == 0) return REG_ESP;
    if (strcmp(name, "ebp") == 0) return REG_EBP;
    if (strcmp(name, "esi") == 0) return REG_ESI;
    if (strcmp(name, "edi") == 0) return REG_EDI;

    return REG_EAX;
}

void ParseExpression(Parser* parser)
{
    ParseOperand(parser);

    while(parser->current.type == TOKEN_PLUS || parser->current.type == TOKEN_MINUS)
    {
        ParserAdvance(parser);
        ParseOperand(parser);
    }
}

