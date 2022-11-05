//
// Created by nathan on 11/5/22.
//

#include "operand.h"
#include "register_file.h"

void x86_insn_aam(struct x86_register_file* register_file, struct operand* base)
{
    byte temp_al = AL(register_file);
    AH(register_file) = temp_al / BYTE(base);
    AL(register_file) = temp_al % BYTE(base);
}