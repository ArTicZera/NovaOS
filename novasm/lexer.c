/*
    Lexer
*/

#include "../Include/stdint.h"
#include "../minilibc/ctype.h"
#include "../Memory/mem.h"
#include "../Font/text.h"

#include "lexer.h"

void LexerInit(Lexer* lexer, char* text)
{
    lexer->source = text;
    lexer->position = 0;
    lexer->current = text[0];
}

void Advance(Lexer* lexer)
{
    lexer->position++;
    lexer->current = lexer->source[lexer->position];
}

void SkipWhitespace(Lexer* lexer)
{
    while (lexer->current == ' ' || lexer->current == '\t' || lexer->current == '\r')
    {
        Advance(lexer);
    }
}

Token ReadIdentifier(Lexer* lexer)
{
    Token t;

    memset(&t, 0, sizeof(Token));

    int i = 0;

    while(isalpha(lexer->current) || isdigit(lexer->current) || lexer->current=='_')
    {
        t.lexeme[i++] = lexer->current;

        Advance(lexer);
    }

    t.lexeme[i] = 0;
    t.type = TOKEN_IDENTIFIER;

    if(strcmp(t.lexeme,"eax")==0) t.type = TOKEN_REGISTER;
    else if(strcmp(t.lexeme,"ebx")==0) t.type = TOKEN_REGISTER;
    else if(strcmp(t.lexeme,"ecx")==0) t.type = TOKEN_REGISTER;
    else if(strcmp(t.lexeme,"edx")==0) t.type = TOKEN_REGISTER;
    else if(strcmp(t.lexeme,"esi")==0) t.type = TOKEN_REGISTER;
    else if(strcmp(t.lexeme,"edi")==0) t.type = TOKEN_REGISTER;
    else if(strcmp(t.lexeme,"esp")==0) t.type = TOKEN_REGISTER;
    else if(strcmp(t.lexeme,"ebp")==0) t.type = TOKEN_REGISTER;

    return t;
}

Token ReadNumber(Lexer *lexer)
{
    Token t;

memset(&t, 0, sizeof(Token));
    t.type = TOKEN_NUMBER;
    t.num = 0;

    if (lexer->current == '0')
    {
        Advance(lexer);

        if (lexer->current == 'x' || lexer->current == 'X')
        {
            Advance(lexer);

            while (1)
            {
                char c = lexer->current;
                int digit;

                if (c >= '0' && c <= '9')
                    digit = c - '0';
                else if (c >= 'a' && c <= 'f')
                    digit = c - 'a' + 10;
                else if (c >= 'A' && c <= 'F')
                    digit = c - 'A' + 10;
                else
                    break;

                t.num = t.num * 16 + digit;
                Advance(lexer);
            }

            return t;
        }

        // Era apenas o número 0
        t.num = 0;
        return t;
    }

    // Decimal
    while (lexer->current >= '0' && lexer->current <= '9')
    {
        t.num = t.num * 10 + (lexer->current - '0');
        Advance(lexer);
    }

    return t;
}

Token NextToken(Lexer *lexer)
{
    Token t;

    memset(&t, 0, sizeof(Token));

    while (1)
    {
        SkipWhitespace(lexer);

        if (lexer->current == '\0')
        {
            t.type = TOKEN_EOF;
            return t;
        }

        if (lexer->current == '\n')
        {
            Advance(lexer);
            t.type = TOKEN_NEWLINE;
            return t;
        }

        if (lexer->current == '\'')
            return ReadString(lexer);

        if (isalpha(lexer->current) || lexer->current == '_')
            return ReadIdentifier(lexer);

        if (isdigit(lexer->current))
            return ReadNumber(lexer);

        /*
        Print("LEXER: ", 0xFFFFFF00);
        Print(t.lexeme, 0xFFFFFFFF);
        Print(" TYPE=", 0xFFFFFFFF);
        PrintInt(t.type, 0xFFFFFFFF);
        Print("\n", 0xFFFFFFFF);
        */

        switch (lexer->current)
        {
            case ',':
                Advance(lexer);
                t.type = TOKEN_COMMA;
                return t;

            case ':':
                Advance(lexer);
                t.type = TOKEN_COLON;
                return t;

            case '[':
                Advance(lexer);
                t.type = TOKEN_LBRACKET;
                return t;

            case ']':
                Advance(lexer);
                t.type = TOKEN_RBRACKET;
                return t;

            case '+':
                Advance(lexer);
                t.type = TOKEN_PLUS;
                return t;

            case '-':
                Advance(lexer);
                t.type = TOKEN_MINUS;
                return t;

            case ';':
                while (lexer->current != '\n' &&
                       lexer->current != '\0')
                {
                    Advance(lexer);
                }
                break;

            default:
                Advance(lexer);
                break;
        }
    }
}

Token ReadString(Lexer *lexer)
{
    Token t;

    memset(&t,0,sizeof(Token));

    Advance(lexer);

    int i=0;

    while(lexer->current!='\'' && lexer->current!='\0')
    {
        t.lexeme[i++]=lexer->current;
        Advance(lexer);
    }

    t.lexeme[i]=0;

    if(lexer->current=='\'')
        Advance(lexer);

    if(i==1)
    {
        t.type=TOKEN_NUMBER;
        t.num=(unsigned char)t.lexeme[0];
    }
    else
    {
        t.type=TOKEN_STRING;
    }

    return t;
}