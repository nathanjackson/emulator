//
// Created by nathan on 11/24/22.
//
#include <gtest/gtest.h>

#include "address_space.h"

struct test_region
{
    struct region base;
    uint32_t value;
};

static void read(struct region* self, uint64_t addr, size_t sz, uint8_t* buffer)
{
    struct test_region* tr = reinterpret_cast<struct test_region*>(self);

    if (addr == 0x0 && sz == sizeof(tr->value)) {
        memcpy(buffer, (uint8_t*) &tr->value, sz);
    }
}

static void write(struct region* self, uint64_t addr, size_t sz, uint8_t* buffer)
{
     struct test_region* tr = reinterpret_cast<struct test_region*>(self);
     if (addr == 0x0 && sz == sizeof(tr->value)) {
         memcpy((uint8_t*)&tr->value, buffer, sz);
     }
}

const static struct region_vtable vtable = {
        .read = &read,
        .write = &write,
};

TEST(address_space, lookup)
{
    struct address_space as;
    address_space_initialize(&as, 0xFFFFFFFF);

    struct test_region region;
    region.base.size = 4;
    region.base.vtable = &vtable;

    address_space_add_region(&as, 0x100, &region.base);

    struct test_region region2;
    region2.base.size = 0x100;
    region2.base.vtable = &vtable;

    address_space_add_region(&as, 0x0, &region2.base);

    uint32_t value = 0xA0A0A0A0;
    address_space_access(&as, 1, 0x100, 4, reinterpret_cast<uint8_t*>(&value));

    EXPECT_EQ(region.value, value);
}