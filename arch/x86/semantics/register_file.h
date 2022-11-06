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
#define ZF(rf) rf->flags.zf
#define PF(rf) rf->flags.pf
#define SF(rf) rf->flags.sf
#define OF(rf) rf->flags.of
#define DF(rf) rf->flags.df
#define IF(rf) rf->flags.if_

#define AL(rf) *((byte *)(&((rf)->gp_regs[0])))
#define AH(rf) *(((byte *)(&((rf)->gp_regs[0]))) + 1)
#define AX(rf) *((word *)(&((rf)->gp_regs[0])))

#define CL(rf) *((byte *)(&((rf)->gp_regs[1])))
#define CH(rf) *(((byte *)(&((rf)->gp_regs[1]))) + 1)
#define CX(rf) *((word *)(&((rf)->gp_regs[1])))

#define DL(rf) *((byte *)(&((rf)->gp_regs[2])))
#define DH(rf) *(((byte *)(&((rf)->gp_regs[2]))) + 1)
#define DX(rf) *((word *)(&((rf)->gp_regs[2])))

#define BL(rf) *((byte *)(&((rf)->gp_regs[3])))
#define BH(rf) *(((byte *)(&((rf)->gp_regs[3]))) + 1)
#define BX(rf) *((word *)(&((rf)->gp_regs[3])))

#define SP(rf) *((word *)(&((rf)->gp_regs[4])))

#define IP(rf) (rf)->ip

#define CS(rf) *((word *)(&((rf)->seg_regs[0])))
#define SS(rf) *((word *)(&((rf)->seg_regs[2])))

inline size_t segment_offset(x86_reg seg)
{
    switch (seg) {
    case X86_REG_CS: {
        return 0;
    } break;
    case X86_REG_DS: {
        return 1;
    } break;
    case X86_REG_SS: {
        return 2;
    } break;
    default:
        abort();
    }
}

#endif //EMULATOR_REGISTER_FILE_H
