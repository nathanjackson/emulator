//
// Created by nathan on 11/5/22.
//

#ifndef EMULATOR_X86_SEMANTICS_TEST_H
#define EMULATOR_X86_SEMANTICS_TEST_H

#include <array>

#include <gtest/gtest.h>

extern "C"
{
#include "arch/x86/semantics/register_file.h"
#include "arch/x86/semantics/memory.h"
}


class x86_semantics_test : public ::testing::Test
{
protected:
    void SetUp() override;

    struct x86_register_file _register_file;
    struct x86_register_file* register_file = &_register_file;

    std::array<byte, 65536> _mem;
    struct memory _mem_struct;
    struct memory* mem;
};

#endif //EMULATOR_X86_SEMANTICS_TEST_H
