//
// Created by nathan on 11/13/22.
//

#include <iostream>
#include <sstream>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include "lifter.h"

lifter::lifter(llvm::Module* module,
               x86_register_file* register_file, struct memory* guest_memory) :
        register_file(register_file), guest_memory(guest_memory), module(module)
{
    // Clear attributes to enable optimization.
    for (auto& func : module->getFunctionList()) {
        func.setAttributes({});
    }

    if (CS_ERR_OK != cs_open(CS_ARCH_X86, CS_MODE_16, &capstone_handle)) {
        std::cerr << "Could not open capstone.\n";
        abort();
    }

    if (CS_ERR_OK != cs_option(capstone_handle, CS_OPT_DETAIL, CS_OPT_ON)) {
        std::cerr << "Could not configure capstone handle.\n";
        abort();
    }

    make_u8_immediate_f = module->getFunction("make_u8_immediate");
    assert(make_u8_immediate_f);
    make_u16_immediate_f = module->getFunction("make_u16_immediate");
    assert(make_u16_immediate_f);
    make_register_operand_f = module->getFunction("make_register_operand");
    assert(make_register_operand_f);
    make_memory_operand_direct_f = module->getFunction("make_memory_operand_direct");
    assert(make_memory_operand_direct_f);


    // lookup structs
    for (auto st : module->getIdentifiedStructTypes()) {
        if (st->getName() == "struct.immediate_operand") {
            immediate_operand_ty = st;
        } else if (st->getName() == "struct.register_operand") {
            register_operand_ty = st;
        } else if (st->getName() == "struct.x86_register_file") {
            x86_register_file_ty = st;
        } else if (st->getName() == "struct.operand") {
            operand_ty = st;
        } else if (st->getName() == "struct.memory") {
            memory_ty = st;
        } else if (st->getName() == "struct.memory_operand") {
            memory_operand_ty = st;
        }
    }
    assert(operand_ty);
    assert(register_operand_ty);
    assert(immediate_operand_ty);
    assert(x86_register_file_ty);
    assert(memory_ty);
    assert(memory_operand_ty);
}

lifter::~lifter()
{
    cs_close(&capstone_handle);
}

