#include "SoundSystem.hpp"
#include "System.hpp"
#include "dllimport.hpp"

using namespace std;
using namespace csimp;
using namespace uniconv;

void SoundSystem::PlaySound(wstring filepath, bool wait)
{
    auto filepathPtr = Utf8StringToUnicode(WStringToNative(filepath).c_str());

    csimp::TextureSystem_PlaySound(filepathPtr, wait);
}