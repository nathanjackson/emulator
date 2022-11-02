//
// Created by nathan on 10/30/22.
//

#include "register_file.h"
#include "operand.h"

void x86_insn_add(struct x86_register_file* register_file, struct operand* dest, struct operand* src)
{
    if (sizeof(word) == dest->size) {
        int32_t c = (int32_t) WORD(dest) + WORD(src);
        WORD(dest) = c;
        register_file->flags.of = (c < INT16_MIN || c > INT16_MAX);
        register_file->flags.sf = (WORD(dest) & 0x8000) > 0;
        register_file->flags.zf = WORD(dest) == 0;
    } else if (sizeof(byte) == dest->size) {
        int16_t c = (int16_t) BYTE(dest) + BYTE(src);
        BYTE(dest) = c;
        register_file->flags.of = (c < INT8_MIN || c > INT8_MAX);
        register_file->flags.sf = (BYTE(dest) & 0x80) > 0;
        register_file->flags.zf = BYTE(dest) == 0;
    }
}