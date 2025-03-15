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

    //inline std::vector<std::vector<smart_ref<Console::Symbol> > > PtrToTexture(void* ptr, bool direct = false);
    //inline void* TextureToPtr(std::vector<std::vector<Console::Symbol> > &texture);

    class TextureSystem {
    public:
        static std::vector<std::u16string> ImportText(std::u16string filename);
        static void ExportText(std::u16string file, std::vector<std::u16string> lines);
        static std::vector<std::vector<cpp::Console::Symbol> > TextureFromFile(std::u16string filepath);
        static void FileFromTexture(std::u16string filepath, std::vector<std::vector<cpp::Console::Symbol> > texture, bool recycle = false);
        static void DrawTextureToScreen(int x, int y, std::vector<std::vector<cpp::Console::Symbol> > texture, std::vector<std::vector<cpp::Console::Symbol> > screen);
    };
}

#include "TextureSystem.ipp"