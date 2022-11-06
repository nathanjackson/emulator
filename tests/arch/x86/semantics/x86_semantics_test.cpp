//
// Created by nathan on 11/5/22.
//

#include "x86_semantics_test.h"

void x86_semantics_test::SetUp()
{
    _mem_struct.ptr = _mem.data();
    mem = &_mem_struct;
}
