//
// Created by nathan on 10/30/22.
//

#ifndef EMULATOR_IMMEDIATE_OPERAND_H
#define EMULATOR_IMMEDIATE_OPERAND_H

#include "operand.h"

struct immediate_operand
{
    struct operand base;
    word value;
};

/**
 * Constructs a 8-bit immediate operand.
 * @param imm the immediate operand struct
 * @param value the value
 * @return the operand, must be freed by the caller
 */
struct operand* make_u8_immediate(struct immediate_operand* imm, uint8_t value);

/**
 * Constructs a 16-bit immediate operand.
 * @param value the value
 * @return the operand, must be freed by the caller
 */
struct operand* make_u16_immediate(struct immediate_operand* imm, uint16_t value);

#endif //EMULATOR_IMMEDIATE_OPERAND_H
