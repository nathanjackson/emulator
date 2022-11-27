//
// Created by nathan on 11/16/22.
//

#include <stdlib.h>

#include "address_space.h"
#include "register_file.h"

#include "operand.h"
#include "memory_operand.h"

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

void x86_insn_movsb(struct x86_register_file* register_file, struct address_space* as)
{
    struct memory_operand mem1;
    struct memory_operand mem2;

    struct operand* dest = make_memory_operand_indirect(&mem1, as, register_file, sizeof(byte), X86_REG_ES, X86_REG_DI, 0);
    struct operand* src = make_memory_operand_indirect(&mem2, as, register_file, sizeof(byte), X86_REG_DS, X86_REG_SI, 0);

    BYTE(dest) = BYTE(src);

    if (0 == DF(register_file)) {
        SI(register_file) += 1;
        DI(register_file) += 1;
    } else {
        SI(register_file) -= 1;
        DI(register_file) -= 1;
    }
}