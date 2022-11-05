//
// Created by nathan on 11/5/22.
//


#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/immediate_operand.h"
void x86_insn_aad(struct x86_register_file*, struct operand*);
}

TEST_F(x86_semantics_test, AAD)
{
    struct immediate_operand imm;
    struct operand* base = make_u8_immediate(&imm, 0xA);
    AX(register_file) = 0x105;
    x86_insn_aad(register_file, base);
    EXPECT_EQ(AH(register_file), 0x0);
    EXPECT_EQ(AL(register_file), 0xF);
}