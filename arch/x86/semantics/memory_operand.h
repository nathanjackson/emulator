//
// Created by nathan on 11/4/22.
//

#ifndef EMULATOR_MEMORY_OPERAND_H
#define EMULATOR_MEMORY_OPERAND_H

#include <capstone/capstone.h>

#include "memory.h"
#include "operand.h"
#include "register_file.h"

struct memory_operand
{
    struct operand base;

    struct memory* mem;
    struct x86_register_file* register_file;

    size_t segment_offset;
    word displacement;
    x86_reg base_reg;
    x86_reg index_reg;
};

struct operand* make_memory_operand_direct(struct memory_operand* out, struct memory* mem, struct x86_register_file* reg_file, size_t size, x86_reg seg, word addr);
struct operand* make_memory_operand_indirect(struct memory_operand* out, struct memory*, struct x86_register_file*, size_t size, x86_reg seg, x86_reg);

//struct operand* make_memory_operand_based(struct memory_operand* out, struct register_file* register_file, x86_reg seg, x86_reg base, word disp);
//struct operand* make_memory_operand_indexed(struct memory_operand* out, struct register_file* register_file, x86_reg seg, x86_reg index);
//struct operand* make_memory_operand_based_index(struct memory_operand* out, struct register_file* register_file, x86_reg seg, x86_reg base, x86_reg index, word disp);

#endif //EMULATOR_MEMORY_OPERAND_H
