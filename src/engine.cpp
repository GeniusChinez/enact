#pragma once

#include "engine.h"
#include "opcodes.cpp"
#include "location.h"

#include <cmath>
#include <climits>

#include <iostream>

namespace enact {
    template <typename T, typename S>
    T Engine::get_item_at(const S* s) const {
        return *(const T*)s;
    }

    std::size_t Engine::get_number_of_errors() const {
        return issues.number_of_errors;
    }

    std::size_t Engine::get_number_of_warnings() const {
        return issues.number_of_warnings;
    }

    void Engine::increment_number_of_errors() {
        issues.number_of_errors++;
    }

    void Engine::increment_number_of_warnings() {
        issues.number_of_warnings++;
    }

    template <typename T>
    bool Engine::print_location_if_possible(const T& data) const {
        if constexpr(std::is_same_v<std::decay_t<T>, Location>) {
            std::cout << data.format();
        }
        else if constexpr(std::is_same_v<std::decay_t<T>, Location*>) {
            std::cout << data->format();
        }
        else {
            return false;
        }
        return true;
    }

    void Engine::print_issue_tail() const {
        std::cout << '\n';
    }

    template <typename ...Args>
    void Engine::print_issue_tail(Args&&... args) const {
        (std::cout << ... << args);
        std::cout << '\n';
    }

    template <typename T, typename ...Args> 
    void Engine::report_warning(const T& arg1, Args&&... args) {
        if (print_location_if_possible(arg1)) {
            std::cout << ": warning: ";
        }
        else {
            std::cout << "warning: " << arg1;
        }

        print_issue_tail(std::forward<Args>(args)...);
        increment_number_of_warnings();
    }

    template <typename T, typename ...Args> 
    void Engine::report_error(const T& arg1, Args&&... args) {
        if (print_location_if_possible(arg1)) {
            std::cout << ": error: ";
        }
        else {
            std::cout << "error: " << arg1;
        }

        print_issue_tail(std::forward<Args>(args)...);
        increment_number_of_errors();
    }

    template <typename T, typename ...Args> 
    void Engine::report_fatal_error(const T& arg1, Args&&... args) {
        report_error(arg1, std::forward<Args>(args)...);
        exit(get_number_of_errors());
    }

    template <typename IntegerT>
    IntegerT Engine::get_integer_constant(std::size_t offset) const 
        requires std::is_integral_v<IntegerT>
    {
        return get_item_at<IntegerT>(get_constants() + offset);
    }

    const std::uint8_t* Engine::get_string_constant(std::size_t offset) const {
        return (const std::uint8_t*)(get_constants() + offset);
    }

    template <typename IntegerT=uint64_t>
    IntegerT Engine::get_heap_item(std::size_t offset) const
        requires std::is_integral_v<IntegerT>
    {
        return get_item_at<IntegerT>(get_heap_data() + offset);
    }
    
    template <typename IntegerT=uint64_t>
    void Engine::put_heap_item(std::size_t offset, IntegerT value) 
        requires std::is_integral_v<IntegerT>
    {
        *(IntegerT*)(get_heap_data() + offset) = value;
    }

    void Engine::extend_heap(std::size_t factor) {
        auto temp = std::realloc(get_heap_data(), get_heap_size() + factor);

        if (!temp) {
            throw std::bad_alloc();
        }

        runtime_data.heap.data = static_cast<decltype(runtime_data.heap.data)>(
            temp
        );

        get_heap_size() += factor;
    }

    void Engine::free_heap() {
        if (!get_heap_data()) {
            return;
        }

        std::free(get_heap_data());
        runtime_data.heap.data = NULL;
    }

    void Engine::initialize_code(const std::uint8_t* code) {
        runtime_data.code.data = code;
        update_code_offset(0);
    }

    void Engine::initialize_constants(const uint8_t* constants) {
        runtime_data.constants = constants;
    }

