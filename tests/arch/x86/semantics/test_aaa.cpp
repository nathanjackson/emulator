//
// Created by nathan on 11/5/22.
//

#include "x86_semantics_test.h"
#include "arch/x86/semantics/register_file.h"

extern "C"
{
void x86_insn_aaa(struct x86_register_file*);
}

TEST_F(x86_semantics_test, AAA)
{
    AX(register_file) = 15;
    x86_insn_aaa(register_file);
    EXPECT_EQ(AH(register_file), 0x01);
    EXPECT_EQ(AL(register_file), 0x05);
    EXPECT_TRUE(AF(register_file));
    EXPECT_TRUE(CF(register_file));

    AF(register_file) = 0;
    AX(register_file) = 0;
    x86_insn_aaa(register_file);
    EXPECT_EQ(AX(register_file), 0x0);
    EXPECT_FALSE(AF(register_file));
    EXPECT_FALSE(CF(register_file));
}