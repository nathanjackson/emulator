//
// Created by nathan on 11/5/22.
//

#include "x86_semantics_test.h"

extern "C"
{
void x86_insn_cld(struct x86_register_file*);
}

TEST_F(x86_semantics_test, CLD)
{
    DF(register_file) = 1;
    x86_insn_cld(register_file);
    EXPECT_FALSE(DF(register_file));
}