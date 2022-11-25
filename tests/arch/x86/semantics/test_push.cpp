//
// Created by nathan on 11/17/22.
//
#include "x86_semantics_test.h"

#include "address_space.h"

extern "C"
{
#include "arch/x86/semantics/register_operand.h"
void x86_insn_push(struct x86_register_file*, struct address_space*, struct operand*);
}

TEST_F(x86_semantics_test, PUSH)
{
    struct register_operand reg;
    struct operand* op = make_register_operand(&reg, register_file, X86_REG_AX);

    AX(register_file) = 0xDEAD;
    SS(register_file) = 0x0;
    SP(register_file) = 0xFFFF;
    x86_insn_push(register_file, &as, op);
    word got_val = 0x0;
    address_space_access_segmented(&as, 0, SS(register_file), SP(register_file), sizeof(got_val), reinterpret_cast<byte*>(&got_val));
    EXPECT_EQ(got_val, 0xDEAD);
    EXPECT_EQ(SP(register_file), 0xFFFD);
}