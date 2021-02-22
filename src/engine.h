#pragma once

#include <cassert>
#include <cstdint>

#include <vector>
#include <string_view>
#include <functional>

namespace enact {
    struct EngineInitializer {
        std::size_t heap_size;
        const std::uint8_t* code;
        const std::uint8_t* constants;
    };

    class Engine {
    public:
        Engine();
        Engine(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) = delete;

        int run(EngineInitializer);

        std::size_t get_number_of_errors() const;
        std::size_t get_number_of_warnings() const;

    private:
        template <typename T>
        bool print_location_if_possible(const T&) const;

        template <typename ...Args>
        void print_issue_tail(Args&&...) const;
        void print_issue_tail() const;

        template <typename T, typename ...Args> 
        void report_warning(const T&, Args&&...);

        template <typename T, typename ...Args> 
        void report_error(const T&, Args&&...);

        template <typename T, typename ...Args> 
        void report_fatal_error(const T&, Args&&...);

        void increment_number_of_errors();
        void increment_number_of_warnings();

        struct {
            std::size_t number_of_errors {0};
            std::size_t number_of_warnings {0};
        } issues;

        void initialize_code(const std::uint8_t*);
        void initialize_constants(const std::uint8_t*);
        void initialize_heap(std::size_t);

        template <typename IntegerT=std::uint64_t>
        [[nodiscard]] 
        IntegerT get_integer_constant(std::size_t) const
            requires std::is_integral_v<IntegerT>;

        [[nodiscard]] 
        const std::uint8_t* get_string_constant(std::size_t) const;

        template <typename IntegerT=std::uint64_t>
        [[nodiscard]] 
        IntegerT get_heap_item(std::size_t) const 
            requires std::is_integral_v<IntegerT>;

        template <typename IntegerT=std::uint64_t>
        void put_heap_item(std::size_t, IntegerT)
            requires std::is_integral_v<IntegerT>;

        void extend_heap(std::size_t);
        void free_heap();

        [[nodiscard]] std::size_t get_stack_size() const;
        [[nodiscard]] const std::uint64_t& get_stack_top() const;
        [[nodiscard]] std::uint64_t& get_stack_top();

        template <typename T>
        T read_code() 
            requires requires {std::is_integral_v<T>; std::is_unsigned_v<T>; };

        [[nodiscard]] const std::uint8_t* get_constants() const;
        [[nodiscard]] std::uint8_t* get_heap_data();
        [[nodiscard]] std::size_t& get_heap_size();
        [[nodiscard]] std::size_t& get_code_offset();
        [[nodiscard]] std::vector<uint64_t>& get_stack();

        void increment_code_offset(std::size_t);

        [[nodiscard]] const std::uint8_t* get_heap_data() const;
        [[nodiscard]] const std::size_t& get_heap_size() const;
        [[nodiscard]] const std::uint8_t* get_code() const;
        [[nodiscard]] const std::size_t& get_code_offset() const;
        [[nodiscard]] const std::vector<uint64_t>& get_stack() const;

        void update_stack_top(uint64_t);
        void update_code_offset(std::size_t);

        void reset_stack();
        std::uint64_t pop_from_stack();
        void push_onto_stack(std::uint64_t);

        void execute_op_add();
        void execute_op_sub();
        void execute_op_mul();
        void execute_op_div();
        void execute_op_mod();
        void execute_op_pow();

        void execute_op_uadd();
        void execute_op_usub();
        void execute_op_umul();
        void execute_op_udiv();
        void execute_op_umod();
        void execute_op_upow();

        void execute_op_shl();
        void execute_op_shr();

        void execute_op_inc();
        void execute_op_dec();

        void execute_op_cmp();

        void execute_op_and();
        void execute_op_or();
        void execute_op_not();
        void execute_op_xor();

        void execute_op_equ();
        void execute_op_neq();
        void execute_op_gtr();
        void execute_op_gte();
        void execute_op_lst();
        void execute_op_lte();

        void execute_op_jmp();
        void execute_op_jt();
        void execute_op_jf();
        void execute_op_jz();
        void execute_op_jnz();
        void execute_op_je();
        void execute_op_jne();
        void execute_op_jl();
        void execute_op_jnl();
        void execute_op_jle();
        void execute_op_jnle();
        void execute_op_jg();
        void execute_op_jng();
        void execute_op_jge();
        void execute_op_jnge();

        void execute_op_xchg();
        void execute_op_iconst();

        void execute_op_pop();
        void execute_op_iload();
        void execute_op_istore();

        void execute_op_int();
        void execute_op_nop();
        void execute_op_lea();
        void execute_op_rand();

        void execute_op_puts();
        void execute_op_putc();

        void execute_op_dup();
        void execute_op_dump();

        void execute_op_halt();

        void handle_interrupt_input();

        struct {
            const std::uint8_t* constants;

            struct {
                std::uint8_t* data;
                std::size_t size;
            } heap;

            struct {
                const std::uint8_t* data;
                std::size_t offset {0};
            } code;

            std::vector<std::uint64_t> stack;
        } runtime_data;

        using InterruptHandler = std::function<void(Engine*)>;
        std::map<std::size_t, InterruptHandler> interrupt_handlers;
    };
}
