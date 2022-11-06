//
// Created by nathan on 11/5/22.
//
#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/register_operand.h"
void x86_insn_cmp(struct x86_register_file*, struct operand*, struct operand*);
}

TEST_F(x86_semantics_test, CMP)
{
    struct register_operand reg0, reg1;
    struct operand* lhs = make_register_operand(&reg0, register_file, X86_REG_AX);
    struct operand* rhs = make_register_operand(&reg1, register_file, X86_REG_BX);

    AX(register_file) = 3;
    BX(register_file) = 3;
    CF(register_file) = 1;
    SF(register_file) = 1;
    ZF(register_file) = 0;
    AF(register_file) = 1;
    OF(register_file) = 1;
    PF(register_file) = 0;
    x86_insn_cmp(register_file, lhs, rhs);
    EXPECT_FALSE(CF(register_file));
    EXPECT_FALSE(SF(register_file));
    EXPECT_TRUE(ZF(register_file));
    EXPECT_FALSE(AF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_TRUE(PF(register_file));

    AX(register_file) = 5;
    BX(register_file) = 7;
    CF(register_file) = 1;
    SF(register_file) = 0;
    ZF(register_file) = 1;
    AF(register_file) = 1;
    PF(register_file) = 1;
    x86_insn_cmp(register_file, lhs, rhs);
    EXPECT_TRUE(CF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_TRUE(AF(register_file));
    EXPECT_FALSE(PF(register_file));

    AX(register_file) = 7;
    BX(register_file) = 5;
    CF(register_file) = 1;
    SF(register_file) = 1;
    ZF(register_file) = 1;
    AF(register_file) = 1;
    PF(register_file) = 1;
    x86_insn_cmp(register_file, lhs, rhs);
    EXPECT_FALSE(CF(register_file));
    EXPECT_FALSE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_FALSE(AF(register_file));
    EXPECT_FALSE(PF(register_file));
}