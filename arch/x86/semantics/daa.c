//
// Created by nathan on 11/8/22.
//

#include "register_file.h"

void x86_insn_daa(struct x86_register_file* register_file)
{
    if ((AL(register_file) & 0x0F) > 9 || AF(register_file)) {
        AL(register_file) += 6;
        AF(register_file) = 1;
    }
    if (AL(register_file) > 0x9F || CF(register_file)) {
        AL(register_file) += 0x60;
        CF(register_file) = 1;
    }
}