    void Engine::initialize_heap(std::size_t heap_size=10) {
        runtime_data.heap.data = static_cast<decltype(get_heap_data())>(
            std::malloc(heap_size*sizeof(std::uint8_t))
        );
        get_heap_size() = heap_size;
    }

    void Engine::reset_stack() {
        get_stack().clear();
    }

    std::uint64_t Engine::pop_from_stack() {
        const std::uint64_t temp = get_stack().back();
        get_stack().pop_back();
        return temp;
    }

    void Engine::push_onto_stack(std::uint64_t value) {
        get_stack().push_back(value);
    }

    std::size_t Engine::get_stack_size() const {
        return get_stack().size();
    }

    std::uint64_t& Engine::get_stack_top() {
        return get_stack().back();
    }

    const std::uint64_t& Engine::get_stack_top() const {
        return get_stack().back();
    }

    template <typename T>
    T Engine::read_code() 
        requires requires {std::is_integral_v<T>; std::is_unsigned_v<T>; } 
    {
        const auto data = *(const T*)(get_code() + get_code_offset());
        increment_code_offset(sizeof(T));
        return data;
    }

    const std::uint8_t* Engine::get_constants() const {
        return runtime_data.constants;
    }

    std::uint8_t* Engine::get_heap_data() {
        return runtime_data.heap.data;
    }

    std::size_t& Engine::get_heap_size() {
        return runtime_data.heap.size;
    }

    std::size_t& Engine::get_code_offset() {
        return runtime_data.code.offset;
    }

    void Engine::increment_code_offset(std::size_t amount) {
        get_code_offset() += amount;
    }

    std::vector<uint64_t>& Engine::get_stack() {
        return runtime_data.stack;
    }

    void Engine::update_stack_top(uint64_t new_value) {
        get_stack_top() = new_value;
    }

    void Engine::update_code_offset(std::size_t new_value) {
        get_code_offset() = new_value;
    }

    const std::uint8_t* Engine::get_heap_data() const {
        return runtime_data.heap.data;
    }

    const std::size_t& Engine::get_heap_size() const {
        return runtime_data.heap.size;
    }

    const std::uint8_t* Engine::get_code() const {
        return runtime_data.code.data;
    }

    const std::size_t& Engine::get_code_offset() const {
        return runtime_data.code.offset;
    }

    const std::vector<uint64_t>& Engine::get_stack() const {
        return runtime_data.stack;
    }

