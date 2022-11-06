//
// Created by nathan on 11/5/22.
//

#include "register_file.h"

void x86_insn_cld(struct x86_register_file* register_file)
{
    DF(register_file) = 0;
}