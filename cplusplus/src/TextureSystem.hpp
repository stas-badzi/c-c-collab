#pragma once

#include <vector>
#include <variant>
#include <unicode_conversion.hpp>
#include "Console.hpp"
#include "smart_ref.hpp"

namespace cs {

    template<typename Tout, typename Tin>
    std::vector<Tout> ConvertVector(const std::vector<Tin>& vec);
    
    template<typename Tout, typename Tin>
    std::vector<std::vector<Tout>> Convert2dVector(const std::vector<std::vector<Tin>>& vec2d);

    class TextureSystem {
    public:
        static std::vector<std::wstring> ImportText(std::wstring filename);
        static void ExportText(std::wstring file, std::vector<std::wstring> lines);
        static std::vector<std::vector<cpp::Console::Symbol> > TextureFromFile(std::wstring filepath);
        static void FileFromTexture(std::wstring filepath, std::vector<std::vector<cpp::Console::Symbol> > texture, bool recycle = false);
        static void DrawTextureToScreen(int x, int y, std::vector<std::vector<cpp::Console::Symbol> > texture, std::vector<std::vector<cpp::Console::Symbol> > screen);
    };
}

#include "TextureSystem.ipp"