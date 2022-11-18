//
// Created by nathan on 11/17/22.
//

#include "memory.h"
#include "operand.h"
#include "register_file.h"

void x86_insn_push(struct x86_register_file* register_file, struct memory* memory, struct operand* op)
{
    SP(register_file) -= op->size;
    memory_write(memory, SS(register_file), SP(register_file), op->size, &WORD(op));
}