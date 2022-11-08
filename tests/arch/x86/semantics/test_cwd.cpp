//
// Created by nathan on 11/5/22.
//

#include "x86_semantics_test.h"

extern "C"
{
void x86_insn_cwd(struct x86_register_file*);
}

TEST_F(x86_semantics_test, CWD)
{
    AX(register_file) = 0x8000;
    x86_insn_cwd(register_file);
    EXPECT_EQ(DX(register_file), 0xFFFF);

    AX(register_file) = 0x4000;
    DX(register_file) = 0x0;
    x86_insn_cwd(register_file);
    EXPECT_EQ(DX(register_file), 0x0);
}