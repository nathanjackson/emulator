//
// Created by nathan on 11/5/22.
//

#include "x86_semantics_test.h"

void x86_semantics_test::SetUp()
{
    address_space_initialize(&as, 0xFFFFF);
    ram_initialize(&ram, 0xA0000);
    ram_map_conventional_memory(&ram, &as);

    //_mem_struct.ptr = _mem.data();
    //mem = &_mem_struct;
}
