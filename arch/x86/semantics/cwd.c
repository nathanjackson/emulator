//
// Created by nathan on 11/7/22.
//

#include "register_file.h"

void x86_insn_cwd(struct x86_register_file* register_file)
{
    if (AX(register_file) & 0x8000) {
        DX(register_file) = 0xFFFF;
    } else {
        DX(register_file) = 0x0;
    }
}