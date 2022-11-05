//
// Created by nathan on 10/30/22.
//

#ifndef EMULATOR_REGISTER_OPERAND_H
#define EMULATOR_REGISTER_OPERAND_H

#include <capstone/capstone.h>

#include "operand.h"
#include "register_file.h"


struct register_operand
{
    struct operand base;
    void* reg_ptr;
};

struct operand* make_register_operand(struct register_operand* out, struct x86_register_file* reg_file,
        x86_reg reg);

#endif //EMULATOR_REGISTER_OPERAND_H
