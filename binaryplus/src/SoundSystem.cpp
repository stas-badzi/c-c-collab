#include "dllimport.hpp"
#include <unicode_conversion.hpp>
#include "SoundSystem.hpp"

using namespace std;
using namespace cppimp;
using namespace uniconv;

namespace cpp {
    void SoundSystem::PlaySound(u16string filepath, bool wait) {
        auto filepathPtr = U16StringToUnicode(filepath);

        SoundSystem_PlaySound(filepathPtr, wait);
    }
}