#pragma once

#include <cassert>

#include <string>
#include <string_view>
#include <map>

namespace enact {
    [[nodiscard]]
    constexpr const char* to_string(Opcode opcode) {
        switch (opcode) {
        case Opcode::Err: return "err";

        case Opcode::Add: return "add";
        case Opcode::Sub: return "sub";
        case Opcode::Mul: return "mul";
        case Opcode::Div: return "div";
        case Opcode::Mod: return "mod";
        case Opcode::Pow: return "pow";

        case Opcode::UAdd: return "uadd";
        case Opcode::USub: return "usub";
        case Opcode::UMul: return "umul";
        case Opcode::UDiv: return "udiv";
        case Opcode::UMod: return "umod";
        case Opcode::UPow: return "upow";

        case Opcode::Shl: return "shl";
        case Opcode::Shr: return "shr";

        case Opcode::Inc: return "inc";
        case Opcode::Dec: return "dec";

        case Opcode::Cmp: return "cmp";
        case Opcode::Neg: return "neg";

        case Opcode::And: return "and";
        case Opcode::Or: return "or";
        case Opcode::Not: return "not";
        case Opcode::Xor: return "xor";

        case Opcode::Equ: return "equ";
        case Opcode::Neq: return "neq";
        case Opcode::Gtr: return "gtr";
        case Opcode::Gte: return "gte";
        case Opcode::Lst: return "lst";
        case Opcode::Lte: return "lte";

        case Opcode::Jmp: return "jmp";
        case Opcode::Jt: return "jt";
        case Opcode::Jf: return "jf";
        case Opcode::Jz: return "jz";
        case Opcode::Jnz: return "jnz";
        case Opcode::Je: return "je";
        case Opcode::Jne: return "jne";
        case Opcode::Jl: return "jl";
        case Opcode::Jnl: return "jnl";
        case Opcode::Jle: return "jle";
        case Opcode::Jnle: return "jnle";
        case Opcode::Jg: return "jg";
        case Opcode::Jng: return "jng";
        case Opcode::Jge: return "jge";
        case Opcode::Jnge: return "jnge";

        case Opcode::Xchg: return "xchg";
        case Opcode::IConst: return "iconst";
        case Opcode::Pop: return "pop";
        case Opcode::ILoad: return "iload";
        case Opcode::IStore: return "istore";

        case Opcode::Int: return "int";
        case Opcode::Nop: return "nop";
        case Opcode::Lea: return "lea";
        case Opcode::Rand: return "rand";

        case Opcode::Puts: return "puts";
        case Opcode::Putc: return "putc";

        case Opcode::Dup: return "dup";
        case Opcode::Dump: return "dump";
        case Opcode::Halt: return "halt";
        default:
            assert(0);
            return "<unknown-opcode>";
        }
    }

    #define OPCODE_STRING_ENTRY(opcode) \
        {to_string(Opcode::opcode), Opcode::opcode}

    std::map<std::string_view, Opcode> opcode_strings {
        OPCODE_STRING_ENTRY(Err),

        OPCODE_STRING_ENTRY(Add),
        OPCODE_STRING_ENTRY(Sub),
        OPCODE_STRING_ENTRY(Mul),
        OPCODE_STRING_ENTRY(Div),
        OPCODE_STRING_ENTRY(Mod),
        OPCODE_STRING_ENTRY(Pow),

        OPCODE_STRING_ENTRY(UAdd),
        OPCODE_STRING_ENTRY(USub),
        OPCODE_STRING_ENTRY(UMul),
        OPCODE_STRING_ENTRY(UDiv),
        OPCODE_STRING_ENTRY(UMod),
        OPCODE_STRING_ENTRY(UPow),

        OPCODE_STRING_ENTRY(Shl),
        OPCODE_STRING_ENTRY(Shr),

        OPCODE_STRING_ENTRY(Inc),
        OPCODE_STRING_ENTRY(Dec),

        OPCODE_STRING_ENTRY(Cmp),
        OPCODE_STRING_ENTRY(Neg),

        OPCODE_STRING_ENTRY(And),
        OPCODE_STRING_ENTRY(Or),
        OPCODE_STRING_ENTRY(Not),
        OPCODE_STRING_ENTRY(Xor),

        OPCODE_STRING_ENTRY(Equ),
        OPCODE_STRING_ENTRY(Neq),
        OPCODE_STRING_ENTRY(Gtr),
        OPCODE_STRING_ENTRY(Gte),
        OPCODE_STRING_ENTRY(Lst),
        OPCODE_STRING_ENTRY(Lte),

        OPCODE_STRING_ENTRY(Jmp),
        OPCODE_STRING_ENTRY(Jt),
        OPCODE_STRING_ENTRY(Jf),
        OPCODE_STRING_ENTRY(Jz),
        OPCODE_STRING_ENTRY(Jnz),
        OPCODE_STRING_ENTRY(Je),
        OPCODE_STRING_ENTRY(Jne),
        OPCODE_STRING_ENTRY(Jl),
        OPCODE_STRING_ENTRY(Jnl),
        OPCODE_STRING_ENTRY(Jle),
        OPCODE_STRING_ENTRY(Jnle),
        OPCODE_STRING_ENTRY(Jg),
        OPCODE_STRING_ENTRY(Jng),
        OPCODE_STRING_ENTRY(Jge),
        OPCODE_STRING_ENTRY(Jnge),

        OPCODE_STRING_ENTRY(Xchg),
        OPCODE_STRING_ENTRY(IConst),
        OPCODE_STRING_ENTRY(Pop),
        OPCODE_STRING_ENTRY(ILoad),
        OPCODE_STRING_ENTRY(IStore),

        OPCODE_STRING_ENTRY(Int),
        OPCODE_STRING_ENTRY(Nop),
        OPCODE_STRING_ENTRY(Lea),
        OPCODE_STRING_ENTRY(Rand),

        OPCODE_STRING_ENTRY(Puts),
        OPCODE_STRING_ENTRY(Putc),

        OPCODE_STRING_ENTRY(Dup), 
        OPCODE_STRING_ENTRY(Dump), 

        OPCODE_STRING_ENTRY(Halt),
    };

    [[nodiscard]]
    Opcode to_opcode(std::string_view data) {
        if (auto temp = opcode_strings.find(data); 
            temp != opcode_strings.end()
        ) {
            return temp->second;
        }
        return Opcode::Err;
    }
}
