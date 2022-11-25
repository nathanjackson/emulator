//
// Created by nathan on 11/24/22.
//

#include <assert.h>
#include <stdlib.h>

#include "address_space.h"

struct region_tree_data
{
    struct avl_node node;
    uint64_t base_addr;
    struct region* region;
};

static int region_tree_data_cmp(struct avl_node* lhs, struct avl_node* rhs)
{
    struct region_tree_data* lhs_rtd = (struct region_tree_data*) lhs;
    struct region_tree_data* rhs_rtd = (struct region_tree_data*) rhs;

    if ((lhs_rtd->base_addr + lhs_rtd->region->size - 1) > (rhs_rtd->base_addr)) {
        return 1;
    } else {
        return -1;
    }
}

void address_space_initialize(struct address_space* as, uint64_t max_addr)
{
    as->max_addr = max_addr;
    avl_tree_init(&as->regions, &region_tree_data_cmp);
}

void address_space_add_region(struct address_space* as, uint64_t addr, struct region* region)
{
    struct region_tree_data* rtd = malloc(sizeof(*rtd));
    rtd->base_addr = addr;
    rtd->region = region;

    avl_tree_insert(&as->regions, &rtd->node);
}

static int addr_lookup_query(struct avl_node* node, void* callback_ref)
{
    uint64_t addr = *(uint64_t *)callback_ref;
    struct region_tree_data* rtd = (struct region_tree_data*)node;

    if ((rtd->base_addr <= addr) && (addr <= (rtd->base_addr + rtd->region->size - 1))) {
        return 0;
    } else if (addr < rtd->base_addr) {
        return 1;
    } else {
        return -1;
    }
}

void address_space_access(struct address_space* as, int rw, uint64_t addr, size_t size, uint8_t* buffer)
{
    struct avl_node* node = avl_tree_search(&as->regions, &addr, &addr_lookup_query);
    assert(node);

    struct region_tree_data* rtd = node;
    uint64_t region_addr = addr - rtd->base_addr;
    if (rw) {
        rtd->region->vtable->write(rtd->region, region_addr, size, buffer);
    } else {
        rtd->region->vtable->read(rtd->region, region_addr, size, buffer);
    }
}