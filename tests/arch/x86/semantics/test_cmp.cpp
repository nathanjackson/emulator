//
// Created by nathan on 11/5/22.
//
#include "x86_semantics_test.h"

#include "address_space.h"

extern "C"
{
#include "arch/x86/semantics/register_operand.h"
void x86_insn_cmp(struct x86_register_file*, struct operand*, struct operand*);
void x86_insn_cmpsb(struct x86_register_file*, struct address_space*);
void x86_insn_cmpsw(struct x86_register_file*, struct address_space*);
}

TEST_F(x86_semantics_test, CMP)
{
    struct register_operand reg0, reg1;
    struct operand* lhs = make_register_operand(&reg0, register_file, X86_REG_AX);
    struct operand* rhs = make_register_operand(&reg1, register_file, X86_REG_BX);

    AX(register_file) = 3;
    BX(register_file) = 3;
    CF(register_file) = 1;
    SF(register_file) = 1;
    ZF(register_file) = 0;
    AF(register_file) = 1;
    OF(register_file) = 1;
    PF(register_file) = 0;
    x86_insn_cmp(register_file, lhs, rhs);
    EXPECT_FALSE(CF(register_file));
    EXPECT_FALSE(SF(register_file));
    EXPECT_TRUE(ZF(register_file));
    EXPECT_FALSE(AF(register_file));
    EXPECT_FALSE(OF(register_file));
    EXPECT_TRUE(PF(register_file));

    AX(register_file) = 5;
    BX(register_file) = 7;
    CF(register_file) = 1;
    SF(register_file) = 0;
    ZF(register_file) = 1;
    AF(register_file) = 1;
    PF(register_file) = 1;
    x86_insn_cmp(register_file, lhs, rhs);
    EXPECT_TRUE(CF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_TRUE(AF(register_file));
    EXPECT_FALSE(PF(register_file));

    AX(register_file) = 7;
    BX(register_file) = 5;
    CF(register_file) = 1;
    SF(register_file) = 1;
    ZF(register_file) = 1;
    AF(register_file) = 1;
    PF(register_file) = 1;
    x86_insn_cmp(register_file, lhs, rhs);
    EXPECT_FALSE(CF(register_file));
    EXPECT_FALSE(SF(register_file));
    EXPECT_FALSE(ZF(register_file));
    EXPECT_FALSE(AF(register_file));
    EXPECT_FALSE(PF(register_file));
}

TEST_F(x86_semantics_test, CMPSB)
{
    char str1[] = "foo";
    char str2[] = "bar";

    DS(register_file) = 0x0;
    SI(register_file) = 0x100;
    ES(register_file) = 0x0;
    DI(register_file) = 0x200;
    DF(register_file) = 0;
    address_space_access_segmented(&as, 1, DS(register_file), SI(register_file), strlen(str1), reinterpret_cast<uint8_t*>(&str1[0]));
    address_space_access_segmented(&as, 1, ES(register_file), DI(register_file), strlen(str1), reinterpret_cast<uint8_t*>(&str1[0]));
    x86_insn_cmpsb(register_file, &as);
    EXPECT_TRUE(ZF(register_file));
    EXPECT_EQ(SI(register_file), 0x101);
    EXPECT_EQ(DI(register_file), 0x201);

    DS(register_file) = 0x0;
    SI(register_file) = 0x100;
    ES(register_file) = 0x0;
    DI(register_file) = 0x200;
    DF(register_file) = 0;
    address_space_access_segmented(&as, 1, DS(register_file), SI(register_file), strlen(str1), reinterpret_cast<uint8_t*>(&str1[0]));
    address_space_access_segmented(&as, 1, ES(register_file), DI(register_file), strlen(str2), reinterpret_cast<uint8_t*>(&str2[0]));
    x86_insn_cmpsb(register_file, &as);
    EXPECT_FALSE(ZF(register_file));
    EXPECT_EQ(SI(register_file), 0x101);
    EXPECT_EQ(DI(register_file), 0x201);
}

TEST_F(x86_semantics_test, CMPSW)
{
    char str1[] = "foo";
    char str2[] = "bar";

    DS(register_file) = 0x0;
    SI(register_file) = 0x100;
    ES(register_file) = 0x0;
    DI(register_file) = 0x200;
    DF(register_file) = 0;
    address_space_access_segmented(&as, 1, DS(register_file), SI(register_file), strlen(str1), reinterpret_cast<uint8_t*>(&str1[0]));
    address_space_access_segmented(&as, 1, ES(register_file), DI(register_file), strlen(str1), reinterpret_cast<uint8_t*>(&str1[0]));
    x86_insn_cmpsw(register_file, &as);
    EXPECT_TRUE(ZF(register_file));
    EXPECT_EQ(SI(register_file), 0x102);
    EXPECT_EQ(DI(register_file), 0x202);

    DS(register_file) = 0x0;
    SI(register_file) = 0x100;
    ES(register_file) = 0x0;
    DI(register_file) = 0x200;
    DF(register_file) = 0;
    address_space_access_segmented(&as, 1, DS(register_file), SI(register_file), strlen(str1), reinterpret_cast<uint8_t*>(&str1[0]));
    address_space_access_segmented(&as, 1, ES(register_file), DI(register_file), strlen(str2), reinterpret_cast<uint8_t*>(&str2[0]));
    x86_insn_cmpsw(register_file, &as);
    EXPECT_FALSE(ZF(register_file));
    EXPECT_EQ(SI(register_file), 0x102);
    EXPECT_EQ(DI(register_file), 0x202);
}