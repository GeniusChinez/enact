#pragma once

#include <cstdint>
#include <string>

namespace enact {
    struct Location {
        std::string file;
        std::size_t line {0};
        std::size_t column {0};

        Location& operator++(){
            column++;
            return *this;
        }

        std::string format() const {
            return std::string("in file '")  + file + "': " + 
                std::string("line ") + std::to_string(line) + "." + 
                std::to_string(column);
        }
    };
}
