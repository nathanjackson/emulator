//
// Created by nathan on 11/13/22.
//

#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/immediate_operand.h"
#include "arch/x86/semantics/register_operand.h"
#include "arch/x86/semantics/memory_operand.h"
void x86_insn_jmp(struct x86_register_file*, struct operand* tgt);
}

TEST_F(x86_semantics_test, JMP_Relative)
{
    struct immediate_operand imm1;
    struct operand* tgt = make_u16_immediate(&imm1, 0x5);

    IP(register_file) = 0x0;
    x86_insn_jmp(register_file, tgt);
    EXPECT_EQ(IP(register_file), 0x5);
}

TEST_F(x86_semantics_test, JMP_Indirect_Reg)
{
    struct register_operand reg1;
    struct operand* tgt = make_register_operand(&reg1, register_file, X86_REG_AX);

    IP(register_file) = 0x0;
    AX(register_file) = 0x33;
    x86_insn_jmp(register_file, tgt);
    EXPECT_EQ(IP(register_file), 0x33);

}

TEST_F(x86_semantics_test, JMP_Indirect_Mem)
{
    struct memory_operand mem1;
    struct operand* tgt = make_memory_operand_direct(&mem1, mem, register_file, 2, X86_REG_CS, 0x0110);

    CS(register_file) = 0x0;
    IP(register_file) = 0x0;
    WORD(tgt) = 0xFACE;
    x86_insn_jmp(register_file, tgt);
    EXPECT_EQ(IP(register_file), 0xFACE);
}