//
// Created by nathan on 11/5/22.
//
#include "x86_semantics_test.h"

extern "C"
{
void x86_insn_cmc(struct x86_register_file*);
}

TEST_F(x86_semantics_test, CMC)
{
    CF(register_file) = 1;
    x86_insn_cmc(register_file);
    EXPECT_FALSE(CF(register_file));

    CF(register_file) = 0;
    x86_insn_cmc(register_file);
    EXPECT_TRUE(CF(register_file));
}