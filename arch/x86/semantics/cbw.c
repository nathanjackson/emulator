//
// Created by nathan on 11/5/22.
//

#include "register_file.h"

void x86_insn_cbw(struct x86_register_file* register_file)
{
    if (AL(register_file) & 0x80) {
        AH(register_file) = 0xFF;
    } else {
        AH(register_file) = 0x0;
    }
}