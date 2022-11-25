//
// Created by nathan on 11/25/22.
//

#include <gtest/gtest.h>

#include "address_space.h"
#include "ram.h"

TEST(RAM, Conventional)
{
    struct address_space pc_address_space;
    struct ram ram;

    address_space_initialize(&pc_address_space, UINT32_MAX);
    ram_initialize(&ram, 655360);
    ram_map_conventional_memory(&ram, &pc_address_space);

    uint32_t value = 0xA0A0A0A0;
    address_space_access(&pc_address_space, 1, 0x0, 4, reinterpret_cast<uint8_t *>(&value));

    EXPECT_EQ(value, *reinterpret_cast<uint32_t *>(ram.ram));
}