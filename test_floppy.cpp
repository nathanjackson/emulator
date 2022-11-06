//
// Created by nathan on 11/6/22.
//

#include <array>
#include <iostream>
#include <fstream>
#include <vector>

#include <capstone/capstone.h>

extern "C"
{
#include "arch/x86/semantics/memory.h"
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

    // Setup System
    struct x86_register_file register_file;
    CS(&register_file) = 0x0;
    IP(&register_file) = 0x7C00;
    struct std::array<uint8_t, UINT16_MAX> mem;
    struct memory memory = {
            .ptr = mem.data(),
    };

    // Load boot sector into memory at 0000:7C00
    std::ifstream floppy;
    floppy.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    floppy.open(argv[1], std::ios::binary);
    uint8_t* dest = (reinterpret_cast<uint8_t*>(memory.ptr) + 0x7C00);
    floppy.read((char*)dest, 512);

    // Setup Capstone
    csh handle = 0;
    if (CS_ERR_OK != cs_open(CS_ARCH_X86, CS_MODE_16, &handle)) {
        std::cerr << "Could not create capstone handle.\n";
        return 2;
    }
    if (CS_ERR_OK != cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON)) {
        std::cerr << "Could not configure capstone.\n";
        return 3;
    }

    // main loop
    bool run = true;
    cs_insn *insn = cs_malloc(handle);
    while (run) {
        const uint8_t* ptr = reinterpret_cast<uint8_t*>(get_host_ptr(&memory, CS(&register_file), IP(&register_file)));
        uint64_t addr = IP(&register_file);
        uint64_t size = 15;
        bool ok = cs_disasm_iter(handle, &ptr, &size, &addr, insn);

        if (!ok) {
            run = false;
            break;
        }

        // adapt operands
        std::vector<struct operand*> operands;
        for (int i = 0; i < insn->detail->x86.op_count; ++i) {
            cs_x86_op& op = insn->detail->x86.operands[i];
            struct operand* emuop = NULL;
            switch (op.type) {
            case X86_OP_IMM: {
                struct immediate_operand* imm = new struct immediate_operand;

                if (sizeof(byte) == op.size) {
                    emuop = make_u8_immediate(imm, op.imm);
                } else if (sizeof(word) == op.size) {
                    emuop = make_u16_immediate(imm, op.imm);
                }
                } break;
                case X86_OP_REG: {
                    struct register_operand* reg = new struct register_operand;
                    emuop = make_register_operand(reg, &register_file, op.reg);
                } break;
                default: {
                    run = false;
                }break;
            }
            operands.push_back(emuop);


        }

        switch (insn->id) {
        default: {
            std::cerr << "Instruction not yet implemented\n";
            run = false;
        } break;
        }

        // cleanup
        for (auto emuop : operands) {
            delete emuop;
        }
    }

    cs_free(insn, 1);
    cs_close(&handle);

    return 0;
}