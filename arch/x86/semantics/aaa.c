//
// Created by nathan on 11/5/22.
//

#include "register_file.h"

void x86_insn_aaa(struct x86_register_file* register_file)
{
    if (((AL(register_file) & 0x0F) > 9) || (AF(register_file))) {
        AX(register_file) += 0x106;
        AF(register_file) = 1;
        CF(register_file) = 1;
    } else {
        AF(register_file) = 0;
        CF(register_file) = 0;
    }
    AL(register_file) &= 0x0F;
}