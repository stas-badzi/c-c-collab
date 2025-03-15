#include "TextureSystem.hpp"
#include "System.hpp"
#include "dllimport.hpp"

using namespace csimp;
using namespace uniconv;
using namespace std;
using namespace cpp;
using namespace cs;

vector<u16string> TextureSystem::ImportText(u16string filename) {
    unichar** textptr = TextureSystem_ImportText(U16StringToUnicode(filename));


    vector<u16string> utftext;
    for (size_t i = 0; true; i++) {
        u16string utfline;
        for (size_t j = 0; textptr[i][j] > 0; j++) {
            utfline.push_back(UnicodeToChar16(textptr[i][j]));
        }
        delete[] textptr[i];
        if (utfline.size() == 0) { break; }
        utftext.push_back(utfline);
    }

    delete[] textptr;

    return utftext;
    
}

void TextureSystem::ExportText(u16string file, vector<u16string> lines) {
    unichar** unilines = new unichar*[lines.size()];

    for (size_t i = 0; i < lines.size(); i++) {
        unilines[i] = U16StringToUnicode(lines[i]);
    }
    unilines[lines.size()] = new unichar[1]{0};
    
    TextureSystem_ExportText(U16StringToUnicode(file),unilines);
}

vector<vector<Console::Symbol> > TextureSystem::TextureFromFile(u16string filepath) {
    unichar* arg1 = U16StringToUnicode(filepath);
    void* ret = csimp::TextureSystem_TextureFromFile(arg1);

    return Convert2dVector<Console::Symbol>(PtrToTexture(ret));
}

void TextureSystem::FileFromTexture(u16string filepath, vector<vector<Console::Symbol> > texture, bool recycle) {
    unichar* filepathPtr = U16StringToUnicode(filepath);
    void* texturePtr = TextureToPtr(texture);

    csimp::TextureSystem_FileFromTexture(filepathPtr, texturePtr, recycle);
}

void TextureSystem::DrawTextureToScreen(int x, int y, vector<vector<Console::Symbol> > texture, vector<vector<Console::Symbol> > screen)
{
    auto texturePtr = TextureToPtr(texture);
    auto screenPtr = TextureToPtr(screen);

    csimp::TextureSystem_DrawTextureToScreen(x, y, texturePtr, screenPtr);
}