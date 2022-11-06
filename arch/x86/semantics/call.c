//
// Created by nathan on 11/5/22.
//

#include "register_operand.h"
#include "memory.h"

void x86_insn_call_near(struct x86_register_file* register_file, struct memory* memory, struct operand* target)
{
    SP(register_file) -= sizeof(SP(register_file));

    word* rap = get_host_ptr(memory, SS(register_file), SP(register_file));
    *rap = IP(register_file);

    IP(register_file) = WORD(target);
}

void x86_insn_call_far(struct x86_register_file* register_file, struct memory* memory, struct operand* segment, struct operand* target)
{
    SP(register_file) -= sizeof(SP(register_file));
    word* rsp = get_host_ptr(memory, SS(register_file), SP(register_file));
    *rsp = CS(register_file);

    SP(register_file) -= sizeof(SP(register_file));
    word* rap = get_host_ptr(memory, SS(register_file), SP(register_file));
    *rap = IP(register_file);

    CS(register_file) = WORD(segment);
    IP(register_file) = WORD(target);
}