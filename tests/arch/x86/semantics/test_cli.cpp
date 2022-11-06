//
// Created by nathan on 11/5/22.
//

#include "x86_semantics_test.h"

extern "C"
{
void x86_insn_cli(struct x86_register_file*);
}

TEST_F(x86_semantics_test, CLI)
{
    IF(register_file) = 1;
    x86_insn_cli(register_file);
    EXPECT_FALSE(IF(register_file));
}