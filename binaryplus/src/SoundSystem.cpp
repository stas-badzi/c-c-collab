#include "dllimport.hpp"
#include <unicode_conversion.hpp>
#include "SoundSystem.hpp"

using namespace std;
using namespace csimp;
using namespace cs;
using namespace uniconv;

void SoundSystem::PlaySound(u16string filepath, bool wait) {
    auto filepathPtr = U16StringToUnicode(filepath);

    csimp::SoundSystem_PlaySound(filepathPtr, wait);
}