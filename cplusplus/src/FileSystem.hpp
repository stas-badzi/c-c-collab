#pragma once

#include <vector>
#include <unicode_conversion.hpp>
#include "Console.hpp"

namespace cs {

    void* TextureToPtr(std::vector<std::vector<cpp::Console::Symbol> >& texture);
    std::vector<std::vector<cpp::Console::Symbol> > PtrToTexture(void* ptr, bool direct = false);

    class FileSystem {
    public:
        static std::vector<std::wstring> ImportText(std::wstring filename);
        static void ExportText(std::wstring file, std::vector<std::wstring> lines);
        static std::vector<std::vector<cpp::Console::Symbol> > TextureFromFile(std::wstring filepath);
        static void FileFromTexture(std::wstring filepath, std::vector<std::vector<cpp::Console::Symbol> > texture, bool recycle = false);
        static void DrawTextureToScreen(int x, int y, std::vector<std::vector<cpp::Console::Symbol> > texture, std::vector<std::vector<cpp::Console::Symbol> > screen);
        static void PlaySound(std::wstring filepath, bool wait = false);
    };
}