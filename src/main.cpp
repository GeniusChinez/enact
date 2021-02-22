
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

#include "assembler.cpp"
#include "opcodes.cpp"
#include "engine.cpp"

int main(int argc, char* const argv[]) {
    (void)argc;
    (void)argv;

    //enact::assemble_file_to_file("sample.asm");
    auto code = enact::assemble_file("sample.asm");

    return enact::get_engine().run(
        {
            .heap_size=1000, 
            .code=(const uint8_t*)(code.data() + 8), 
            .constants=(const uint8_t*)(code.data() + *(uint64_t*)code.data())
        }
    );

    quit(0, 0);
}
