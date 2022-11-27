//
// Created by nathan on 11/16/22.
//

#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/register_operand.h"
#include "arch/x86/semantics/immediate_operand.h"
void x86_insn_mov(struct operand* dest, struct operand* src);
void x86_insn_movsb(struct x86_register_file*, struct address_space*);
void x86_insn_movsw(struct x86_register_file*, struct address_space*);
}

TEST_F(x86_semantics_test, MOV_8bit)
{
    struct immediate_operand imm;
    struct register_operand reg;

    struct operand* dest = make_register_operand(&reg, register_file, X86_REG_AH);
    struct operand* src = make_u8_immediate(&imm, 20);

    AH(register_file) = 0xFF;
    x86_insn_mov(dest, src);
    EXPECT_EQ(AH(register_file), 20);
}

TEST_F(x86_semantics_test, MOV_16bit)
{
    struct immediate_operand imm;
    struct register_operand reg;

    struct operand* dest = make_register_operand(&reg, register_file, X86_REG_AX);
    struct operand* src = make_u16_immediate(&imm, 3);

    AH(register_file) = 0xFFFF;
    x86_insn_mov(dest, src);
    EXPECT_EQ(AX(register_file), 3);
}

TEST_F(x86_semantics_test, MOVSB)
{
    char str1[] = "abcd";

    ES(register_file) = 0x0;
    DI(register_file) = 0x200;
    DS(register_file) = 0x0;
    SI(register_file) = 0x100;
    DF(register_file) = 0;
    address_space_access_segmented(&as, 1, DS(register_file), SI(register_file), strlen(str1), reinterpret_cast<uint8_t*>(&str1[0]));

    // iteration 1
    x86_insn_movsb(register_file, &as);
    EXPECT_EQ(DI(register_file), 0x201);
    EXPECT_EQ(SI(register_file), 0x101);
    uint8_t value = 0;
    address_space_access_segmented(&as, 0, ES(register_file), DI(register_file) - 1, 1, &value);
    EXPECT_EQ(value, 'a');

    // iteration 2
    x86_insn_movsb(register_file, &as);
    EXPECT_EQ(DI(register_file), 0x202);
    EXPECT_EQ(SI(register_file), 0x102);
    address_space_access_segmented(&as, 0, ES(register_file), DI(register_file) - 1, 1, &value);
    EXPECT_EQ(value, 'b');

    // reverse direction
    DI(register_file) = 0x203;
    SI(register_file) = 0x103;
    DF(register_file) = 1;

    // iteration 1
    x86_insn_movsb(register_file, &as);
    EXPECT_EQ(DI(register_file), 0x202);
    EXPECT_EQ(SI(register_file), 0x102);
    address_space_access_segmented(&as, 0, ES(register_file), DI(register_file) + 1, 1, &value);
    EXPECT_EQ(value, 'd');

    // iteration 2
    x86_insn_movsb(register_file, &as);
    EXPECT_EQ(DI(register_file), 0x201);
    EXPECT_EQ(SI(register_file), 0x101);
    address_space_access_segmented(&as, 0, ES(register_file), DI(register_file) + 1, 1, &value);
    EXPECT_EQ(value, 'c');
}

TEST_F(x86_semantics_test, MOVSW)
{
    char str1[] = "abcd";

    ES(register_file) = 0x0;
    DI(register_file) = 0x200;
    DS(register_file) = 0x0;
    SI(register_file) = 0x100;
    DF(register_file) = 0;
    address_space_access_segmented(&as, 1, DS(register_file), SI(register_file), strlen(str1), reinterpret_cast<uint8_t*>(&str1[0]));

    x86_insn_movsw(register_file, &as);
    EXPECT_EQ(DI(register_file), 0x202);
    EXPECT_EQ(SI(register_file), 0x102);
    uint8_t buf[2];
    address_space_access_segmented(&as, 0, ES(register_file), DI(register_file) - 2, 2, buf);
    EXPECT_EQ(buf[0], 'a');
    EXPECT_EQ(buf[1], 'b');

    // reverse direction
    DI(register_file) = 0x202;
    SI(register_file) = 0x102;
    DF(register_file) = 1;

    x86_insn_movsw(register_file, &as);
    EXPECT_EQ(DI(register_file), 0x200);
    EXPECT_EQ(SI(register_file), 0x100);
    address_space_access_segmented(&as, 0, ES(register_file), DI(register_file) + 2, 2, buf);
    EXPECT_EQ(buf[0], 'c');
    EXPECT_EQ(buf[1], 'd');
}