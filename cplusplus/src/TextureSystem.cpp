#include "TextureSystem.hpp"
#include "System.hpp"
#include "dllimport.hpp"
#include "unicode_conversion.hpp"

#ifdef _WIN32
#define nfopen _wfopen
#define tfgetc fgetwc
#define nstoi wstoi
#else
#define nfopen fopen
#define tfgetc fgetc
#define nstoi stoi
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

static uint8_t HexToByte(int input) {
    if (input>='0'&&input<='9')return input-'0';
    if (input>='A'&&input<='F')return input-'A'+10;
    return 16;
}
static int ByteToHex(uint8_t input) {
    if (input<10)return input+'0';
    if (input<16)return input+'A'-10;
    return '-';
}

vector<vector<Console::Symbol>> TextureSystem::TextureFromFile(u16string filepath) {
    auto file = ImportText(filepath); // Imported List<string>
    auto symbols = vector<vector<Console::Symbol>>(); // Final symbol list

    int width=nstoi(uniconv::U16StringToNative(file[0])),height=nstoi(uniconv::U16StringToNative(file[1]));

    int remainingSymbols = width * 3 * height;
    for (int i = 0; i < height; i++) {
        auto symbolLine = vector<Console::Symbol>();
        for (int j = 0; j < width * 3; j+=3)
            symbolLine.emplace_back(UnicodeToNative(file[i][j]), HexToByte(file[i][j+1]), HexToByte(file[i][j+2]));
        symbols.emplace_back().swap(symbolLine);
    }

    return symbols;
}

void TextureSystem::FileFromTexture(u16string filepath, vector<vector<Console::Symbol> > texture) {
    unichar* filepathPtr = U16StringToUnicode(filepath);
    void* texturePtr = TextureToPtr(texture);

    // [TODO] implement
    //csimp::TextureSystem_FileFromTexture(filepathPtr, texturePtr, recycle);
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