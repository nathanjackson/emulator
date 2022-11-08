//
// Created by nathan on 11/4/22.
//

#ifndef EMULATOR_MEMORY_H
#define EMULATOR_MEMORY_H

#include "types.h"

struct memory
{
    void *ptr;
};

void* get_host_ptr(struct memory* memory, word segment, word offset);

void memory_write(struct memory*, word segment, word offset, word size, const byte* buffer);

#endif //EMULATOR_MEMORY_H
