//
// Created by nathan on 11/5/22.
//

#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/register_operand.h"

void x86_insn_and(struct x86_register_file*, struct operand*, struct operand*);
}

TEST_F(x86_semantics_test, AND_8bit)
{
    struct register_operand reg1, reg2;
    struct operand* dest = make_register_operand(&reg1, register_file, X86_REG_CL);
    struct operand* src = make_register_operand(&reg2, register_file, X86_REG_DL);

    CL(register_file) = 0xAA;
    DL(register_file) = 0x0F;
    CF(register_file) = 1;
    OF(register_file) = 1;
    PF(register_file) = 0;
    SF(register_file) = 1;
    ZF(register_file) = 1;
    x86_insn_and(register_file, dest, src);
    EXPECT_EQ(CL(register_file), 0x0A);
    EXPECT_FALSE(CF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_TRUE(PF(register_file));
    EXPECT_FALSE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
}

TEST_F(x86_semantics_test, AND_16bit)
{
    struct register_operand reg1, reg2;
    struct operand* dest = make_register_operand(&reg1, register_file, X86_REG_AX);
    struct operand* src = make_register_operand(&reg2, register_file, X86_REG_CX);

    AX(register_file) = 0xABCD;
    CX(register_file) = 0x0FF0;
    CF(register_file) = 1;
    OF(register_file) = 1;
    PF(register_file) = 0;
    SF(register_file) = 1;
    ZF(register_file) = 1;
    x86_insn_and(register_file, dest, src);
    EXPECT_EQ(AX(register_file), 0xBC0);
    EXPECT_FALSE(CF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_TRUE(PF(register_file));
    EXPECT_FALSE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
}