    int Engine::run(EngineInitializer initializer) {
        initialize_constants(initializer.constants);
        initialize_code(initializer.code);
        initialize_heap(initializer.heap_size);

        while (1) {
            auto temp = read_code<uint16_t>();

            #define HANDLE_OPCODE(opcode, callback) \
                case Opcode::opcode: {\
                    callback();\
                    break;\
                }

            switch (static_cast<Opcode>(temp)) {
            case Opcode::Halt: {
                execute_op_halt();
                if (get_stack_size() > 0) {
                    return static_cast<int>(get_stack_top());
                }
                return -1;
            }
            HANDLE_OPCODE(Add, execute_op_add)
            HANDLE_OPCODE(Sub, execute_op_sub)
            HANDLE_OPCODE(Mul, execute_op_mul)
            HANDLE_OPCODE(Div, execute_op_div)
            HANDLE_OPCODE(Mod, execute_op_mod)
            HANDLE_OPCODE(Pow, execute_op_pow)
            HANDLE_OPCODE(UAdd, execute_op_uadd)
            HANDLE_OPCODE(USub, execute_op_usub)
            HANDLE_OPCODE(UMul, execute_op_umul)
            HANDLE_OPCODE(UDiv, execute_op_udiv)
            HANDLE_OPCODE(UMod, execute_op_umod)
            HANDLE_OPCODE(UPow, execute_op_upow)
            HANDLE_OPCODE(Shl, execute_op_shl)
            HANDLE_OPCODE(Shr, execute_op_shr)
            HANDLE_OPCODE(Inc, execute_op_inc)
            HANDLE_OPCODE(Dec, execute_op_dec)
            HANDLE_OPCODE(Cmp, execute_op_cmp)
            HANDLE_OPCODE(And, execute_op_and)
            HANDLE_OPCODE(Or, execute_op_or)
            HANDLE_OPCODE(Not, execute_op_not)
            HANDLE_OPCODE(Xor, execute_op_xor)
            HANDLE_OPCODE(Equ, execute_op_equ)
            HANDLE_OPCODE(Neq, execute_op_neq)
            HANDLE_OPCODE(Gtr, execute_op_gtr)
            HANDLE_OPCODE(Gte, execute_op_gte)
            HANDLE_OPCODE(Lst, execute_op_lst)
            HANDLE_OPCODE(Lte, execute_op_lte)
            HANDLE_OPCODE(Jmp, execute_op_jmp)
            HANDLE_OPCODE(Jt, execute_op_jt)
            HANDLE_OPCODE(Jf, execute_op_jf)
            HANDLE_OPCODE(Jz, execute_op_jz)
            HANDLE_OPCODE(Jnz, execute_op_jnz)
            HANDLE_OPCODE(Je, execute_op_je)
            HANDLE_OPCODE(Jne, execute_op_jne)
            HANDLE_OPCODE(Jl, execute_op_jl)
            HANDLE_OPCODE(Jnl, execute_op_jnl)
            HANDLE_OPCODE(Jle, execute_op_jle)
            HANDLE_OPCODE(Jnle, execute_op_jnle)
            HANDLE_OPCODE(Jg, execute_op_jg)
            HANDLE_OPCODE(Jng, execute_op_jng)
            HANDLE_OPCODE(Jge, execute_op_jge)
            HANDLE_OPCODE(Jnge, execute_op_jnge)
            HANDLE_OPCODE(Xchg, execute_op_xchg)
            HANDLE_OPCODE(IConst, execute_op_iconst)
            HANDLE_OPCODE(Pop, execute_op_pop)
            HANDLE_OPCODE(ILoad, execute_op_iload)
            HANDLE_OPCODE(IStore, execute_op_istore)
            HANDLE_OPCODE(Int, execute_op_int)
            HANDLE_OPCODE(Nop, execute_op_nop)
            HANDLE_OPCODE(Lea, execute_op_lea)
            HANDLE_OPCODE(Rand, execute_op_rand)
            HANDLE_OPCODE(Puts, execute_op_puts)
            HANDLE_OPCODE(Putc, execute_op_putc)
            HANDLE_OPCODE(Dup, execute_op_dup)
            HANDLE_OPCODE(Dump, execute_op_dump)
            default:
                report_fatal_error("unknown opcode: 0x", std::hex, temp);
            }
        }
        return 0;
    }

    void Engine::execute_op_add() {
        const auto second_value = static_cast<int64_t>(pop_from_stack());
        const auto first_value = static_cast<int64_t>(get_stack_top());
        update_stack_top(static_cast<uint64_t>(first_value + second_value));
    }

    void Engine::execute_op_sub() {
        const auto second_value = static_cast<int64_t>(pop_from_stack());
        const auto first_value = static_cast<int64_t>(get_stack_top());
        update_stack_top(static_cast<uint64_t>(first_value - second_value));
    }

    void Engine::execute_op_mul() {
        const auto second_value = static_cast<int64_t>(pop_from_stack());
        const auto first_value = static_cast<int64_t>(get_stack_top());
        update_stack_top(static_cast<uint64_t>(first_value * second_value));
    }

    void Engine::execute_op_div() {
        const auto second_value = static_cast<int64_t>(pop_from_stack());
        const auto first_value = static_cast<int64_t>(get_stack_top());
        update_stack_top(static_cast<uint64_t>(first_value / second_value));
    }

