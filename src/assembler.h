#pragma once

#include "opcodes.cpp"
#include "location.h"

#include <cstdint>

#include <type_traits>
#include <vector>
#include <string>
#include <string_view>

namespace enact {
    struct Token {
        std::size_t line;
        std::size_t column;
    };

    using byte_stream_t = std::vector<std::uint8_t>;

    class Assembler {
    public:
        Assembler() = default;

        const byte_stream_t& assemble_files(const std::vector<std::string>&);

        [[nodiscard]] 
        const byte_stream_t& reap() const;

        std::size_t get_number_of_errors() const;
        std::size_t get_number_of_warnings() const;

        void clear();

    private:
        template <typename T>
        [[nodiscard]] 
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

        template <typename ...Args> 
        void report_fatal_error_here(Args&&...);

        void increment_number_of_errors();
        void increment_number_of_warnings();

        struct {
            std::size_t number_of_errors {0};
            std::size_t number_of_warnings {0};
        } issues;

        std::uint32_t get_next_char();
        std::uint32_t get_current_char() const;

        bool data_ended() const;
        void skip_space();

        void start();
        void read_all_instructions();
        void fill_in_locations_waiting_for_constants();
        void fill_location_waiting_for_constant(std::size_t, std::size_t);

        void replace_each_waiting_position_with_value(
            const std::string&, 
            const std::vector<std::size_t>&
        );

        void read_instruction();

        [[nodiscard]] 
        std::string read_name();
        
        void read_label(std::string&&);
        void read_constant_definition(std::string&&);
        void read_constant_string_definition(std::string&&);
        void read_iconst();
        void read_iconst_integer_operand();
        void read_iconst_name_operand();

        void expect_character(std::uint32_t);

        void add_waiting_location(std::string_view, std::size_t);

        template <typename IntType> 
        void write_integer(IntType) 
            requires std::is_integral_v<IntType>;

        void write_opcode(Opcode);

        template <typename IntType>
        [[nodiscard]]
        std::size_t add_integer_constant(IntType)
            requires std::is_integral_v<IntType>;

        void add_string_constant(std::string_view, std::string_view);
        void record_constant_location(std::string_view, std::size_t);

        [[nodiscard]]
        std::size_t get_constant_location(std::string_view);

        void combine_constants_and_code();

        struct {
            std::string filepath;
            std::string data;
            std::string::iterator data_iter;
            std::uint32_t current_char;

            struct {
                std::size_t line {1};
                std::size_t column {0};
            } position;
        } source;

        byte_stream_t output_data;
        byte_stream_t constant_pool;

        std::map<std::string, std::size_t> constant_locations;
        std::map<std::string, std::vector<std::size_t>> waiting_locations;
    };
}
