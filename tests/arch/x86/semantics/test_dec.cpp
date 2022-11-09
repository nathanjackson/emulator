//
// Created by nathan on 11/8/22.
//

#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/register_operand.h"
void x86_insn_dec(struct x86_register_file*, struct operand*);
}

TEST_F(x86_semantics_test, DEC_8bit)
{
    struct register_operand regop;
    struct operand* op = make_register_operand(&regop, register_file, X86_REG_AH);

    AH(register_file) = 1;
    SF(register_file) = 1;
    ZF(register_file) = 0;
    AF(register_file) = 1;
    OF(register_file) = 1;
    PF(register_file) = 0;
    x86_insn_dec(register_file, op);
    EXPECT_EQ(AH(register_file), 0);
    EXPECT_FALSE(SF(register_file));
    EXPECT_TRUE(ZF(register_file));
    EXPECT_FALSE(AF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_TRUE(PF(register_file));

    AH(register_file) = 0xFF;
    SF(register_file) = 0;
    ZF(register_file) = 1;
    AF(register_file) = 1;
    OF(register_file) = 1;
    PF(register_file) = 1;
    x86_insn_dec(register_file, op);
    EXPECT_EQ(AH(register_file), 0xFE);
    EXPECT_TRUE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_FALSE(AF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_FALSE(PF(register_file));

    AH(register_file) = 0x0;
    SF(register_file) = 0;
    ZF(register_file) = 1;
    AF(register_file) = 1;
    OF(register_file) = 0;
    PF(register_file) = 1;
    x86_insn_dec(register_file, op);
    EXPECT_EQ(AH(register_file), 0xFF);
    EXPECT_TRUE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_TRUE(AF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_TRUE(PF(register_file));
}

TEST_F(x86_semantics_test, DEC_16bit)
{
    struct register_operand regop;
    struct operand* op = make_register_operand(&regop, register_file, X86_REG_AX);

    AX(register_file) = 1;
    SF(register_file) = 1;
    ZF(register_file) = 0;
    AF(register_file) = 1;
    OF(register_file) = 1;
    PF(register_file) = 0;
    x86_insn_dec(register_file, op);
    EXPECT_EQ(AX(register_file), 0);
    EXPECT_FALSE(SF(register_file));
    EXPECT_TRUE(ZF(register_file));
    EXPECT_FALSE(AF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_TRUE(PF(register_file));
}