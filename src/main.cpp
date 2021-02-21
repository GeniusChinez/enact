
#include <iostream>

#include "opcodes.cpp"
#include "engine.cpp"

int main(int argc, char* const argv[]) {
    (void)argc;
    (void)argv;

    const uint8_t* constants = (const uint8_t*)"";
    const uint8_t* code = (const uint8_t*)"";

    return enact::get_engine().run(
        {
            .heap_size=100, 
            .code=code, 
            .constants=constants
        }
    );
}
