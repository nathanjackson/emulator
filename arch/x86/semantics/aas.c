//
// Created by nathan on 11/5/22.
//

#include "register_file.h"

void x86_insn_aas(struct x86_register_file* register_file)
{
    if (((AL(register_file) & 0x0F) > 9) || (AF(register_file))) {
        AX(register_file) -= 6;
        AH(register_file) -= 1;
        AF(register_file) = 1;
        CF(register_file) = 1;
        AL(register_file) = AL(register_file) & 0x0F;
    } else {
        CF(register_file) = 0;
        AF(register_file) = 0;
        AL(register_file) &= 0x0F;
    }
}