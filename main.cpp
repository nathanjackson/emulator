#include <cstdio>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/Utils/Cloning.h>

#include <capstone/capstone.h>

#include "x86/lifting/types.h"
#include "x86/lifting/register_file.h"

int main(int argc, char** argv)
{
    x86_register_file register_file;

    // Setup LLVM
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();

    llvm::LLVMContext context;

    llvm::SMDiagnostic err;
    auto semantics_module = llvm::parseIRFile("x86_semantics.bc", err, context);
    assert(semantics_module);

    llvm::StructType* operand_ty = nullptr;
    llvm::StructType* register_operand_ty = nullptr;
    llvm::StructType* immediate_operand_ty = nullptr;
    llvm::StructType* x86_register_file_ty = nullptr;

    for (auto st : semantics_module->getIdentifiedStructTypes()) {
        //llvm::outs() << st->getName() << "\n";
        if (st->getName() == "struct.immediate_operand") {
            immediate_operand_ty = st;
        } else if (st->getName() == "struct.register_operand") {
            register_operand_ty = st;
        } else if (st->getName() == "struct.x86_register_file") {
            x86_register_file_ty = st;
        } else if (st->getName() == "struct.operand") {
            operand_ty = st;
        }
    }
    assert(operand_ty);
    assert(register_operand_ty);
    assert(immediate_operand_ty);
    assert(x86_register_file_ty);

    auto make_u8_immediate_f = semantics_module->getFunction("make_u8_immediate");
    assert(make_u8_immediate_f);
    make_u8_immediate_f->setAttributes({});
    auto make_u16_immediate_f = semantics_module->getFunction("make_u16_immediate");
    assert(make_u16_immediate_f);
    make_u16_immediate_f->setAttributes({});
    auto make_register_operand_f = semantics_module->getFunction("make_register_operand");
    assert(make_register_operand_f);
    make_register_operand_f->setAttributes({});

    semantics_module->getFunction("byte_ptr")->setAttributes({});
    semantics_module->getFunction("word_ptr")->setAttributes({});
    semantics_module->getFunction("byte_ptr.2")->setAttributes({});
    semantics_module->getFunction("word_ptr.3")->setAttributes({});

    auto func_type = llvm::FunctionType::get(llvm::Type::getVoidTy(context), { x86_register_file_ty->getPointerTo() }, false);
    auto func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, "lifted", semantics_module.get());
    assert(func);
    auto entry_bb = llvm::BasicBlock::Create(context, "entry", func);
    assert(entry_bb);

    auto irb = std::make_unique<llvm::IRBuilder<>>(entry_bb);

    // Setup Capstone
    csh csh;
    if (CS_ERR_OK != cs_open(CS_ARCH_X86, CS_MODE_16, &csh)) {
        printf("error\n");
        return 1;
    }
    if (CS_ERR_OK != cs_option(csh, CS_OPT_DETAIL, CS_OPT_ON)) {
        printf("error 2\n");
        return 2;
    }

    // Lift
    std::vector<byte> code = { 0x83, 0xC0, 0x01  };
    cs_insn* insn = NULL;
    auto dr = cs_disasm(csh, code.data(), code.size(), 0x1000, 1, &insn);
    assert(1 == dr);

    auto register_file_arg = func->getArg(0);
    // setup instruction operands
    std::vector<llvm::Value*> operands;
    for (int i = 0; i < insn->detail->x86.op_count; ++i) {
        cs_x86_op& op = insn->detail->x86.operands[i];
        switch (op.type) {
        case X86_OP_IMM: {
            llvm::Value* op_ptr = irb->CreateAlloca(immediate_operand_ty);
            if (sizeof(byte) == op.size) {
                llvm::Value* imm_val = irb->getInt8(op.imm);
                irb->CreateCall(make_u8_immediate_f, { op_ptr, imm_val });
            } else if (sizeof(word) == op.size) {
                llvm::Value* imm_val = irb->getInt16(op.imm);
                irb->CreateCall(make_u16_immediate_f, { op_ptr, imm_val });
            }
            operands.push_back(irb->CreatePointerCast(op_ptr, operand_ty->getPointerTo()));
        } break;
        case X86_OP_REG: {
            llvm::Value* op_ptr = irb->CreateAlloca(register_operand_ty);
            auto cs_reg = irb->getInt32(op.reg);
            irb->CreateCall(make_register_operand_f, { op_ptr, register_file_arg, cs_reg });
            operands.push_back(irb->CreatePointerCast(op_ptr, operand_ty->getPointerTo()));
        } break;
        default:
            break;
        }
    }

    // insert instruction semantics
    auto x86_insn_add = semantics_module->getFunction("x86_insn_add");
    assert(x86_insn_add);
    x86_insn_add->setAttributes({});

    std::vector<llvm::CallInst*> insn_calls;

    switch (insn->id) {
    case X86_INS_ADD: {
        auto call_inst = irb->CreateCall(x86_insn_add, { register_file_arg, operands.at(0), operands.at(1) });
        insn_calls.push_back(call_inst);
    } break;
    }

    // force inlining of the instruction semantics
    irb->CreateRetVoid();
    for (auto ci : insn_calls) {
        llvm::InlineFunctionInfo ifi;
        llvm::InlineFunction(*ci, ifi);
    }

    // Optimization
    llvm::PassBuilder pass_builder;
    llvm::LoopAnalysisManager lam;
    llvm::FunctionAnalysisManager fam;
    llvm::CGSCCAnalysisManager cam;
    llvm::ModuleAnalysisManager mam;

    pass_builder.registerModuleAnalyses(mam);
    pass_builder.registerCGSCCAnalyses(cam);
    pass_builder.registerFunctionAnalyses(fam);
    pass_builder.registerLoopAnalyses(lam);
    pass_builder.crossRegisterProxies(lam, fam, cam, mam);

    llvm::ModulePassManager mpm = pass_builder.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O2);
    mpm.run(*semantics_module, mam);

    func->print(llvm::outs());
    //semantics_module->print(llvm::outs(), nullptr);

    // Setup ExecutionEngine
    std::string err_str = "";
    auto exec_engine = llvm::EngineBuilder(std::move(semantics_module))
            .setErrorStr(&err_str)
            .setVerifyModules(true)
            .create();

    void (*lifted)(x86_register_file*) = reinterpret_cast<void (*)(x86_register_file*)>(exec_engine->getFunctionAddress("lifted"));

    register_file.flags.af = 1;
    AX(&register_file) = 0x0E;

    lifted(&register_file);

    printf("AX=%x OF=%d SF=%d ZF=%d CF=%d PF=%d AF=%d\n", AX(&register_file), register_file.flags.of != 0, register_file.flags.sf != 0, register_file.flags.zf != 0, register_file.flags.cf != 0, register_file.flags.pf != 0, register_file.flags.af != 0);

    // lifting algorithm
    // ---------------------------------------------
    // for each instruction
    //     for each operand
    //         alloca operand struct
    //         call constructor
    //     insert call to instruction

    return 0;
}
