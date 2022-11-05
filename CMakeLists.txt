cmake_minimum_required(VERSION 3.9.4)
project(emulator)

add_library(x86_semantics_bitcode OBJECT x86/lifting/immediate_operand.c x86/lifting/register_operand.c x86/lifting/add.c x86/lifting/memory_operand.c)
target_compile_options(x86_semantics_bitcode PUBLIC -flto)
add_custom_target(x86_semantics.bc ALL
        llvm-link -o x86_semantics.bc $<TARGET_OBJECTS:x86_semantics_bitcode>
        COMMAND_EXPAND_LISTS
        DEPENDS x86_semantics_bitcode)

add_executable(emulator main.cpp)
target_link_libraries(emulator -lLLVM-14 -lcapstone)
add_dependencies(emulator x86_semantics.bc)
