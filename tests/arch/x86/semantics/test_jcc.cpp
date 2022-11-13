//
// Created by nathan on 11/13/22.
//

#include "x86_semantics_test.h"

extern "C"
{
#include "arch/x86/semantics/register_file.h"
#include "arch/x86/semantics/immediate_operand.h"
void x86_insn_ja(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jae(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jb(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jbe(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jcxz(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_je(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jg(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jge(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jl(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jle(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jne(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jno(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jnp(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jns(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jo(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_jp(struct x86_register_file*, struct operand* tgt, struct operand* next);
void x86_insn_js(struct x86_register_file*, struct operand* tgt, struct operand* next);
}

TEST_F(x86_semantics_test, JA)
{
    struct immediate_operand imm1, imm2;
    struct operand* tgt = make_u16_immediate(&imm1, 0x5);
    struct operand* next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    CF(register_file) = 0;
    ZF(register_file) = 0;
    x86_insn_ja(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    CF(register_file) = 1;
    ZF(register_file) = 0;
    x86_insn_ja(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JAE)
{
    struct immediate_operand imm1, imm2;
    struct operand* tgt = make_u16_immediate(&imm1, 0x5);
    struct operand* next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    CF(register_file) = 0;
    x86_insn_jae(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    CF(register_file) = 1;
    x86_insn_jae(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JB)
{
    struct immediate_operand imm1, imm2;
    struct operand* tgt = make_u16_immediate(&imm1, 0x5);
    struct operand* next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    CF(register_file) = 1;
    x86_insn_jb(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    CF(register_file) = 0;
    x86_insn_jb(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JBE)
{
    struct immediate_operand imm1, imm2;
    struct operand* tgt = make_u16_immediate(&imm1, 0x5);
    struct operand* next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    CF(register_file) = 1;
    ZF(register_file) = 0;
    x86_insn_jbe(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    CF(register_file) = 0;
    ZF(register_file) = 1;
    x86_insn_jbe(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    CF(register_file) = 0;
    ZF(register_file) = 0;
    x86_insn_jbe(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JCXZ)
{
    struct immediate_operand imm1, imm2;
    struct operand *tgt = make_u16_immediate(&imm1, 0x5);
    struct operand *next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    CX(register_file) = 0x0;
    x86_insn_jcxz(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    CX(register_file) = 0x1;
    x86_insn_jcxz(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JE)
{
    struct immediate_operand imm1, imm2;
    struct operand *tgt = make_u16_immediate(&imm1, 0x5);
    struct operand *next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    ZF(register_file) = 1;
    x86_insn_je(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    ZF(register_file) = 0;
    x86_insn_je(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JG)
{
    struct immediate_operand imm1, imm2;
    struct operand *tgt = make_u16_immediate(&imm1, 0x5);
    struct operand *next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    ZF(register_file) = 0;
    SF(register_file) = 1;
    OF(register_file) = 1;
    x86_insn_jg(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    ZF(register_file) = 1;
    SF(register_file) = 1;
    OF(register_file) = 1;
    x86_insn_jg(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);

    IP(register_file) = 0x0;
    ZF(register_file) = 1;
    SF(register_file) = 0;
    OF(register_file) = 1;
    x86_insn_jg(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JGE)
{
    struct immediate_operand imm1, imm2;
    struct operand *tgt = make_u16_immediate(&imm1, 0x5);
    struct operand *next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    SF(register_file) = 1;
    OF(register_file) = 1;
    x86_insn_jge(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    SF(register_file) = 1;
    OF(register_file) = 0;
    x86_insn_jge(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JL)
{
    struct immediate_operand imm1, imm2;
    struct operand *tgt = make_u16_immediate(&imm1, 0x5);
    struct operand *next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    SF(register_file) = 1;
    OF(register_file) = 0;
    x86_insn_jl(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    SF(register_file) = 1;
    OF(register_file) = 1;
    x86_insn_jl(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JLE)
{
    struct immediate_operand imm1, imm2;
    struct operand *tgt = make_u16_immediate(&imm1, 0x5);
    struct operand *next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    SF(register_file) = 1;
    OF(register_file) = 0;
    ZF(register_file) = 0;
    x86_insn_jle(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    SF(register_file) = 1;
    OF(register_file) = 1;
    ZF(register_file) = 1;
    x86_insn_jle(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    SF(register_file) = 1;
    OF(register_file) = 1;
    ZF(register_file) = 0;
    x86_insn_jle(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JNE)
{
    struct immediate_operand imm1, imm2;
    struct operand *tgt = make_u16_immediate(&imm1, 0x5);
    struct operand *next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    ZF(register_file) = 0;
    x86_insn_jne(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    ZF(register_file) = 1;
    x86_insn_jne(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JNO)
{
    struct immediate_operand imm1, imm2;
    struct operand *tgt = make_u16_immediate(&imm1, 0x5);
    struct operand *next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    OF(register_file) = 0;
    x86_insn_jno(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    OF(register_file) = 1;
    x86_insn_jno(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JNP)
{
    struct immediate_operand imm1, imm2;
    struct operand *tgt = make_u16_immediate(&imm1, 0x5);
    struct operand *next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    PF(register_file) = 0;
    x86_insn_jnp(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    PF(register_file) = 1;
    x86_insn_jnp(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JNS)
{
    struct immediate_operand imm1, imm2;
    struct operand *tgt = make_u16_immediate(&imm1, 0x5);
    struct operand *next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    SF(register_file) = 0;
    x86_insn_jns(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    SF(register_file) = 1;
    x86_insn_jns(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JO)
{
    struct immediate_operand imm1, imm2;
    struct operand *tgt = make_u16_immediate(&imm1, 0x5);
    struct operand *next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    OF(register_file) = 1;
    x86_insn_jo(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    OF(register_file) = 0;
    x86_insn_jo(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JP)
{
    struct immediate_operand imm1, imm2;
    struct operand *tgt = make_u16_immediate(&imm1, 0x5);
    struct operand *next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    PF(register_file) = 1;
    x86_insn_jp(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    PF(register_file) = 0;
    x86_insn_jp(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}

TEST_F(x86_semantics_test, JS)
{
    struct immediate_operand imm1, imm2;
    struct operand *tgt = make_u16_immediate(&imm1, 0x5);
    struct operand *next = make_u16_immediate(&imm2, 0x2);

    IP(register_file) = 0x0;
    SF(register_file) = 1;
    x86_insn_js(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x5);

    IP(register_file) = 0x0;
    SF(register_file) = 0;
    x86_insn_js(register_file, tgt, next);
    EXPECT_EQ(IP(register_file), 0x2);
}