//
// Created by nathan on 11/17/22.
//
#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/register_operand.h"
void x86_insn_push(struct x86_register_file*, struct memory*, struct operand*);
}

TEST_F(x86_semantics_test, PUSH)
{
    struct register_operand reg;
    struct operand* op = make_register_operand(&reg, register_file, X86_REG_AX);

    AX(register_file) = 0xDEAD;
    SS(register_file) = 0x0;
    SP(register_file) = 0xFFFF;
    x86_insn_push(register_file, mem, op);
    auto ptr = reinterpret_cast<word*>(get_host_ptr(mem, SS(register_file), SP(register_file)));
    EXPECT_EQ(*ptr, 0xDEAD);
    EXPECT_EQ(SP(register_file), 0xFFFD);
}