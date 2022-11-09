//
// Created by nathan on 11/8/22.
//

#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/register_file.h"
void x86_insn_das(struct x86_register_file*);
};

TEST_F(x86_semantics_test, DAS)
{
    AL(register_file) = 0xFF;
    x86_insn_das(register_file);
    EXPECT_EQ(AL(register_file), 0x99);
    EXPECT_TRUE(CF(register_file));
}