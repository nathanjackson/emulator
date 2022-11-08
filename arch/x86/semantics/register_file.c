//
// Created by nathan on 11/7/22.
//

#include "register_file.h"

uint16_t get_register_value(struct x86_register_file* register_file, x86_reg reg)
{
    switch (reg) {
    case X86_REG_AL: {
        return AL(register_file);
    } break;
    case X86_REG_AH: {
        return AH(register_file);
    } break;
    case X86_REG_AX: {
        return AX(register_file);
    } break;
    case X86_REG_DI: {
        return DI(register_file);
    } break;
    case X86_REG_SI: {
        return SI(register_file);
    } break;
    }
}

size_t segment_offset(x86_reg seg)
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
    case X86_REG_ES: {
        return 3;
    } break;
    default:
        abort();
    }
}
