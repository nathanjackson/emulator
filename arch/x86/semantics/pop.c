//
// Created by nathan on 11/17/22.
//

#include "address_space.h"

#include "register_operand.h"

void x86_insn_pop(struct x86_register_file* register_file, struct address_space* as, struct operand* op)
{
    address_space_access_segmented(as, 0, SS(register_file), SP(register_file), sizeof(word), &WORD(op));
    SP(register_file) += sizeof(WORD(op));
}