/*
    Encoder
*/

#include "../Memory/mem.h"
#include "../Include/stdint.h"
#include "../Font/text.h"
#include "encoder.h"
#include "symbols.h"

unsigned char output[65536];

extern int assemblerPass;
int outPos = 0;

void OutputByte(unsigned char b)
{
    if (assemblerPass == 2)
        output[outPos] = b;

    outPos++;
}

void OutputWord(unsigned short w)
{
    OutputByte(w & 0xFF);
    OutputByte(w >> 8);
}

void OutputDword(unsigned int d)
{
    OutputByte(d);
    OutputByte(d >> 8);
    OutputByte(d >> 16);
    OutputByte(d >> 24);
}

void EncodeInstruction(Instruction* inst)
{
    switch(inst->opcode)
    {
        case OP_MOV:
            if (inst->operandCount == 2 && inst->operands[0].type == OPERAND_REGISTER)
            {
                OutputByte(0xB8 + inst->operands[0].reg);

                switch(inst->operands[1].type)
                {
                    case OPERAND_IMMEDIATE:

                        OutputDword(inst->operands[1].imm);
                        break;

                    case OPERAND_LABEL:
                    {
                        Symbol* sym = FindSymbol(inst->operands[1].label);

                        if(sym == NULL)
                        {
                            Print("Undefined label\n",0xFFFF0000);
                            OutputDword(0);
                        }
                        else
                        {
                            OutputDword(sym->addr);
                        }

                        break;
                    }

                    default:
                        OutputDword(0);
                        break;
                }
            }
            break;

        case OP_INT:
            if (inst->operandCount == 1 && inst->operands[0].type == OPERAND_IMMEDIATE)
            {
                OutputByte(0xCD);
                OutputByte(inst->operands[0].imm);
            }
            break;

        case OP_RET:
            OutputByte(0xC3);
            break;
    }
}