llvm::Function* lifter::lift(word segment, word addr)
{

    // first, read a few bytes from the guest memory
    std::array<byte, 15> buffer;
    byte* host_ptr = reinterpret_cast<byte*>(get_host_ptr(guest_memory, segment, addr));
    std::copy(host_ptr, host_ptr + buffer.size(), buffer.begin());

    // create a new function for the block we're going to lift
    auto func_type = llvm::FunctionType::get(llvm::Type::getVoidTy(module->getContext()), { x86_register_file_ty->getPointerTo(), memory_ty->getPointerTo() }, false);
    std::stringstream ss;
    ss << std::hex << segment << "_" << addr << "_lifted";
    auto func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, ss.str(), module);
    assert(func);
    auto entry_bb = llvm::BasicBlock::Create(module->getContext(), "entry", func);
    assert(entry_bb);

    // Setup the IR Builder
    auto irb = std::make_unique<llvm::IRBuilder<>>(entry_bb);

    // TODO: Handle multiple instructions

    // disassemble an instruction
    std::unique_ptr<cs_insn, void (*)(cs_insn*)> insn(cs_malloc(capstone_handle), [](cs_insn* ptr) {
        cs_free(ptr, 1);
    });
    const byte* code_ptr = buffer.data();
    uint64_t code_size = buffer.size();
    uint64_t addrl = addr;
    if (!cs_disasm_iter(capstone_handle, &code_ptr, &code_size, &addrl, insn.get())) {
        std::cerr << "Failed to disassemble!\n";
        abort();
    }

    // Get Values for the register file and memory structures.
    auto register_file_arg = func->getArg(0);
    auto memory_arg = func->getArg(1);

    // Construct operands
    std::vector<llvm::Value*> operands;
    for (int i = 0; i < insn->detail->x86.op_count; i++) {
        cs_x86_op& op = insn->detail->x86.operands[i];
        switch (op.type) {
        case X86_OP_IMM: {
            llvm::Value* op_ptr = irb->CreateAlloca(immediate_operand_ty);
            llvm::Value* dyn_ptr = nullptr;
            if (sizeof(byte) == op.size) {
                llvm::Value* imm_val = irb->getInt8(op.imm);
                dyn_ptr = irb->CreateCall(make_u8_immediate_f, { op_ptr, imm_val });
            } else if (sizeof(word) == op.size) {
                llvm::Value* imm_val = irb->getInt16(op.imm);
                dyn_ptr = irb->CreateCall(make_u16_immediate_f, { op_ptr, imm_val });
            }
            operands.push_back(dyn_ptr);
        } break;
        case X86_OP_REG: {
            llvm::Value* op_ptr = irb->CreateAlloca(register_operand_ty);
            auto cs_reg = irb->getInt32(op.reg);
            irb->CreateCall(make_register_operand_f, { op_ptr, register_file_arg, cs_reg });
            operands.push_back(irb->CreatePointerCast(op_ptr, operand_ty->getPointerTo()));
        } break;
        case X86_OP_MEM: {
            llvm::Value* op_ptr = irb->CreateAlloca(memory_operand_ty);

            auto size_value = irb->getInt64(op.size);

            x86_reg segment = X86_REG_DS;
            if (X86_REG_INVALID != op.mem.segment) {
                segment = op.mem.segment;
            }
            auto segment_offset_value = irb->getInt64(segment_offset(segment));

            auto disp_value = irb->getInt16(op.mem.disp);

            if (X86_REG_INVALID == op.mem.base && X86_REG_INVALID == op.mem.index) {
                irb->CreateCall(make_memory_operand_direct_f, { op_ptr, memory_arg, register_file_arg, size_value, segment_offset_value, disp_value });
            } else {
                abort();
            }
            operands.push_back(irb->CreatePointerCast(op_ptr, operand_ty->getPointerTo()));
        } break;
        default:
            break;
        }
    }

    // Insert instruction call

    std::vector<llvm::CallInst*> insn_calls;
    switch (insn->id) {
     case X86_INS_CLD: {
        auto f = module->getFunction("x86_insn_cld");
        assert(f);
        auto ci = irb->CreateCall(f, { register_file_arg });
        insn_calls.push_back(ci);
    } break;
    case X86_INS_CLI: {
        auto f = module->getFunction("x86_insn_cli");
        assert(f);
        auto ci = irb->CreateCall(f, { register_file_arg });
        insn_calls.push_back(ci);
    } break;
    case X86_INS_JMP: {
        auto f = module->getFunction("x86_insn_jmp");
        assert(f);
        auto ci = irb->CreateCall(f, { register_file_arg, operands.at(0) });
        insn_calls.push_back(ci);
    } break;
    case X86_INS_XOR: {
        auto f = module->getFunction("x86_insn_xor");
        assert(f);
        auto ci = irb->CreateCall(f, { register_file_arg, operands.at(0), operands.at(1) });
        insn_calls.push_back(ci);
    } break;
    default: {
        std::cerr << "Instruction not yet supported\n";
        abort();
    } break;
    }

    if (!cs_insn_group(capstone_handle, insn.get(), X86_GRP_JUMP)) {
        auto reg_val = irb->getInt32(X86_REG_IP);
        auto get_reg_val_f = module->getFunction("get_register_value");
        auto set_reg_val_f = module->getFunction("set_register_value_word");
        auto insn_size = irb->getInt16(insn->size);

        auto pc_val = irb->CreateCall(get_reg_val_f, { register_file_arg, reg_val });
        insn_calls.push_back(pc_val);
        auto new_pc_val = irb->CreateCall(set_reg_val_f, { register_file_arg, reg_val, irb->CreateAdd(pc_val, insn_size) });
        insn_calls.push_back(new_pc_val);
    }

    // terminate the guest code block
    irb->CreateRetVoid();

//    // inline calls to instructions
//    for (auto ci : insn_calls) {
//        llvm::InlineFunctionInfo ifi;
//        llvm::InlineFunction(*ci, ifi);
//    }
//
//    // Optimization
//    llvm::PassBuilder pass_builder;
//    llvm::LoopAnalysisManager lam;
//    llvm::FunctionAnalysisManager fam;
//    llvm::CGSCCAnalysisManager cam;
//    llvm::ModuleAnalysisManager mam;
//
//    pass_builder.registerModuleAnalyses(mam);
//    pass_builder.registerCGSCCAnalyses(cam);
//    pass_builder.registerFunctionAnalyses(fam);
//    pass_builder.registerLoopAnalyses(lam);
//    pass_builder.crossRegisterProxies(lam, fam, cam, mam);
//
//    llvm::ModulePassManager mpm = pass_builder.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O2);
//    mpm.run(*module, mam);

    //module->print(llvm::outs(), NULL);

    func->print(llvm::outs());

    return func;
}