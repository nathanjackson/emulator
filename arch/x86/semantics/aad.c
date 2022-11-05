//
// Created by nathan on 11/5/22.
//

#include "operand.h"
#include "register_file.h"

void x86_insn_aad(struct x86_register_file* register_file, struct operand* base)
{
    byte temp_al = AL(register_file);
    byte temp_ah = AH(register_file);
    AL(register_file) = (temp_al + (temp_ah * BYTE(base))) & 0xFF;
    AH(register_file) = 0;
}