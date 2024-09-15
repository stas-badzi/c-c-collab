#pragma once

#include <vector>
#include <unicode_conversion.hpp>
#include "Console.hpp"

namespace cs {
    class FileSystem {
    public:
        static std::vector<uniconv::utfstr> ImportText(uniconv::utfstr filename);
        static void ExportText(uniconv::utfstr file, std::vector<uniconv::utfstr> lines);
        static std::vector<std::vector<cpp::Console::Symbol>> TextureFromFile(uniconv::utfstr filepath);
        static void FileFromTexture(uniconv::utfstr filepath, std::vector<std::vector<Console::Symbol>> texture, bool recycle = false);
        static void DrawTextureToScreen(int x, int y, std::vector<std::vector<Console::Symbol>> texture, std::vector<std::vector<Console::Symbol>> screen);
        static void PlayWAV(uniconv::utfstr filepath, bool wait = false);
    };
}