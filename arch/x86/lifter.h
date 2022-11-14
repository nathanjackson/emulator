//
// Created by nathan on 11/13/22.
//

#ifndef EMULATOR_LIFTER_H
#define EMULATOR_LIFTER_H

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

#include <capstone/capstone.h>

extern "C"
{
#include "semantics/memory.h"
#include "semantics/register_file.h"
};

class lifter
{
public:
    lifter(llvm::LLVMContext&, x86_register_file* register_file, struct memory* guest_memory);
    ~lifter();

    llvm::Function* lift(word segment, word addr);

private:
    struct x86_register_file* register_file;
    struct memory* guest_memory;

    llvm::LLVMContext& llvm_context;
    std::unique_ptr<llvm::Module> semantics_module;

    llvm::StructType* operand_ty;
    llvm::StructType* register_operand_ty;
    llvm::StructType* immediate_operand_ty;
    llvm::StructType* x86_register_file_ty;
    llvm::StructType* memory_ty;
    llvm::StructType* memory_operand_ty;

    llvm::Function* make_u8_immediate_f;
    llvm::Function* make_u16_immediate_f;
    llvm::Function* make_register_operand_f;
    llvm::Function* make_memory_operand_direct_f;

    csh capstone_handle;
};


#endif //EMULATOR_LIFTER_H
