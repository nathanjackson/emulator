//
// Created by nathan on 11/5/22.
//

#include "x86_semantics_test.h"

extern "C"
{
void x86_insn_cbw(struct x86_register_file*);
}

TEST_F(x86_semantics_test, CBW)
{
    AL(register_file) = 0x40;
    AH(register_file) = 0xAA;
    x86_insn_cbw(register_file);
    EXPECT_EQ(AX(register_file), 0x0040);

    AL(register_file) = 0x80;
    AH(register_file) = 0x00;
    x86_insn_cbw(register_file);
    EXPECT_EQ(AX(register_file), 0xFF80);
}