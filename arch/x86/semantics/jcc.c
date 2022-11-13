//
// Created by nathan on 11/13/22.
//

#include "operand.h"
#include "register_file.h"

#define JCC(mnemonic, cond) \
    void x86_insn_##mnemonic(struct x86_register_file* register_file, struct operand* tgt, struct operand* next) \
    {                                       \
        if ((cond)) {                       \
            IP(register_file) = WORD(tgt);  \
        } else {                            \
            IP(register_file) = WORD(next); \
        }                                   \
    }

JCC(ja, !(CF(register_file) || ZF(register_file)))
JCC(jae, !(CF(register_file)))
JCC(jb, CF(register_file))
JCC(jbe, CF(register_file) || ZF(register_file))
JCC(jcxz, 0 == CX(register_file))
JCC(je, ZF(register_file))
JCC(jg, (!ZF(register_file)) && ((SF(register_file) != 0) == (OF(register_file) != 0)))
JCC(jge, (SF(register_file) != 0) == (OF(register_file) != 0))
JCC(jl, (SF(register_file) != 0) != (OF(register_file) != 0))
JCC(jle, ((SF(register_file) != 0) != (OF(register_file) != 0)) || ZF(register_file))
JCC(jne, !ZF(register_file))
JCC(jno, !OF(register_file))
JCC(jnp, !PF(register_file))
JCC(jns, !SF(register_file))
JCC(jo, OF(register_file))
JCC(jp, PF(register_file))
JCC(js, SF(register_file))
