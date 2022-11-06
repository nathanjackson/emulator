//
// Created by nathan on 11/5/22.
//

#include "x86_semantics_test.h"

extern "C"
{
void x86_insn_clc(struct x86_register_file*);
}

TEST_F(x86_semantics_test, CLC)
{
    CF(register_file) = 1;
    x86_insn_clc(register_file);
    EXPECT_FALSE(CF(register_file));
}
