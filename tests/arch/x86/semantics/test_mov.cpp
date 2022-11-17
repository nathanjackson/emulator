//
// Created by nathan on 11/16/22.
//

#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/register_operand.h"
#include "arch/x86/semantics/immediate_operand.h"
void x86_insn_mov(struct operand* dest, struct operand* src);
}

TEST_F(x86_semantics_test, MOV_8bit)
{
    struct immediate_operand imm;
    struct register_operand reg;

    struct operand* dest = make_register_operand(&reg, register_file, X86_REG_AH);
    struct operand* src = make_u8_immediate(&imm, 20);

    AH(register_file) = 0xFF;
    x86_insn_mov(dest, src);
    EXPECT_EQ(AH(register_file), 20);
}

TEST_F(x86_semantics_test, MOV_16bit)
{
    struct immediate_operand imm;
    struct register_operand reg;

    struct operand* dest = make_register_operand(&reg, register_file, X86_REG_AX);
    struct operand* src = make_u16_immediate(&imm, 3);

    AH(register_file) = 0xFFFF;
    x86_insn_mov(dest, src);
    EXPECT_EQ(AX(register_file), 3);
}