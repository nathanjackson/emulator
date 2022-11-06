//
// Created by nathan on 11/5/22.
//

#include "register_operand.h"
#include "operand.h"

void x86_insn_and(struct x86_register_file* register_file, struct operand* dest, struct operand* src)
{
    if (sizeof(byte) == dest->size) {
        BYTE(dest) &= BYTE(src);
        SF(register_file) = (BYTE(dest) & 0x80) > 0;
        ZF(register_file) = BYTE(dest) == 0;
    } else if (sizeof(word) == dest->size) {
        WORD(dest) &= WORD(src);
        SF(register_file) = (WORD(dest) & 0x8000) > 0;
        ZF(register_file) = WORD(dest) == 0;
    }
    OF(register_file) = 0;
    CF(register_file) = 0;
    PF(register_file) = (0 == (__builtin_popcount(BYTE(dest)) % 2));
}