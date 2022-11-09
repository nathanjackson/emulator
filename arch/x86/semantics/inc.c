//
// Created by nathan on 11/8/22.
//

#include "operand.h"
#include "register_file.h"

void x86_insn_inc(struct x86_register_file* register_file, struct operand* dest)
{
    byte orig_dest_lsb = BYTE(dest);
    byte lsb = 0;

    if (sizeof(word) == dest->size) {
        int32_t delta = ((int32_t) ((int16_t) WORD(dest))) + 1;
        WORD(dest) = delta;
        OF(register_file) = (((int16_t)WORD(dest)) < 0 && delta < INT16_MIN) || (((int16_t)WORD(dest)) > 0 && delta > INT16_MAX);
        SF(register_file) = (delta & 0x8000) != 0;
        ZF(register_file) = (WORD(dest) == 0);
        lsb = (byte) delta;
    } else if (sizeof(byte) == dest->size) {
        int16_t delta = ((int16_t)((int8_t) BYTE(dest))) + 1;
        BYTE(dest) = delta;
        OF(register_file) = (((int8_t)BYTE(dest)) < 0 && delta < INT8_MIN) || (((int8_t)BYTE(dest)) > 0 && delta > INT8_MAX);
        SF(register_file) = (delta & 0x80) != 0;
        ZF(register_file) = (BYTE(dest) == 0);
        lsb = (byte) delta;
    }
    PF(register_file) = (0 == (__builtin_popcount(lsb) % 2));
    AF(register_file) = (((orig_dest_lsb & 0xF) + 1) & 0xF0) > 0;
}