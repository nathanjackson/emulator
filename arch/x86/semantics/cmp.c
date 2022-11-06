//
// Created by nathan on 11/5/22.
//

#include "register_file.h"
#include "operand.h"

void x86_insn_cmp(struct x86_register_file* register_file, struct operand* lhs, struct operand* rhs)
{
    byte lsb = 0;

    if (sizeof(word) == lhs->size) {
        int32_t delta = (int32_t) WORD(lhs) - WORD(rhs);
        OF(register_file) = (delta < INT16_MIN || delta > INT16_MAX);
        SF(register_file) = (delta & 0x8000) != 0;
        ZF(register_file) = (delta == 0);
        CF(register_file) = (delta < 0);
        lsb = (byte) delta;
    } else if (sizeof(byte) == lhs->size) {
        int16_t delta = (int16_t) BYTE(lhs) - BYTE(rhs);
        OF(register_file) = (delta < INT8_MIN || delta > INT8_MAX);
        SF(register_file) = (delta & 0x80) != 0;
        ZF(register_file) = (delta == 0);
        CF(register_file) = (delta < 0);
        lsb = (byte) delta;
    }
    PF(register_file) = (0 == (__builtin_popcount(lsb) % 2));
    AF(register_file) = (((BYTE(lhs) & 0xF) - (BYTE(rhs) & 0xF)) & 0xF0) > 0;
//    if (sizeof(word) == lhs->size) {
//        word delta = WORD(lhs) - WORD(rhs);
//        CF(register_file) = __builtin_sub_overflow(WORD(lhs), WORD(rhs), &delta);
//        OF(register_file) = __builtin_sub_overflow((int16_t) WORD(lhs), (int16_t) WORD(rhs), &delta);
//        SF(register_file) = (delta & 0x8000);
//        ZF(register_file) = (delta == 0);
//    } else if (sizeof(byte) == lhs->size) {
//        byte delta = BYTE(lhs) - BYTE(rhs);
//        CF(register_file) = __builtin_sub_overflow(BYTE(lhs), BYTE(rhs), &delta);
//        OF(register_file) = __builtin_sub_overflow((int8_t) BYTE(lhs), (int8_t) BYTE(rhs), &delta);
//        SF(register_file) = (delta & 0x8000);
//        ZF(register_file) = (delta == 0);
//    }
//    PF(register_file) = (0 == (__builtin_popcount(BYTE(lhs)) % 2));
}