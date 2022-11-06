//
// Created by nathan on 11/5/22.
//

#include "x86_semantics_test.h"

extern "C"
{
void x86_insn_aas(struct x86_register_file*);
}

TEST_F(x86_semantics_test, AAS)
{
    AH(register_file) = 0x02;
    AL(register_file) = 0xFF;
    x86_insn_aas(register_file);
    EXPECT_EQ(AH(register_file), 0x01);
    EXPECT_EQ(AL(register_file), 0x09);
    EXPECT_TRUE(AF(register_file));
    EXPECT_TRUE(CF(register_file));

    AH(register_file) = 0x02;
    AL(register_file) = 0x00;
    AF(register_file) = 0;
    CF(register_file) = 1;
    x86_insn_aas(register_file);
    EXPECT_EQ(AH(register_file), 0x02);
    EXPECT_EQ(AL(register_file), 0x00);
    EXPECT_FALSE(AF(register_file));
    EXPECT_FALSE(CF(register_file));
}
