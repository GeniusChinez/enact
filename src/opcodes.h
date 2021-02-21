#pragma once

namespace enact {
    enum class Opcode {
        Err,

        // arithmetic
        Add,
        Sub,
        Mul,
        Div,
        Mod,
        Pow,

        UAdd,
        USub,
        UMul,
        UDiv,
        UMod,
        UPow,

        Shl,
        Shr,

        Inc,
        Dec,

        Cmp,

        // logical
        And,
        Or,
        Not,
        Xor,

        // relational
        Equ,
        Neq,
        Gtr,
        Gte,
        Lst,
        Lte,

        // control flow
        Jmp,
        Jt,
        Jf,
        Jz,
        Jnz,
        Je,
        Jne,
        Jl,
        Jnl,
        Jle,
        Jnle,
        Jg,
        Jng,
        Jge,
        Jnge,

        // data transfer
        Xchg,
        IConst,
        Pop,
        ILoad,
        IStore,

        // miscellaneous
        Int,
        Nop,
        Lea,
        Rand,

        Puts,
        Putc,

        Dup, 
        Dump, 

        Halt,
    };
}
