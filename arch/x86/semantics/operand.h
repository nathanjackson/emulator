//
// Created by nathan on 10/30/22.
//

#ifndef EMULATOR_OPERAND_H
#define EMULATOR_OPERAND_H

#include <stddef.h>
#include <stdint.h>

#include "types.h"

struct operand_vtable;

/**
 * Operand interface
 */
struct operand
{
    size_t size;
    const struct operand_vtable *vtable;
};

/**
 * vtable declaration for all operands
 */
struct operand_vtable
{
    byte* (*byte_ptr)(struct operand*);
    word* (*word_ptr)(struct operand*);
};

#define BYTE_PTR(op) op->vtable->byte_ptr(op)
#define BYTE(op) *BYTE_PTR(op)
#define WORD_PTR(op) op->vtable->word_ptr(op)
#define WORD(op) *WORD_PTR(op)

#endif //EMULATOR_OPERAND_H
