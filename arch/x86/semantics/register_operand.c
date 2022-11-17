//
// Created by nathan on 10/30/22.
//

#include "register_operand.h"

static byte* byte_ptr(struct operand* op)
{
    struct register_operand* reg_op = (struct register_operand*)op;
    return (byte*)reg_op->reg_ptr;
}

static word* word_ptr(struct operand* op)
{
    struct register_operand* reg_op = (struct register_operand*)op;
    return (word*)reg_op->reg_ptr;
}

const static struct operand_vtable vtable = {
        .byte_ptr = &byte_ptr,
        .word_ptr = &word_ptr,
};

struct operand* make_register_operand(struct register_operand* out, struct x86_register_file* reg_file,
        x86_reg reg)
{
    out->base.vtable = &vtable;

    switch (reg) {
    case X86_REG_AL: {
        out->base.size = 1;
        out->reg_ptr = &reg_file->gp_regs[0];
    } break;
    case X86_REG_AH: {
        out->base.size = 1;
        out->reg_ptr = ((byte*)&reg_file->gp_regs[0]) + 1;
    } break;
    case X86_REG_AX: {
        out->base.size = 2;
        out->reg_ptr = &reg_file->gp_regs[0];
    } break;

    case X86_REG_CL: {
        out->base.size = 1;
        out->reg_ptr = &reg_file->gp_regs[1];
    } break;
    case X86_REG_CH: {
        out->base.size = 1;
        out->reg_ptr = ((byte*)&reg_file->gp_regs[1]) + 1;
    } break;
    case X86_REG_CX: {
        out->base.size = 2;
        out->reg_ptr = &reg_file->gp_regs[1];
    } break;

    case X86_REG_DL: {
        out->base.size = 1;
        out->reg_ptr = &reg_file->gp_regs[2];
    } break;
    case X86_REG_DH: {
        out->base.size = 1;
        out->reg_ptr = ((byte*)&reg_file->gp_regs[2]) + 1;
    } break;
    case X86_REG_DX: {
        out->base.size = 2;
        out->reg_ptr = &reg_file->gp_regs[2];
    } break;

    case X86_REG_BL: {
        out->base.size = 1;
        out->reg_ptr = &reg_file->gp_regs[3];
    } break;
    case X86_REG_BH: {
        out->base.size = 1;
        out->reg_ptr = ((byte*)&reg_file->gp_regs[3]) + 1;
    } break;
    case X86_REG_BX: {
        out->base.size = 2;
        out->reg_ptr = &reg_file->gp_regs[3];
    } break;
    case X86_REG_SP: {
        out->base.size = 2;
        out->reg_ptr = &SP(reg_file);
    } break;
    case X86_REG_SS: {
        out->base.size = 2;
        out->reg_ptr = &SS(reg_file);
    } break;
    default: {
        fprintf(stderr, "Register not yet handled.\n");
        abort();
    } break;
    };
    return (struct operand*)out;
}