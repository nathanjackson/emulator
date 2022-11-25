//
// Created by nathan on 11/25/22.
//

#ifndef EMULATOR_RAM_H
#define EMULATOR_RAM_H

#include <stdint.h>

#include "address_space.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct ram_region
{
    struct region base;
    struct ram* parent;
    size_t phys_start;
};

struct ram
{
    uint8_t* ram;
    size_t size;

    struct ram_region *mapped_regions;
};

/**
 * Allocates a block of memory to be used as RAM for an emulated system.
 *
 * @param ram
 * @param size
 */
void ram_initialize(struct ram *ram, size_t size);

/**
 * Maps the RAM into the given address space for a IBM PC or compatible guest.
 *
 * @param ram
 * @param as
 */
void ram_map_conventional_memory(struct ram* ram, struct address_space* as);

#ifdef __cplusplus
}
#endif

#endif //EMULATOR_RAM_H
