#pragma once

#include <vector>
#include <unicode_conversion.hpp>
#include "Console.hpp"

namespace cs {
    class TextureSystem {
    public:
        static std::vector<std::u16string> ImportText(std::u16string filename);
        static void ExportText(std::u16string file, std::vector<std::u16string> lines);
        static std::vector<std::vector<cpp::Console::Symbol> > TextureFromFile(std::u16string filepath);
        static void FileFromTexture(std::u16string filepath, std::vector<std::vector<cpp::Console::Symbol> > texture, bool recycle = false);
        static void DrawTextureToScreen(int x, int y, std::vector<std::vector<cpp::Console::Symbol> > texture, std::vector<std::vector<cpp::Console::Symbol> >& screen);
        static void PlaySound(std::u16string filepath, bool wait = false);
    };
 
    void* TextureToPtr(std::vector<std::vector<cpp::Console::Symbol> >& texture);
    std::vector<std::vector<cpp::Console::Symbol> > PtrToTexture(void* ptr, bool direct = false);
}