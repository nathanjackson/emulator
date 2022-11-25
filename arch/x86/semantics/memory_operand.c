//
// Created by nathan on 11/4/22.
//

#include "memory_operand.h"

static void* host_ptr(struct memory_operand* mem_op)
{
    word segment = mem_op->register_file->seg_regs[mem_op->segment_offset];
    word offset_in_segment = mem_op->displacement;
    if (X86_REG_INVALID != mem_op->base_reg) {
        offset_in_segment += get_register_value(mem_op->register_file, mem_op->base_reg);
    }
    if (X86_REG_INVALID != mem_op->index_reg) {
        offset_in_segment += get_register_value(mem_op->register_file, mem_op->index_reg);
    }
    return address_space_get_host_ptr_segmented(mem_op->as, segment, offset_in_segment);
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

struct operand* make_memory_operand_direct(struct memory_operand* out, struct address_space* as, struct x86_register_file* reg_file, size_t size, x86_reg seg, word addr)
{
    out->base.vtable = &vtable;
    out->base.size = size;

    out->as = as;
    out->register_file = reg_file;

    out->segment_offset = segment_offset(seg);
    out->displacement = addr;
    out->base_reg = X86_REG_INVALID;
    out->index_reg = X86_REG_INVALID;

    return (struct operand*)out;
}

struct operand* make_memory_operand_indirect(struct memory_operand* out, struct address_space* as, struct x86_register_file* register_file, size_t size, x86_reg seg, x86_reg reg, word disp)
{
    out->base.vtable = &vtable;
    out->base.size = size;

    out->as = as;
    out->register_file = register_file;

    out->segment_offset = segment_offset(seg);
    out->displacement = disp;
    out->base_reg = reg;
    out->index_reg = X86_REG_INVALID;

    return (struct operand*)out;
}