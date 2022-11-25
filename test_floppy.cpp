//
// Created by nathan on 11/6/22.
//

#include <array>
#include <iostream>
#include <fstream>
#include <vector>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetSelect.h>

#include <capstone/capstone.h>

#include "arch/x86/lifter.h"
#include "address_space.h"
#include "ram.h"

extern "C"
{
#include "arch/x86/semantics/register_file.h"
#include "arch/x86/semantics/immediate_operand.h"
#include "arch/x86/semantics/register_operand.h"
#include "arch/x86/semantics/memory_operand.h"
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "usage: " << argv[0] << " FLOPPY-IMAGE\n";
        return 1;
    }

    // Setup LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();

    // Setup System
    struct x86_register_file register_file;
    CS(&register_file) = 0x0;
    IP(&register_file) = 0x7C00;

    struct address_space memory_as;
    address_space_initialize(&memory_as, 0xFFFFFFFF);

    struct ram ram;
    ram_initialize(&ram, 0xA0000);
    ram_map_conventional_memory(&ram, &memory_as);

    // Load boot sector into memory at 0000:7C00
    std::ifstream floppy;
    floppy.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    floppy.open(argv[1], std::ios::binary);
    uint8_t* dest = address_space_get_host_ptr_segmented(&memory_as, 0x0, 0x7c00);
    floppy.read((char*)dest, 512);

    for (int i = 0; i < 9; i++) {
        llvm::LLVMContext llvm_context;

        llvm::SMDiagnostic err;
        auto semantics_module = llvm::parseIRFile("x86_semantics.bc", err, llvm_context);
        assert(semantics_module);

        // Setup Lifter
        lifter lifter(semantics_module.get(), &register_file, &memory_as);

        // Setup Execution Engine
        std::string err_str = "";
        std::unique_ptr<llvm::ExecutionEngine> exec_engine(llvm::EngineBuilder(std::move(semantics_module)).setErrorStr(&err_str).setVerifyModules(true).create());

        // lift block
        auto func = lifter.lift(CS(&register_file), IP(&register_file));
        assert(func);

        // code gen
        auto guest_block = reinterpret_cast<void(*)(struct x86_register_file*, struct address_space*)>(exec_engine->getFunctionAddress(func->getName().str()));
        assert(guest_block);

        std::cout << "CS=0x" << std::hex << CS(&register_file) << " IP=0x" << IP(&register_file) << "\n";
        // run it!
        guest_block(&register_file, &memory_as);
    }

    std::cout << "CS=0x" << std::hex << CS(&register_file) << " IP=0x" << IP(&register_file) << "\n";
    std::cout << "IF=" << IF(&register_file) << "\n";
    std::cout << "AX=" << AX(&register_file) << "\n";
    std::cout << "SS=" << SS(&register_file) << "\n";
    std::cout << "SP=" << SP(&register_file) << "\n";
    std::cout << "BX=" << BX(&register_file) << "\n";
    std::cout << "SI=" << SI(&register_file) << "\n";
    std::cout << "DS=" << DS(&register_file) << "\n";

    return 0;
}