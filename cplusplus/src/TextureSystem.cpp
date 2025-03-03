#include "TextureSystem.hpp"
#include "System.hpp"
#include "dllimport.hpp"

using namespace csimp;
using namespace uniconv;
using namespace std;
using namespace cpp;

using namespace cs;

vector<wstring> TextureSystem::ImportText(wstring filename) {
    unichar** textptr = TextureSystem_ImportText(Utf8StringToUnicode(WStringToNative(filename).c_str()));


    vector<wstring> utftext;
    for (size_t i = 0; true; i++) {
        wstring utfline;
        for (size_t j = 0; textptr[i][j] > 0; j++) {
            utfline.push_back(NativeToWChar(UnicodeToUtf8(textptr[i][j])));
        }
        delete[] textptr[i];
        if (utfline.size() == 0) { break; }
        utftext.push_back(utfline);
    }

    delete[] textptr;

    return utftext;
    
}

void TextureSystem::ExportText(wstring file, vector<wstring> lines) {
    unichar** unilines = new unichar*[lines.size()];

    for (size_t i = 0; i < lines.size(); i++) {
        unilines[i] = new unichar[lines[i].size() + 1];
        size_t ofst = 0;
        for (size_t j = 0; j < lines[i].size(); j++) {
            size_t ch_size;
            unilines[i][j] = Utf8ToUnicode(ReadUtfChar(WStringToNative(lines[i]).c_str(),ofst, &ch_size));
            ofst += ch_size;
        }
    }
    unilines[lines.size()] = new unichar[1]{0};
    
    TextureSystem_ExportText(Utf8StringToUnicode(WStringToNative(file).c_str()),unilines);
}

vector<vector<Console::Symbol> > TextureSystem::TextureFromFile(wstring filepath) {
    unichar* arg1 = Utf8StringToUnicode(WStringToNative(filepath).c_str());
    void* ret = csimp::TextureSystem_TextureFromFile(arg1);

    return Convert2dVector<Console::Symbol>(PtrToTexture(ret));
}

void TextureSystem::FileFromTexture(wstring filepath, vector<vector<Console::Symbol> > texture, bool recycle) {
    unichar* filepathPtr = Utf8StringToUnicode(WStringToNative(filepath).c_str());
    void* texturePtr = TextureToPtr(texture);

    csimp::TextureSystem_FileFromTexture(filepathPtr, texturePtr, recycle);
}

void TextureSystem::DrawTextureToScreen(int x, int y, vector<vector<Console::Symbol> > texture, vector<vector<Console::Symbol> > screen)
{
    auto texturePtr = TextureToPtr(texture);
    auto screenPtr = TextureToPtr(screen);

    csimp::TextureSystem_DrawTextureToScreen(x, y, texturePtr, screenPtr);
}