    void Engine::execute_op_mod() {
        const auto second_value = static_cast<int64_t>(pop_from_stack());
        const auto first_value = static_cast<int64_t>(get_stack_top());
        update_stack_top(static_cast<uint64_t>(first_value % second_value));
    }

    void Engine::execute_op_pow() {
        const auto second_value = pop_from_stack();
        const uint64_t first_value = static_cast<int64_t>(get_stack_top());
        update_stack_top(static_cast<uint64_t>(pow(first_value, second_value)));
    }

    void Engine::execute_op_uadd() {
        const auto second_value = pop_from_stack();
        get_stack_top() += second_value;
    }

    void Engine::execute_op_usub() {
        const auto second_value = pop_from_stack();
        get_stack_top() -= second_value;
    }

    void Engine::execute_op_umul() {
        const auto second_value = pop_from_stack();
        get_stack_top() *= second_value;
    }

    void Engine::execute_op_udiv() {
        const auto second_value = pop_from_stack();
        get_stack_top() /= second_value;
    }

    void Engine::execute_op_umod() {
        const auto second_value = pop_from_stack();
        get_stack_top() %= second_value;
    }

    void Engine::execute_op_upow() {
        const auto second_value = pop_from_stack();
        const uint64_t first_value = get_stack_top();
        update_stack_top(pow(first_value, second_value));
    }

    void Engine::execute_op_shl() {
        const auto second_value = pop_from_stack();
        get_stack_top() <<= second_value;
    }

    void Engine::execute_op_shr() {
        const auto second_value = pop_from_stack();
        get_stack_top() >>= second_value;
    }

    void Engine::execute_op_inc() {
        get_stack_top()++;
    }

    void Engine::execute_op_dec() {
        get_stack_top()--;
    }

    void Engine::execute_op_cmp() {
        const auto second_value = pop_from_stack();
        const uint64_t first_value = get_stack_top();

        if (first_value > second_value) {
            update_stack_top(1);
        }
        else if (first_value < second_value) {
            update_stack_top(-1);
        }
        else {
            update_stack_top(0);
        }
    }

    void Engine::execute_op_and() {
        const auto second_value = pop_from_stack();
        get_stack_top() &= second_value;
    }

    void Engine::execute_op_or() {
        const auto second_value = pop_from_stack();
        get_stack_top() |= second_value;
    }

    void Engine::execute_op_not() {
        const auto result = ~get_stack_top();
        update_stack_top(result);
    }

    void Engine::execute_op_xor() {
        const auto second_value = pop_from_stack();
        get_stack_top() ^= second_value;
    }

    void Engine::execute_op_equ() {
        const auto second_value = pop_from_stack();
        update_stack_top(second_value == get_stack_top() ? 1 : 0);
    }

    void Engine::execute_op_neq() {
        const auto second_value = pop_from_stack();
        update_stack_top(second_value != get_stack_top() ? 1 : 0);
    }

    void Engine::execute_op_gtr() {
        const auto second_value = pop_from_stack();
        update_stack_top(get_stack_top() > second_value ? 1 : 0);
    }

    void Engine::execute_op_gte() {
        const auto second_value = pop_from_stack();
        update_stack_top(get_stack_top() >= second_value ? 1 : 0);
    }

    void Engine::execute_op_lst() {
        const auto second_value = pop_from_stack();
        update_stack_top(get_stack_top() < second_value ? 1 : 0);
    }

    void Engine::execute_op_lte() {
        const auto second_value = pop_from_stack();
        update_stack_top(get_stack_top() <= second_value ? 1 : 0);
    }

    void Engine::execute_op_jmp() {
        update_code_offset(pop_from_stack());
    }

    void Engine::execute_op_jt() {
        const auto target = pop_from_stack();
        if (pop_from_stack() > 0) {
            update_code_offset(target);
        }
    }

    void Engine::execute_op_jf() {
        const auto target = pop_from_stack();
        if (pop_from_stack() == 0) {
            update_code_offset(target);
        }
    }

