//
// Created by nathan on 11/5/22.
//

#include "register_file.h"

void x86_insn_clc(struct x86_register_file* register_file)
{
    CF(register_file) = 0;
}