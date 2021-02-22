
#include <iostream>

void quit(std::size_t number_of_errors, std::size_t number_of_warnings) {
    if (number_of_errors == 0 && number_of_warnings == 0) {
        std::cout << "-- successful\n";
        return;
    }

    std::printf(
        "errors: %zu; warnings: %zu\n\n", 
        number_of_errors, number_of_warnings
    );

    exit(number_of_errors);
}
//(78 << 0) + (97 << 8) + (188 << 16) + (0 << 24)

template <typename IntType>
IntType read_stored_integer(const std::uint8_t* src) {
    IntType result = 0;
    std::size_t i = 0;

    while (i < sizeof(IntType)) {
        result += (src[i] << (i*8));
        i++;
    }

    return result;
}

#include "assembler.cpp"
#include "opcodes.cpp"
#include "engine.cpp"

#include "clargs/clargs.cpp"

std::size_t get_default_heap_size(std::optional<std::string_view> value) {
    if (value == std::nullopt) {
        return 1024 * 1024 * 10;
    }

    std::size_t default_heap_size;

    try {
        default_heap_size = std::stoll(std::string(value->data()));
    }
    catch (...) {
        std::cout << "error: could not convert the given default-heap-size to an integer: " << value->data() << "\n";
        exit(4);
    }

    return default_heap_size;
}

int assemble_only(
    const std::vector<std::string>& filenames, 
    const clargs::Arguments& args
) {
    puts("** assembling (only)...");

    if (filenames.size() == 0) {
        puts("error: no input files to assemble");
        exit(2);
    }

    std::string outputfile = "output.out";

    if (args.is_set("o")) {
        if (filenames.size() > 0) {
            puts("error: output file must not be specified when assembling multiple files");
            exit(3);
        }
        else {
            outputfile = *args.get_value_of("o");
        }
    }

    enact::assemble_files_to_file(filenames, outputfile);
    return 0;
}

int assemble_and_run(
    const std::vector<std::string>& filenames, 
    const clargs::Arguments& args
) {
    puts("** assembling (and running)...");

    auto code = enact::assemble_files(filenames);
    const auto constants_offset = read_stored_integer<uint64_t>(code.data());

    if (args.is_set("o")) {
        enact::write_code_to_file(code, *args.get_value_of("o"));
    }

    return enact::get_engine().run(
        {
            .heap_size=get_default_heap_size(
                args.get_value_of("default-heap-size")
            ), 
            .code=code.data() + sizeof(uint64_t), 
            .constants=code.data() + constants_offset
        }
    );
}

void run_only(
    const std::vector<std::string>& filenames, 
    const clargs::Arguments& args
) {
    puts("running (only)...");

    for (const auto& filetorun : filenames) {
        auto [fileread, filedata] = enact::read_file(filetorun);

        if (!fileread) {
            std::cout << "error: file not found '" << filetorun << "'\n";
        }

        const auto constants_offset = read_stored_integer<uint64_t>(
            (const uint8_t*)filedata.data()
        );

        enact::get_engine().run(
            {
                .heap_size=get_default_heap_size(
                    args.get_value_of("default-heap-size")
                ), 
                .code=(const uint8_t*)filedata.data() + sizeof(uint64_t),
                .constants=(const uint8_t*)filedata.data() + constants_offset
            }
        );
    }
}

int main(int argc, char* const argv[]) {
    auto args = clargs::get_arguments(argc, argv);
    args.remove_first_naked_value();
    const auto& filenames = args.get_naked_values();

    if (args.is_set("asm-only") || args.is_set("a")) {
        return assemble_only(filenames, args);
    }
    else if (args.is_set("asm-and-run") || args.is_set("ar")) {
        return assemble_and_run(filenames, args);
    }
    else {
        run_only(filenames, args);
        return 0;
    }
}
