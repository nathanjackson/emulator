//
// Created by nathan on 11/16/22.
//

#include <stdlib.h>

#include "operand.h"

void x86_insn_mov(struct operand* dest, struct operand* src)
{
    if (sizeof(byte) == dest->size) {
        BYTE(dest) = BYTE(src);
    } else if (sizeof(word) == dest->size) {
        WORD(dest) = WORD(src);
    } else {
        abort();
    }
}