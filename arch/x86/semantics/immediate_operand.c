//
// Created by nathan on 10/30/22.
//

#include "immediate_operand.h"

static byte* byte_ptr(struct operand* op)
{
    struct immediate_operand* imm = (struct immediate_operand*)op;
    return (byte*)&imm->value;
}

static word* word_ptr(struct operand* op)
{
    struct immediate_operand* imm = (struct immediate_operand*)op;
    return &imm->value;
}

const static struct operand_vtable vtable = {
        .byte_ptr = &byte_ptr,
        .word_ptr = &word_ptr,
};

struct operand* make_u8_immediate(struct immediate_operand* out, uint8_t imm)
{
    out->base.vtable = &vtable;
    out->base.size = 1;
    out->value = imm;
    return (struct operand*)out;
}

struct operand* make_u16_immediate(struct immediate_operand* out, uint16_t imm)
{
    out->base.vtable = &vtable;
    out->base.size = 2;
    out->value = imm;
    return (struct operand*)out;
}