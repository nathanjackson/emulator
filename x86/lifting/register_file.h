//
// Created by nathan on 10/30/22.
//

#ifndef EMULATOR_REGISTER_FILE_H
#define EMULATOR_REGISTER_FILE_H

#include "types.h"

#define NUM_GP_REGS 8
#define NUM_SEG_REGS 4

struct x86_register_file
{
    word gp_regs[NUM_GP_REGS];
    word seg_regs[NUM_SEG_REGS];
    word ip;
    word flags;
};

#define AL(rf) *((byte *)(&((rf)->gp_regs[0])))
#define AH(rf) *(((byte *)(&((rf)->gp_regs[0]))) + 1)
#define AX(rf) *((word *)(&((rf)->gp_regs[0])))

#endif //EMULATOR_REGISTER_FILE_H
