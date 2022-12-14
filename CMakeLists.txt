cmake_minimum_required(VERSION 3.9.4)
project(emulator)

set(SEMANTICS_SOURCES
    arch/x86/semantics/immediate_operand.c
    arch/x86/semantics/register_operand.c
    arch/x86/semantics/memory_operand.c
    arch/x86/semantics/register_file.c
    arch/x86/semantics/adc.c
    arch/x86/semantics/add.c
    arch/x86/semantics/aaa.c
    arch/x86/semantics/aad.c
    arch/x86/semantics/aam.c
    arch/x86/semantics/aas.c
    arch/x86/semantics/and.c
    arch/x86/semantics/call.c
    arch/x86/semantics/cbw.c
    arch/x86/semantics/clc.c
    arch/x86/semantics/cld.c
    arch/x86/semantics/cli.c
    arch/x86/semantics/cmc.c
    arch/x86/semantics/cmp.c
    arch/x86/semantics/cwd.c
    arch/x86/semantics/daa.c
    arch/x86/semantics/das.c arch/x86/semantics/dec.c arch/x86/semantics/imul.c arch/x86/semantics/inc.c arch/x86/semantics/jcc.c arch/x86/semantics/jmp.c arch/x86/semantics/xor.c arch/x86/semantics/mov.c arch/x86/semantics/push.c arch/x86/semantics/pop.c arch/x86/semantics/lds.c

    address_space.c utils/avl_tree.c ram.c
        )

add_library(x86_semantics_bitcode OBJECT ${SEMANTICS_SOURCES})
target_compile_options(x86_semantics_bitcode PUBLIC -flto)
target_include_directories(x86_semantics_bitcode PUBLIC ${CMAKE_SOURCE_DIR})
add_custom_target(x86_semantics.bc ALL
        llvm-link -o x86_semantics.bc $<TARGET_OBJECTS:x86_semantics_bitcode>
        COMMAND_EXPAND_LISTS
        DEPENDS x86_semantics_bitcode)

add_executable(emulator test_floppy.cpp arch/x86/lifter.cpp arch/x86/semantics/register_file.c address_space.c ram.c utils/avl_tree.c arch/x86/bios.cpp)
target_link_libraries(emulator -lcapstone -lLLVM-14)
add_dependencies(emulator x86_semantics.bc)
target_include_directories(emulator PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(test_x86_semantics
    tests/arch/x86/semantics/x86_semantics_test.cpp
    tests/arch/x86/semantics/test_aaa.cpp
    tests/arch/x86/semantics/test_add.cpp
    tests/arch/x86/semantics/test_aad.cpp
    tests/arch/x86/semantics/test_aam.cpp
    tests/arch/x86/semantics/test_aas.cpp
    tests/arch/x86/semantics/test_adc.cpp
    tests/arch/x86/semantics/test_and.cpp
    tests/arch/x86/semantics/test_call.cpp
    tests/arch/x86/semantics/test_cbw.cpp
    tests/arch/x86/semantics/test_clc.cpp
    tests/arch/x86/semantics/test_cld.cpp
    tests/arch/x86/semantics/test_cli.cpp
    tests/arch/x86/semantics/test_cmc.cpp
    tests/arch/x86/semantics/test_cmp.cpp
    tests/arch/x86/semantics/test_cwd.cpp
    tests/arch/x86/semantics/test_daa.cpp
    tests/arch/x86/semantics/test_das.cpp
    tests/arch/x86/semantics/test_dec.cpp
    ${SEMANTICS_SOURCES} tests/arch/x86/semantics/test_imul.cpp tests/arch/x86/semantics/test_inc.cpp tests/arch/x86/semantics/test_jcc.cpp tests/arch/x86/semantics/test_jmp.cpp tests/arch/x86/semantics/test_xor.cpp tests/arch/x86/semantics/test_mov.cpp tests/arch/x86/semantics/test_push.cpp tests/arch/x86/semantics/test_pop.cpp tests/arch/x86/semantics/test_lds.cpp)
target_link_libraries(test_x86_semantics -lgtest -lgtest_main)
target_include_directories(test_x86_semantics PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(test_utils
        tests/utils/test_avl_tree.cpp utils/avl_tree.c)
target_link_libraries(test_utils -lgtest -lgtest_main)
target_include_directories(test_utils PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(test_address_space
        tests/test_address_space.cpp utils/avl_tree.c address_space.c)
target_link_libraries(test_address_space -lgtest -lgtest_main)
target_include_directories(test_address_space PUBLIC ${CMAKE_SOURCE_DIR})

add_executable(test_ram
        tests/test_ram.cpp utils/avl_tree.c address_space.c ram.c)
target_link_libraries(test_ram -lgtest -lgtest_main)
target_include_directories(test_ram PUBLIC ${CMAKE_SOURCE_DIR})