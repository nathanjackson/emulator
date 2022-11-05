//
// Created by nathan on 10/30/22.
//

#ifndef EMULATOR_REGISTER_FILE_H
#define EMULATOR_REGISTER_FILE_H

#include <stddef.h>

#include <capstone/capstone.h>

#include "types.h"

#define NUM_GP_REGS 8
#define NUM_SEG_REGS 4

struct flags_register
{
    int cf : 1;
    int reserved1 : 1;
    int pf : 1;
    int reserved2 : 1;
    int af : 1;
    int reserved3 : 1;
    int zf : 1;
    int sf : 1;
    int tf : 1;
    int if_ : 1;
    int df : 1;
    int of : 1;
};

struct x86_register_file
{
    word gp_regs[NUM_GP_REGS];
    word seg_regs[NUM_SEG_REGS];
    word ip;
    struct flags_register flags;
};

#define AF(rf) rf->flags.af
#define CF(rf) rf->flags.cf

#define AL(rf) *((byte *)(&((rf)->gp_regs[0])))
#define AH(rf) *(((byte *)(&((rf)->gp_regs[0]))) + 1)
#define AX(rf) *((word *)(&((rf)->gp_regs[0])))

inline size_t segment_offset(x86_reg seg)
{
    switch (seg) {
    case X86_REG_CS: {
        return 0;
    } break;
    case X86_REG_DS: {
        return 1;
    } break;
    default:
        abort();
    }
}

#endif //EMULATOR_REGISTER_FILE_H
