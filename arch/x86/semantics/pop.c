//
// Created by nathan on 11/17/22.
//

#include "register_operand.h"
#include "memory.h"

void x86_insn_pop(struct x86_register_file* register_file, struct memory* memory, struct operand* op)
{
    WORD(op) = *((word*)(get_host_ptr(memory, SS(register_file), SP(register_file))));
    SP(register_file) += sizeof(WORD(op));
}