//
// Created by nathan on 11/13/22.
//

#include "operand.h"
#include "register_file.h"

void x86_insn_jmp(struct x86_register_file* register_file, struct operand* tgt)
{
    IP(register_file) = WORD(tgt);
}