#include "TextureSystem.hpp"
#include "System.hpp"
#include "dllimport.hpp"
#include "unicode_conversion.hpp"
#include <cstdint>
#include <cstdio>

#ifdef _WIN32
#define nfopen _wfopen
#define tfgetc fgetwc
#define ntoi _wtoi
#else
#ifndef __APPLE__
#define fread fread_unlocked
#endif
#define nfopen fopen
#define tfgetc fgetc
#define ntoi atoi
#endif

using namespace uniconv;
using namespace std;
using namespace cpp;
using namespace util;

vector<u16string> TextureSystem::ImportText(u16string filename) {
    FILE* f = nfopen(U16StringToNative(filename).c_str(),N("r"));
    vector<u16string> result;
    nstring text;
    while (1) {
        nchar_t ch = tfgetc(f);
        if (ch&&!feof(f)) text.push_back(ch);
    }
    fclose(f);
    u16string parsed = NativeToU16String(text);
    result.emplace_back();
    for (const auto& ch : parsed)
        if (ch==u'\n')result.emplace_back();
        else result.back().push_back(ch);
    return result;
}

void TextureSystem::ExportText(u16string filename, vector<u16string> lines) {
    FILE* f = nfopen(U16StringToNative(filename).c_str(),N("w"));
    u16string text;
    for (auto&& ln : lines) {
        for (auto&& ch : ln)text.push_back(ch);
        text.push_back(u'\n');
    }
    nstring nativetext = U16StringToNative(text);
    fwrite(nativetext.c_str(),sizeof(nstring::value_type),nativetext.size(),f);
    fclose(f);
}

vector<vector<Console::Symbol>> TextureSystem::TextureFromFile(u16string filename) {
    auto symbols = vector<vector<Console::Symbol>>();
    FILE* f = nfopen(U16StringToNative(filename).c_str(),N("r"));

    uint32_t width,height;
    fread(&width,sizeof(uint32_t),1,f);
    fread(&height,sizeof(uint32_t),1,f);

    for (uint32_t i = 0; i < height; i++) {
        auto symbolLine = vector<Console::Symbol>();
        for (uint32_t j = 0; j < width; ++j) {
            unichar ch;
            fread(&ch,sizeof(unichar),1,f);
            uint8_t fg,bg;
            fread(&fg,sizeof(uint8_t),1,f);
            fread(&bg,sizeof(uint8_t),1,f);
            symbolLine.emplace_back(UnicodeToNative(ch), fg, bg);
        }
        symbols.emplace_back().swap(symbolLine);
    }
    fclose(f);

    return symbols;
}

void TextureSystem::FileFromTexture(u16string filepath, vector<vector<Console::Symbol> > texture) {
    FILE* f = nfopen(U16StringToNative(filepath).c_str(),N("w"));
    uint32_t height=texture.size(),width=height?texture.front().size():0;
    fwrite(&width,sizeof(uint32_t),1,f);
    fwrite(&height,sizeof(uint32_t),1,f);
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++) {
            unichar ch = NativeToUnicode(texture[i][j].character);
            fwrite(&ch,sizeof(unichar),1,f);
            fwrite(&(texture[i][j].foreground),sizeof(uint8_t),1,f);
            fwrite(&(texture[i][j].background),sizeof(uint8_t),1,f);
        }

    }
    fclose(f);
}
static const auto tab = UnicodeToNative(U'\t');
void TextureSystem::DrawTextureToScreen(int x, int y, const std::vector<std::vector<Console::Symbol> >& texture, std::vector<std::vector<Console::Symbol>>& screen) {
    int height = texture.size();
    int scrHeight = screen.size();

    for (int i = 0; i < height; i++) {
        if (y + i < 0) {
            i = -(y + 1);
            continue;
        }
        else if (y + i >= scrHeight) break;
        int width = texture[i].size();
        int scrWidth = screen[y + i].size();
        for (int j = 0; j < width; j++) {
            if (y+i >= 0 && y+i < scrHeight && x+j >= 0 && x+j < scrWidth) {
                auto elem = texture[i][j];
                if (elem.character != tab) {
                    screen[y+i][x+j].character = elem.character;
                }
                if (elem.foreground < 16) {
                    screen[y+i][x+j].foreground = elem.foreground;
                }
                if (elem.background < 16) {
                    screen[y+i][x+j].background = elem.background;
                }
            }
        }
    }
}