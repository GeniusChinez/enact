#pragma once

#include "assembler.h"
#include "reader.h"

namespace enact {
    std::size_t Assembler::get_number_of_errors() const {
        return issues.number_of_errors;
    }

    std::size_t Assembler::get_number_of_warnings() const {
        return issues.number_of_warnings;
    }

    void Assembler::increment_number_of_errors() {
        issues.number_of_errors++;
    }

    void Assembler::increment_number_of_warnings() {
        issues.number_of_warnings++;
    }

    template <typename T>
    bool Assembler::print_location_if_possible(const T& data) const {
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

    void Assembler::print_issue_tail() const {
        std::cout << '\n';
    }

    template <typename ...Args>
    void Assembler::print_issue_tail(Args&&... args) const {
        (std::cout << ... << args);
        std::cout << '\n';
    }

    template <typename T, typename ...Args> 
    void Assembler::report_warning(const T& arg1, Args&&... args) {
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
    void Assembler::report_error(const T& arg1, Args&&... args) {
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
    void Assembler::report_fatal_error(const T& arg1, Args&&... args) {
        report_error(arg1, std::forward<Args>(args)...);
        quit(get_number_of_errors(), get_number_of_warnings());
    }

    template <typename ...Args> 
    void Assembler::report_fatal_error_here(Args&&... args) {
        report_fatal_error(
            new Location{
                .file=source.filepath, 
                .line=source.position.line,
                .column=source.position.column
            },
            std::forward<Args>(args)...
        );
    }

    const byte_stream_t& Assembler::assemble_files(
        const std::vector<std::string>& filepaths
    ) {
        // as a placeholder. It represents the constants poool offset
        write_integer((uint64_t)0);

        for (const auto& filepath : filepaths) {
            const auto [fileread, filedata] = read_file(filepath);

            if (!fileread) {
                report_fatal_error(
                    "could not open input file: ", 
                    filepath.data()
                );
            }

            source.filepath = filepath.data();
            source.data = std::move(filedata);
            source.data_iter = std::begin(source.data);

            read_all_instructions();

            source.current_char = 1;
            source.position.line = 1;
            source.position.column = 0;
        }

        fill_in_locations_waiting_for_constants();
        combine_constants_and_code();

        return reap();
    }

    const byte_stream_t& Assembler::reap() const {
        return output_data;
    }

    void Assembler::clear() {
        source.filepath = "";

        issues.number_of_errors = 0;
        issues.number_of_warnings = 0;

        output_data.clear();

        source.data = "";
        source.filepath = "";
        source.current_char = 1;

        waiting_locations.clear();
    }

    std::uint32_t Assembler::get_next_char() {
        source.position.column++;

        // for now, use ascii
        return (
            source.current_char = static_cast<std::uint32_t>(
                *source.data_iter++
            )
        );
    }

    std::uint32_t Assembler::get_current_char() const {
        return source.current_char;
    }

    bool Assembler::data_ended() const {
        return get_current_char() == 0;
    }

    void Assembler::skip_space() {
        while ((!data_ended()) && (std::isspace(source.current_char))) {
            if (source.current_char == '\n') {
                source.position.line++;
                source.position.column = 0;
            }
            get_next_char();
        }
    }

    void Assembler::start() {
        // ...
    }

    void Assembler::read_all_instructions() {
        get_next_char();

        while (!data_ended()) {
            skip_space();

            if (
                std::isalpha(get_current_char()) || (get_current_char() == '_')
            ) {
                read_instruction();
                continue;
            }

            if (get_current_char() == 0) {
                break;
            }

            if (get_current_char() == '#') {
                while ((!data_ended()) && get_next_char() != '\n') {
                    // ...
                }
                continue;
            }

            report_fatal_error_here(
                "expected an alphabet, but found: 0x", 
                std::hex,
                get_current_char()
            );
        }
    }
    void Assembler::fill_in_locations_waiting_for_constants() {
        for (const auto& location : waiting_locations) {
            replace_each_waiting_position_with_value(
                location.first, 
                location.second
            );
        }
    }

    void Assembler::replace_each_waiting_position_with_value(
        const std::string& name,
        const std::vector<std::size_t>& positions
    ) {
        const auto& constant_location = get_constant_location(name);
        for (const auto& position : positions) {
            fill_location_waiting_for_constant(position, constant_location);
        }
    }

    void Assembler::fill_location_waiting_for_constant(
        std::size_t position,
        std::size_t location
    ) {
        for (std::size_t i = 0; i < sizeof(uint64_t); ++i) {
            output_data[position++] = (0xff & (location >> (i * 8)));
        }
    }

    void Assembler::read_instruction() {
        std::string value = read_name();
        const auto opcode = to_opcode(value);

        if (opcode == Opcode::Err) {
            if (get_current_char() == ':') {
                read_label(std::move(value));
                return;
            }

            read_constant_definition(std::move(value));
            return;
        }

        if (opcode == Opcode::IConst) {
            read_iconst();
            return;
        }

        write_opcode(opcode);
    }

    std::string Assembler::read_name() {
        std::string value;
        do {
            value += get_current_char();
            get_next_char();
        } while (
            (!data_ended()) && 
            (std::isalnum(get_current_char()) || (get_current_char() == '_'))
        );
        return value;
    }

    void Assembler::read_label(std::string&& name) {
        get_next_char();
        record_constant_location(
            name, 

            // we need to subtract the first sizeof(uint64_t) bytes taken up by 
            // the constant_pool offset in the returned code.
            add_integer_constant(output_data.size() - sizeof(uint64_t))
        );
    }

    void Assembler::read_constant_definition(std::string&& name) {
        skip_space();
        std::string token_after_name;

        if ((!data_ended()) && std::isalpha(get_current_char())) {
            token_after_name = read_name();

            if (token_after_name == "str") {
                read_constant_string_definition(std::move(name));
                return;
            }
        }

        report_fatal_error_here("expected 'str' after name '", name, "'");
    }

    void Assembler::read_constant_string_definition(std::string&& name) {
        skip_space();

        expect_character('"');
        get_next_char();

        std::string string_value;

        while ((!data_ended()) && (get_current_char() != '"')) {
            string_value += get_current_char();
            get_next_char();
        }

        expect_character('"');
        get_next_char();

        add_string_constant(name, string_value);
    }

    void Assembler::read_iconst() {
        skip_space();
        write_opcode(Opcode::IConst);

        if (std::isdigit(get_current_char())) {
            read_iconst_integer_operand();
        }
        else if (std::isalpha(get_current_char())) {
            read_iconst_name_operand();
        }
        else {
            report_fatal_error_here(
                "expected integer or name, but found: 0x",
                std::hex,
                get_current_char()
            );
        }
    }

    void Assembler::read_iconst_integer_operand() {
        std::string temp;
        do {
            temp += get_current_char();
            get_next_char();
        } while ((!data_ended()) && std::isdigit(get_current_char()));

        std::uint64_t value;

        try {
            value = std::stoll(temp);
        }
        catch (...) {
            report_fatal_error_here(
                "invalid integer value: ",
                temp
            );
        }

        const std::size_t offset = add_integer_constant(value);
        write_integer((std::uint64_t)offset);
    }

    void Assembler::read_iconst_name_operand() {
        auto name = read_name();

        if (auto temp = constant_locations.find(name); 
            temp != constant_locations.end()
        ) {
            write_integer((std::uint64_t)(temp->second));
        }
        else {
            add_waiting_location(name, output_data.size());
            write_integer((std::uint64_t)0);
        }
    }

    void Assembler::expect_character(std::uint32_t value) {
        if (get_current_char() == value) {
            return;
        }

        report_fatal_error_here(
            "expected '",
            (char)value,
            "', but found: 0x",
            std::hex,
            get_current_char()
        );
    }

    template <typename IntType>
    std::size_t Assembler::add_integer_constant(IntType value)
        requires std::is_integral_v<IntType>
    {
        const std::size_t location = constant_pool.size();

        for (std::size_t i = 0; i < sizeof(IntType); ++i) {
            constant_pool.push_back(0xff & (value >> (i * 8)));
        }

        return location;
    }

    void Assembler::add_string_constant(
        std::string_view name, 
        std::string_view value
    ) {
        record_constant_location(name, constant_pool.size());
        constant_pool.insert(
            constant_pool.end(), 
            std::begin(value), 
            std::end(value)
        );
    }

    void Assembler::record_constant_location(
        std::string_view name, 
        std::size_t location
    ) {
        auto [_, was_inserted] = constant_locations.insert(
            std::pair{name, location}
        );

        if (was_inserted) {
            return;
        }

        report_fatal_error_here("redefinition of constant '", name, "'");
    }

    std::size_t Assembler::get_constant_location(std::string_view name) {
        auto temp = constant_locations.find(name.data());

        if (temp != constant_locations.end()) {
            return temp->second;
        }

        report_fatal_error(
            "referencing undefined constant '",
            name.data(),
            "'"
        );

        // this next line won't get executed... the program would have exited 
        return 0;
    }

    void Assembler::add_waiting_location(
        std::string_view name, 
        std::size_t offset
    ) {
        waiting_locations[name.data()].push_back(offset);
    }

    template <typename IntType> 
    void Assembler::write_integer(IntType value) 
        requires std::is_integral_v<IntType>
    {
        for (std::size_t i = 0; i < sizeof(IntType); ++i) {
            output_data.push_back(0xff & (value >> (i * 8)));
        }
    }

    void Assembler::write_opcode(Opcode opcode) {
        write_integer(static_cast<std::uint16_t>(opcode));
    }

    void Assembler::combine_constants_and_code() {
        std::size_t temp_offset = 0;
        std::uint64_t constant_pool_offset = output_data.size();

        for (std::size_t i = 0; i < sizeof(uint64_t); ++i) {
            output_data[temp_offset++] = 0xff & 
                (constant_pool_offset >> (i * 8));
        }

        output_data.insert(
            output_data.end(), 
            constant_pool.begin(), 
            constant_pool.end()
        );
    }

    byte_stream_t assemble_files(const std::vector<std::string>& filepaths) {
        return Assembler{}.assemble_files(filepaths);
    }

    void write_code_to_file(
        const byte_stream_t& code, 
        std::string_view filepath
    ) {
        std::fstream os(filepath.data(), std::ios::out/* | std::ios::binary*/);

        auto iter = std::begin(code);
        while (iter != std::end(code)) {
            os << *iter++;
        }

        os.close();
    }

    void assemble_files_to_file(
        const std::vector<std::string>& filepaths,
        std::string_view dest="output.asm"
    ) {
        auto code = Assembler{}.assemble_files(filepaths);
        write_code_to_file(code, dest);
    }
}
