//
// Created by nathan on 11/17/22.
//

#include "address_space.h"

#include "operand.h"
#include "register_file.h"

void x86_insn_push(struct x86_register_file* register_file, struct address_space* as, struct operand* op)
{
    SP(register_file) -= op->size;
    address_space_access_segmented(as, 1, SS(register_file), SP(register_file), op->size, &WORD(op));
}