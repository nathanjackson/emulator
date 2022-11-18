//
// Created by nathan on 11/17/22.
//

#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/register_operand.h"
#include "arch/x86/semantics/memory_operand.h"

void x86_insn_lds(struct x86_register_file*, struct operand* dst, struct operand* src);
}

TEST_F(x86_semantics_test, LDS)
{
    struct register_operand reg;
    struct memory_operand memop;
    struct operand* dst = make_register_operand(&reg, register_file, X86_REG_AX);
    struct operand* src = make_memory_operand_direct(&memop, mem, register_file, 4, X86_REG_DS, 0x0);

    DS(register_file) = 0x0;
    word val1 = 0x1234;
    word val2 = 0x5678;
    memory_write(mem, DS(register_file), 0x0, 2, (byte*)&val1);
    memory_write(mem, DS(register_file), 0x2, 2, (byte*)&val2);
    x86_insn_lds(register_file, dst, src);
    EXPECT_EQ(AX(register_file), 0x1234);
    EXPECT_EQ(DS(register_file), 0x5678);
}