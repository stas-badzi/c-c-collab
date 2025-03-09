#pragma once
#include <string>
#ifdef PlaySound
#undef PlaySound
#endif

namespace cs {
    class SoundSystem {
    public:
        static void PlaySound(std::u16string filepath, bool wait = false);
    };
}