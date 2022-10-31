//
// Created by nathan on 10/30/22.
//

#include <assert.h>

#include "operand.h"

void x86_insn_add(struct x86_register_file* register_file, struct operand* dest, struct operand* src)
{
    assert(2 >= dest->size && 0 < dest->size);
    assert(dest->size >= src->size);

    if (sizeof(word) == dest->size) {
        WORD(dest) += WORD(src);
    } else if (sizeof(byte) == dest->size) {
        BYTE(dest) += BYTE(src);
    }

    // TODO: update flags
}