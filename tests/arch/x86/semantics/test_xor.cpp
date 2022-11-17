//
// Created by nathan on 11/16/22.
//

#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/register_operand.h"
#include "arch/x86/semantics/immediate_operand.h"
void x86_insn_xor(struct x86_register_file*, struct operand* lhs, struct operand* rhs);
}

TEST_F(x86_semantics_test, XOR_8bit)
{
    struct immediate_operand imm;
    struct register_operand reg;

    struct operand* rhs = make_u8_immediate(&imm, 0xAA);
    struct operand* lhs = make_register_operand(&reg, register_file, X86_REG_AH);

    AH(register_file) = 0x55;
    OF(register_file) = 1;
    CF(register_file) = 1;
    x86_insn_xor(register_file, lhs, rhs);
    EXPECT_EQ(AH(register_file), 0xFF);
    EXPECT_TRUE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_TRUE(PF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_FALSE(CF(register_file));

    AH(register_file) = 0xAA;
    x86_insn_xor(register_file, lhs, rhs);
    EXPECT_EQ(AH(register_file), 0x0);
    EXPECT_FALSE(SF(register_file));
    EXPECT_TRUE(ZF(register_file));
    EXPECT_TRUE(PF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_FALSE(CF(register_file));
}

TEST_F(x86_semantics_test, XOR_16bit)
{
    struct immediate_operand imm;
    struct register_operand reg;

    struct operand* rhs = make_u16_immediate(&imm, 0xAAAA);
    struct operand* lhs = make_register_operand(&reg, register_file, X86_REG_AX);

    AX(register_file) = 0x5555;
    OF(register_file) = 1;
    CF(register_file) = 1;
    x86_insn_xor(register_file, lhs, rhs);
    EXPECT_EQ(AX(register_file), 0xFFFF);
    EXPECT_TRUE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_TRUE(PF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_FALSE(CF(register_file));

    AX(register_file) = 0xAAAA;
    x86_insn_xor(register_file, lhs, rhs);
    EXPECT_EQ(AX(register_file), 0x0);
    EXPECT_FALSE(SF(register_file));
    EXPECT_TRUE(ZF(register_file));
    EXPECT_TRUE(PF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_FALSE(CF(register_file));
}