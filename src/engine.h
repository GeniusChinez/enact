#pragma once

#include <cassert>
#include <cstdint>

#include <vector>
#include <string_view>

namespace enact {
    struct EngineInitializer {
        std::size_t heap_size;
        const std::uint8_t* code;
        const std::uint8_t* constants;
    };

    class Engine {
    public:
        Engine() = default;
        Engine(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) = delete;

        int run(EngineInitializer);

    private:
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

        void reset_stack();
        std::uint64_t pop_from_stack();
        void push_onto_stack(std::uint64_t);

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
    };
}
