cmake_minimum_required(VERSION 3.9.4)
project(emulator)

set(SEMANTICS_SOURCES
    arch/x86/semantics/immediate_operand.c
    arch/x86/semantics/register_operand.c
    arch/x86/semantics/memory_operand.c
    arch/x86/semantics/adc.c
    arch/x86/semantics/add.c
    arch/x86/semantics/aaa.c
    arch/x86/semantics/aad.c arch/x86/semantics/aam.c arch/x86/semantics/aas.c)

add_library(x86_semantics_bitcode OBJECT ${SEMANTICS_SOURCES})
target_compile_options(x86_semantics_bitcode PUBLIC -flto)
add_custom_target(x86_semantics.bc ALL
        llvm-link -o x86_semantics.bc $<TARGET_OBJECTS:x86_semantics_bitcode>
        COMMAND_EXPAND_LISTS
        DEPENDS x86_semantics_bitcode)

add_executable(emulator main.cpp)
target_link_libraries(emulator -lLLVM-14 -lcapstone)
add_dependencies(emulator x86_semantics.bc)

add_executable(test_x86_semantics
    tests/arch/x86/semantics/x86_semantics_test.cpp
    tests/arch/x86/semantics/test_aaa.cpp
    tests/arch/x86/semantics/test_add.cpp
    tests/arch/x86/semantics/test_aad.cpp
    ${SEMANTICS_SOURCES} tests/arch/x86/semantics/test_aam.cpp tests/arch/x86/semantics/test_aas.cpp tests/arch/x86/semantics/test_adc.cpp)
target_link_libraries(test_x86_semantics -lgtest -lgtest_main)
target_include_directories(test_x86_semantics PUBLIC ${CMAKE_SOURCE_DIR})