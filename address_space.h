//
// Created by nathan on 11/24/22.
//

#ifndef EMULATOR_ADDRESS_SPACE_H
#define EMULATOR_ADDRESS_SPACE_H

#include <stdint.h>
#include <stddef.h>

#include "utils/avl_tree.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct region_vtable;

struct region
{
    size_t size;
    const struct region_vtable *vtable;
};

struct region_vtable
{
    void (*read)(struct region* self, uint64_t addr, size_t size, uint8_t* buffer);
    void (*write)(struct region* self, uint64_t addr, size_t size, uint8_t* buffer);
};

struct address_space
{
    uint64_t max_addr;
    struct avl_tree regions;
};

/**
 * Initializes an address space struct.
 *
 * @param as the address space to initialize
 * @param max_addr the maximum address for the address space
 */
void address_space_initialize(struct address_space* as, uint64_t max_addr);

/**
 * Adds a region to an address_space.
 * @param as the address space that the region will be added to
 * @param max_addr the base address relative to the start of the address space to place the region
 * @param region the region to add to the address space
 */
void address_space_add_region(struct address_space* as, uint64_t addr, struct region*);

/**
 * Access memory in the address space.
 *
 * @param as the address space to access
 * @param rw read or write flag (0 = read, 1 = write)
 * @param addr the address relative to the address space to access
 * @param size the number of bytes to read or write
 * @param buffer the buffer for output or input
 */
void address_space_access(struct address_space* as, int rw, uint64_t addr, size_t size, uint8_t* buffer);

#ifdef __cplusplus
}
#endif

#endif //EMULATOR_ADDRESS_SPACE_H
