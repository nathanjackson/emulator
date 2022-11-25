//
// Created by nathan on 11/25/22.
//

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "ram.h"

static void ram_region_read(struct region* region, uint64_t addr, size_t size, uint8_t* buffer)
{
    struct ram_region* rr = (struct ram_region*)region;
    uint8_t* src = rr->parent->ram + rr->phys_start + addr;
    memcpy(buffer, src, size);
}

static void ram_region_write(struct region* region, uint64_t addr, size_t size, uint8_t* buffer)
{
    struct ram_region* rr = (struct ram_region*)region;
    uint8_t* dest = rr->parent->ram + rr->phys_start + addr;
    memcpy(dest, buffer, size);
}

const static struct region_vtable vtable = {
        .read = &ram_region_read,
        .write = &ram_region_write,
};

void ram_initialize(struct ram* ram, size_t size)
{
    ram->size = size;
    ram->ram = malloc(size);
    ram->mapped_regions = NULL;
}

void ram_map_conventional_memory(struct ram* ram, struct address_space* as)
{
    ram->mapped_regions = malloc(1 * sizeof(*ram->mapped_regions));
    assert(ram->mapped_regions);

    struct ram_region* conventional = &(ram->mapped_regions[0]);

    // setup conventional memory
    conventional->parent = ram;
    conventional->phys_start = 0x0;
    conventional->base.size = 0xA0000;
    conventional->base.vtable = &vtable;
    address_space_add_region(as, 0x0, conventional);
}