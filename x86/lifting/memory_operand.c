//
// Created by nathan on 11/4/22.
//

#include "memory_operand.h"

static void* host_ptr(struct memory_operand* mem_op)
{
    word segment = mem_op->register_file->seg_regs[mem_op->segment_offset];
    word offset_in_segment = mem_op->displacement;
    if (X86_REG_INVALID != mem_op->base_reg) {
        offset_in_segment += mem_op->register_file->gp_regs[mem_op->base_reg];
    }
    if (X86_REG_INVALID != mem_op->index_reg) {
        offset_in_segment += mem_op->register_file->gp_regs[mem_op->index_reg];
    }
    size_t host_offset = segment * 16 + offset_in_segment;
    return mem_op->mem->ptr + host_offset;
}

static byte* byte_ptr(struct operand* op)
{
    struct memory_operand* mem_op = (struct memory_operand*)op;
    return host_ptr(mem_op);
}

static word* word_ptr(struct operand* op)
{
    struct memory_operand* mem_op = (struct memory_operand*)op;
    return host_ptr(mem_op);
}

const static struct operand_vtable vtable = {
        .byte_ptr = &byte_ptr,
        .word_ptr = &word_ptr,
};

struct operand* make_memory_operand_direct(struct memory_operand* out, struct memory* mem, struct x86_register_file* reg_file, size_t size, size_t segment_offset, word addr)
{
    out->base.vtable = &vtable;
    out->base.size = size;

    out->mem = mem;
    out->register_file = reg_file;

    out->segment_offset = segment_offset;
    out->displacement = addr;
    out->base_reg = X86_REG_INVALID;
    out->index_reg = X86_REG_INVALID;

    return (struct operand*)out;
}
