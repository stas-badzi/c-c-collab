#pragma once

#include <utility>

namespace cpp {
    struct ConsoleMouseStatus {
        bool primary;
        bool secondary;
        bool middle;
        std::pair<bool,bool> scroll;
        unsigned int x;
        unsigned int y;
    };
} // namespace cpp