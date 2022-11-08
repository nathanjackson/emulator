//
// Created by nathan on 11/5/22.
//

#include <string.h>

#include "memory.h"

void* get_host_ptr(struct memory* memory, word segment, word offset)
{
    return memory->ptr + (segment * 16) + offset;
}

void memory_write(struct memory* memory, word segment, word offset, word size, const byte* buffer)
{
    void* dest = get_host_ptr(memory, segment, offset);
    memcpy(dest, buffer, size);
}