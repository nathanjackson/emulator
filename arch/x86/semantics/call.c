//
// Created by nathan on 11/5/22.
//

#include "address_space.h"

#include "register_operand.h"

void x86_insn_call_near(struct x86_register_file* register_file, struct address_space* as, struct operand* target)
{
    SP(register_file) -= sizeof(SP(register_file));

    word rap = IP(register_file);
    address_space_access_segmented(as, 1, SS(register_file), SP(register_file), sizeof(rap), &rap);

    IP(register_file) = WORD(target);
}

void x86_insn_call_far(struct x86_register_file* register_file, struct address_space* as, struct operand* segment, struct operand* target)
{
    SP(register_file) -= sizeof(SP(register_file));
    word seg = CS(register_file);
    address_space_access_segmented(as, 1, SS(register_file), SP(register_file), sizeof(seg), &seg);

    SP(register_file) -= sizeof(SP(register_file));
    word rap = IP(register_file);
    address_space_access_segmented(as, 1, SS(register_file), SP(register_file), sizeof(seg), &rap);

    CS(register_file) = WORD(segment);
    IP(register_file) = WORD(target);
}