//
// Created by nathan on 11/5/22.
//

#ifndef EMULATOR_X86_SEMANTICS_TEST_H
#define EMULATOR_X86_SEMANTICS_TEST_H

#include "arch/x86/semantics/register_file.h"

#include <gtest/gtest.h>

class x86_semantics_test : public ::testing::Test
{
protected:
    struct x86_register_file _register_file;
    struct x86_register_file* register_file = &_register_file;
};

#endif //EMULATOR_X86_SEMANTICS_TEST_H
