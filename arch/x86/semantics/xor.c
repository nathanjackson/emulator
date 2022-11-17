//
// Created by nathan on 11/16/22.
//
#include "operand.h"
#include "register_file.h"

void x86_insn_xor(struct x86_register_file* register_file, struct operand* lhs, struct operand* rhs)
{
    if (sizeof(byte) == lhs->size) {
        BYTE(lhs) ^= BYTE(rhs);
        ZF(register_file) = (0 == BYTE(lhs));
        SF(register_file) = (0 != (BYTE(lhs) & 0x80));
    } else {
        WORD(lhs) ^= WORD(rhs);
        ZF(register_file) = (0 == WORD(lhs));
        SF(register_file) = (0 != (WORD(lhs) & 0x8000));
    }
    PF(register_file) = (0 == (__builtin_popcount(BYTE(lhs)) % 2));
    CF(register_file) = 0;
    OF(register_file) = 0;
}