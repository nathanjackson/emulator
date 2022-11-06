//
// Created by nathan on 11/5/22.
//

#include "memory.h"

void* get_host_ptr(struct memory* memory, word segment, word offset)
{
    return memory->ptr + (segment * 16) + offset;
}