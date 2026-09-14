#ifndef ENCODE_H
#define ENCODE_H

typedef enum
{
    OPERAND_NONE,
    OPERAND_REGISTER,
    OPERAND_IMMEDIATE,
    OPERAND_LABEL,
    OPERAND_MEMORY
} OperandType;

typedef enum
{
    REG_EAX,
    REG_ECX,
    REG_EDX,
    REG_EBX,
    REG_ESP,
    REG_EBP,
    REG_ESI,
    REG_EDI
} Register;

typedef struct
{
    OperandType type;

    union
    {
        Register reg;

        unsigned int imm;

        char label[64];

        struct
        {
            Register base;
            int offset;
        } mem;
    };

} Operand;

typedef enum
{
    OP_MOV,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_PUSH,
    OP_POP,
    OP_CALL,
    OP_JMP,
    OP_RET,
    OP_CMP,
    OP_JE,
    OP_JNE,
    OP_INT
} Opcode;

typedef struct
{
    Opcode opcode;
    int operandCount;
    Operand operands[2];
} Instruction;

extern unsigned char output[65536];
extern int outPos;

void OutputByte(unsigned char b);
void OutputWord(unsigned short w);
void OutputDword(unsigned int d);

void EncodeInstruction(Instruction* inst);

#endif