    void Engine::execute_op_jz() {
        const auto target = pop_from_stack();
        if (pop_from_stack() == 0) {
            update_code_offset(target);
        }
    }

    void Engine::execute_op_jnz() {
        const auto target = pop_from_stack();
        if (pop_from_stack() != 0) {
            update_code_offset(target);
        }
    }

    void Engine::execute_op_je() {
        const auto target = pop_from_stack();
        const auto second_value = pop_from_stack();
        if (pop_from_stack() == second_value) {
            update_code_offset(target);
        }
    }

    void Engine::execute_op_jne() {
        const auto target = pop_from_stack();
        const auto second_value = pop_from_stack();
        if (pop_from_stack() != second_value) {
            update_code_offset(target);
        }
    }

    void Engine::execute_op_jl() {
        const auto target = pop_from_stack();
        const auto second_value = pop_from_stack();
        if (pop_from_stack() < second_value) {
            update_code_offset(target);
        }
    }

    void Engine::execute_op_jnl() {
        const auto target = pop_from_stack();
        const auto second_value = pop_from_stack();
        if (pop_from_stack() >= second_value) {
            update_code_offset(target);
        }
    }

    void Engine::execute_op_jle() {
        const auto target = pop_from_stack();
        const auto second_value = pop_from_stack();
        if (pop_from_stack() <= second_value) {
            update_code_offset(target);
        }
    }

    void Engine::execute_op_jnle() {
        execute_op_jg();
    }

    void Engine::execute_op_jg() {
        const auto target = pop_from_stack();
        const auto second_value = pop_from_stack();
        if (pop_from_stack() > second_value) {
            update_code_offset(target);
        }
    }

    void Engine::execute_op_jng() {
        execute_op_jle();
    }

    void Engine::execute_op_jge() {
        execute_op_jnl();
    }

    void Engine::execute_op_jnge() {
        execute_op_jl();
    }

    void Engine::execute_op_xchg() {
        const auto second_address = pop_from_stack();
        const auto first_address = pop_from_stack();

        const auto first_item = get_heap_item<uint64_t>(first_address);
        const auto second_item = get_heap_item<uint64_t>(second_address);

        put_heap_item(first_address, second_item);
        put_heap_item(second_address, first_item);
    }

    void Engine::execute_op_iconst() {
        const auto constant_address = read_code<uint64_t>();
        push_onto_stack(get_integer_constant<uint64_t>(constant_address));
    }

    void Engine::execute_op_pop() {
        pop_from_stack();
    }

    void Engine::execute_op_iload() {
        const auto item = get_heap_item<uint64_t>(get_stack_top());
        update_stack_top(item);
    }

    void Engine::execute_op_istore() {
        const auto address = pop_from_stack();
        put_heap_item(address, pop_from_stack());
    }

    void Engine::execute_op_int() {
        assert(0);
    }

    void Engine::execute_op_nop() {
        // ...
    }

    void Engine::execute_op_lea() {
        assert(0);
    }

    void Engine::execute_op_rand() {
        srand(time(NULL));
        push_onto_stack(((uint64_t)rand()) % ULLONG_MAX);
    }

    void Engine::execute_op_puts() {
        const auto address = pop_from_stack();
        const auto length = pop_from_stack();
        printf("%.*s", (int)length, get_heap_data() + address);
    }

    void Engine::execute_op_putc() {
        printf("%c", get_heap_item<char>(pop_from_stack()));
    }

    void Engine::execute_op_dup() {
        push_onto_stack(get_stack_top());
    }

    void Engine::execute_op_dump() {
        std::cout << "## Stack: \n";

        auto iter = get_stack().rbegin();
        const auto end = get_stack().rend();

        while (iter != end) {
            std::cout << "  " << *iter++ << "\n";
        }
    }

    void Engine::execute_op_halt() {
        std::cout << "exiting...\n";
    }
}

namespace enact {
    Engine get_engine() {
        return Engine();
    }
}
