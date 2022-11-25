//
// Created by nathan on 11/5/22.
//

#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/immediate_operand.h"

void x86_insn_call_near(struct x86_register_file*, struct address_space*, struct operand*);
void x86_insn_call_far(struct x86_register_file*, struct address_space*, struct operand*, struct operand*);
}

TEST_F(x86_semantics_test, CALL_Near)
{
    struct immediate_operand imm;
    struct operand* tgt = make_u16_immediate(&imm, 0x3);

    SS(register_file) = 0x0;
    SP(register_file) = 0xFFFF;
    IP(register_file) = 0x1234;
    x86_insn_call_near(register_file, &as, tgt);
    EXPECT_EQ(SP(register_file), 0xFFFD);
    word got_rap = UINT16_MAX;
    address_space_access_segmented(&as, 0, SS(register_file), SP(register_file), 2, (uint8_t*)&got_rap);
    EXPECT_EQ(got_rap, 0x1234);
    EXPECT_EQ(IP(register_file), 0x0003);
}

TEST_F(x86_semantics_test, CALL_Far)
{
    struct immediate_operand imm1, imm2;
    struct operand* seg = make_u16_immediate(&imm1, 0x12);
    struct operand* tgt = make_u16_immediate(&imm2, 0x34);

    CS(register_file) = 0xBBBB;
    SS(register_file) = 0x0;
    SP(register_file) = 0xFFFF;
    IP(register_file) = 0xAAAA;
    x86_insn_call_far(register_file, &as, seg, tgt);
    EXPECT_EQ(SP(register_file), 0xFFFB);
    word got_rap = UINT16_MAX;
    word got_rsg = UINT16_MAX;
    address_space_access_segmented(&as, 0, 0x0, 0xFFFB, 2, (uint8_t*)&got_rap);
    address_space_access_segmented(&as, 0, 0x0, 0xFFFD, 2, (uint8_t*)&got_rsg);
    EXPECT_EQ(got_rap, 0xAAAA);
    EXPECT_EQ(got_rsg, 0xBBBB);
    EXPECT_EQ(CS(register_file), 0x12);
    EXPECT_EQ(IP(register_file), 0x34);
}