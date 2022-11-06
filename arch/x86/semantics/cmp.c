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
}