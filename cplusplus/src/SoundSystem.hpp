#pragma once

namespace cs {
    class SoundSystem {
    public:
        static void PlaySound(std::wstring filepath, bool wait = false);
    };
}