//
// Created by nathan on 11/5/22.
//

#ifndef EMULATOR_X86_SEMANTICS_TEST_H
#define EMULATOR_X86_SEMANTICS_TEST_H

#include <array>

#include <gtest/gtest.h>

#include "address_space.h"
#include "ram.h"

extern "C"
{
#include "arch/x86/semantics/register_file.h"
}


class x86_semantics_test : public ::testing::Test
{
protected:
    void SetUp() override;

    struct x86_register_file _register_file;
    struct x86_register_file* register_file = &_register_file;

    struct address_space as;
    struct ram ram;
};

#endif //EMULATOR_X86_SEMANTICS_TEST_H
