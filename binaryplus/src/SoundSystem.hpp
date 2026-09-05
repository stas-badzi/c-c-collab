#pragma once
#include <string>
#ifdef PlaySound
#undef PlaySound
#endif

namespace cpp {
    class SoundSystem {
    public:
        static void PlaySound(std::u16string filepath, bool wait = false);
    };
}