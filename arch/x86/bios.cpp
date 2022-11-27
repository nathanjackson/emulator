//
// Created by nathan on 11/26/22.
//
#include <array>

#include "address_space.h"

#include "bios.h"

static void write_diskette_base_table_into_bda(struct address_space* as)
{
    // zero the diskette base table
    std::array<uint8_t, 14> values = {
            // disk controller mode byte 1
            0x0,
            // disk controller mode byte 2
            0x0,
            // clock ticks until motor off
            0x0,
            // fm or mfm
            0x0,
            // last sector on track
            0x0,
            // gap length in bytes
            0x2,
            // disk data length
            0xff,
            // gap length when formatting
            0x0,
            // data pattern used during formatting
            0x0,
            // floppy head bounce delay
            0x0,
            // floppy motor start delay
            0x0,
            // floppy motor stop delay
            0x0
    };
    address_space_access_segmented(as, 1, 0x50, 0x22, values.size(), values.data());
}

static void setup_ivt(struct address_space* as)
{
    std::array<uint16_t, 2> dbpt_ivt = { 0x22, 0x50 };
    address_space_access_segmented(as, 1, 0x0, 0x78, dbpt_ivt.size() * 2, reinterpret_cast<uint8_t*>(dbpt_ivt.data()));
}

void bios_initialize(struct address_space* as)
{
    write_diskette_base_table_into_bda(as);

    setup_ivt(as);
}