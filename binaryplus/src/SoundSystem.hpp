#pragma once

#include <vector>
#include <unicode_conversion.hpp>
#include "Console.hpp"

namespace cs {
    class SoundSystem {
        static void PlaySound(std::wstring filepath, bool wait = false);
    };
}