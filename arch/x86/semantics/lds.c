//
// Created by nathan on 11/17/22.
//

#include "register_file.h"
#include "operand.h"

void x86_insn_lds(struct x86_register_file* register_file, struct operand* dst, struct operand* src)
{
    dword tmp = * ((dword*) (&(BYTE(src))));
    WORD(dst) = (word)tmp;
    DS(register_file) = (word)(tmp >> 16);
}