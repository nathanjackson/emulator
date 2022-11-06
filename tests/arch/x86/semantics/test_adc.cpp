//
// Created by nathan on 11/5/22.
//

#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/immediate_operand.h"
#include "arch/x86/semantics/register_operand.h"
void x86_insn_adc(struct x86_register_file* register_file, struct operand* dest, struct operand* src);
}

TEST_F(x86_semantics_test, ADC_8bit)
{
    struct immediate_operand imm;
    struct register_operand reg;

    // ADC AH, 0x01
    struct operand* dest = make_register_operand(&reg, register_file, X86_REG_AH);
    struct operand* src = make_u8_immediate(&imm, 1);

    AH(register_file) = 5;
    CF(register_file) = 1;
    ZF(register_file) = 1;
    PF(register_file) = 1;
    x86_insn_adc(register_file, dest, src);
    EXPECT_EQ(AH(register_file), 7);
    EXPECT_FALSE(CF(register_file));
    EXPECT_FALSE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_FALSE(PF(register_file));

    AH(register_file) = 5;
    CF(register_file) = 0;
    ZF(register_file) = 1;
    PF(register_file) = 1;
    x86_insn_adc(register_file, dest, src);
    EXPECT_EQ(AH(register_file), 6);
    EXPECT_FALSE(CF(register_file));
    EXPECT_FALSE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_TRUE(PF(register_file));

    AH(register_file) = UINT8_MAX;
    CF(register_file) = 0;
    ZF(register_file) = 0;
    PF(register_file) = 0;
    x86_insn_adc(register_file, dest, src);
    EXPECT_EQ(AH(register_file), 0);
    EXPECT_TRUE(CF(register_file));
    EXPECT_FALSE(SF(register_file));
    EXPECT_TRUE(ZF(register_file));
    EXPECT_TRUE(PF(register_file));

    AH(register_file) = 127;
    CF(register_file) = 0;
    OF(register_file) = 0;
    SF(register_file) = 0;
    ZF(register_file) = 1;
    PF(register_file) = 1;
    x86_insn_adc(register_file, dest, src);
    EXPECT_EQ(AH(register_file), 128);
    EXPECT_TRUE(OF(register_file));
    EXPECT_TRUE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_FALSE(PF(register_file));

    AH(register_file) = 0x0F;
    AF(register_file) = 0;
    CF(register_file) = 0;
    OF(register_file) = 0;
    SF(register_file) = 0;
    ZF(register_file) = 1;
    x86_insn_adc(register_file, dest, src);
    EXPECT_TRUE(AF(register_file));

    AH(register_file) = 0x0E;
    AF(register_file) = 1;
    CF(register_file) = 0;
    OF(register_file) = 0;
    SF(register_file) = 0;
    ZF(register_file) = 1;
    PF(register_file) = 0;
    x86_insn_adc(register_file, dest, src);
    EXPECT_FALSE(AF(register_file));
    EXPECT_TRUE(PF(register_file));
}

TEST_F(x86_semantics_test, ADC_16bit)
{
    struct immediate_operand imm;
    struct register_operand reg;

    // ADC AX, 0x01
    struct operand* dest = make_register_operand(&reg, register_file, X86_REG_AX);
    struct operand* src = make_u16_immediate(&imm, 1);

    AX(register_file) = 5;
    CF(register_file) = 1;
    SF(register_file) = 1;
    ZF(register_file) = 1;
    PF(register_file) = 1;
    x86_insn_adc(register_file, dest, src);
    EXPECT_EQ(AX(register_file), 7);
    EXPECT_FALSE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_FALSE(CF(register_file));
    EXPECT_FALSE(PF(register_file));

    AX(register_file) = 5;
    CF(register_file) = 0;
    SF(register_file) = 1;
    ZF(register_file) = 1;
    PF(register_file) = 0;
    x86_insn_adc(register_file, dest, src);
    EXPECT_EQ(AX(register_file), 6);
    EXPECT_FALSE(CF(register_file));
    EXPECT_FALSE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_TRUE(PF(register_file));

    AX(register_file) = UINT16_MAX;
    CF(register_file) = 0;
    SF(register_file) = 1;
    ZF(register_file) = 0;
    PF(register_file) = 0;
    x86_insn_adc(register_file, dest, src);
    EXPECT_EQ(AX(register_file), 0);
    EXPECT_TRUE(CF(register_file));
    EXPECT_FALSE(SF(register_file));
    EXPECT_TRUE(ZF(register_file));
    EXPECT_TRUE(PF(register_file));

    AX(register_file) = 32767;
    CF(register_file) = 0;
    OF(register_file) = 0;
    SF(register_file) = 0;
    ZF(register_file) = 1;
    PF(register_file) = 1;
    x86_insn_adc(register_file, dest, src);
    EXPECT_EQ(AX(register_file), 32768);
    EXPECT_TRUE(OF(register_file));
    EXPECT_TRUE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_TRUE(PF(register_file));

    AX(register_file) = 0x0F;
    AF(register_file) = 0;
    CF(register_file) = 0;
    OF(register_file) = 0;
    SF(register_file) = 0;
    ZF(register_file) = 1;
    x86_insn_adc(register_file, dest, src);
    EXPECT_TRUE(AF(register_file));

    AX(register_file) = 0x0E;
    AF(register_file) = 1;
    CF(register_file) = 0;
    OF(register_file) = 0;
    SF(register_file) = 0;
    ZF(register_file) = 1;
    x86_insn_adc(register_file, dest, src);
    EXPECT_FALSE(AF(register_file));
}