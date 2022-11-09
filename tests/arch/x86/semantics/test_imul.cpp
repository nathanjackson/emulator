//
// Created by nathan on 11/8/22.
//

#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/register_operand.h"
void x86_insn_imul(struct x86_register_file*, struct operand*);
}

TEST_F(x86_semantics_test, IMUL_8bit)
{
    struct register_operand regop;
    struct operand* op = make_register_operand(&regop, register_file, X86_REG_BL);

    AL(register_file) = -2;
    BL(register_file) = -4;
    CF(register_file) = 1;
    OF(register_file) = 1;
    x86_insn_imul(register_file, op);
    EXPECT_FALSE(CF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_EQ(AX(register_file), 8);
}

TEST_F(x86_semantics_test, IMUL_16bit)
{
    struct register_operand regop;
    struct operand* op = make_register_operand(&regop, register_file, X86_REG_BX);

    AX(register_file) = -2;
    BX(register_file) = -4;
    CF(register_file) = 1;
    OF(register_file) = 1;
    x86_insn_imul(register_file, op);
    EXPECT_FALSE(CF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_EQ(DX(register_file) | AX(register_file), 8);
}
