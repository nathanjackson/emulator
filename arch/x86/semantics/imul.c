//
// Created by nathan on 11/8/22.
//

#include "operand.h"
#include "register_file.h"

void x86_insn_imul(struct x86_register_file* register_file, struct operand* op)
{
    int flag;
    if (sizeof(byte) == op->size) {
        int16_t lhs = (int8_t)AL(register_file);
        int16_t rhs = (int8_t)BYTE(op);
        int16_t tmp = lhs * rhs;
        flag = (tmp & 0xFF00) > 0;
        AX(register_file) = tmp;
    } else if (sizeof(word) == op->size) {
        int32_t lhs = (int16_t) AX(register_file);
        int32_t rhs = (int16_t) WORD(op);
        int32_t tmp = lhs * rhs;
        flag = (tmp & 0xFFFF0000) > 0;
        DX(register_file) = tmp & 0xFFFF0000;
        AX(register_file) = tmp & 0x0000FFFF;
    }
    CF(register_file) = flag;
    OF(register_file) = flag;
}