#include "TextureSystem.hpp"
#include "System.hpp"
#include "dllimport.hpp"

using namespace uniconv;
using namespace std;
using namespace util;
using namespace cpp;

vector<u16string> TextureSystem::ImportText(u16string filename) {
    unichar** textptr = cppimp::TextureSystem_ImportText(U16StringToUnicode(filename));


    vector<u16string> utftext;
    for (size_t i = 0; textptr[i]; i++) {
        u16string utfline;
        for (size_t j = 0; textptr[i][j] > 0; j++) {
            utfline.push_back(UnicodeToChar16(textptr[i][j]));
        }
        free(textptr[i]);
        utftext.push_back(utfline);
    }

    free(textptr);

    return utftext;
    
}

void TextureSystem::ExportText(u16string file, vector<u16string> lines) {
    unichar** unilines = new unichar*[lines.size()+1];

    for (size_t i = 0; i < lines.size(); i++)
        unilines[i] = U16StringToUnicode(lines[i]);
    unilines[lines.size()] = NULL;
    
    cppimp::TextureSystem_ExportText(U16StringToUnicode(file),unilines);
}

vector<vector<Console::Symbol> > TextureSystem::TextureFromFile(u16string filepath) {
    unichar* arg1 = U16StringToUnicode(filepath);
    void* ret = cppimp::TextureSystem_TextureFromFile(arg1);

    return PtrToTexture(ret);
}

void TextureSystem::FileFromTexture(u16string filepath, const vector<vector<Console::Symbol> >& texture) {
    unichar* filepathPtr = U16StringToUnicode(filepath);
    void* texturePtr = TextureToPtr((vector<vector<Console::Symbol> >&)texture);

    cppimp::TextureSystem_FileFromTexture(filepathPtr, texturePtr);
}

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
                if (elem.character != u'\t') {
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

vector<vector<Console::Symbol> > util::PtrToTexture(nint ptr) {
    vector<vector<Console::Symbol> > ret;

    const int int32_size = sizeof(int32_t);
    const int intptr_size = sizeof(void*);

    void* now_ptr;

    int32_t height = System::ReadPointer<int32_t>(ptr);
    now_ptr = System::MovePointer(ptr, int32_size);

    for (int32_t i = 0; i < height; i++) {
        int32_t width = System::ReadPointer<int32_t>(now_ptr);
        now_ptr = System::MovePointer(now_ptr, int32_size);
        vector<Console::Symbol> now;

        for (int32_t j = 0; j < width; j++) {
            nint sym = System::ReadPointer<nint>(now_ptr);
            now.push_back(Console::Symbol(sym));
            cppimp::Console_Symbol_Destruct(sym);
            now_ptr = System::MovePointer(now_ptr, intptr_size);
        }

        ret.push_back(now);
    }

    System::FreeMemory(ptr);
    
    return ret;
}

void* util::TextureToPtr(vector<vector<Console::Symbol> >& texture) {
    const int int32_size = sizeof(int32_t);
    const int intptr_size = sizeof(void*);
    int32_t size, count;

    size = texture.size();
    count = 0;
    for (int32_t i = 0; i < size; i++) {
        count += texture[i].size();
    }

    void* ret = System::AllocateMemory((size + 1) * int32_size + count * intptr_size);

    count = 0;
    void* where;
    System::WritePointer<int32_t>(ret, size);
    where = System::MovePointer(ret, int32_size);
    for (int32_t i = 0; i < size; i++) {
        System::WritePointer<int32_t>(where,texture[i].size());
        where = System::MovePointer(where, int32_size);
        for (size_t j = 0; j < texture[i].size(); j++) {
            System::WritePointer<nint>(where, cppimp::Console_Symbol_Construct$smb(texture[i][j].Get()));
            where = System::MovePointer(where, intptr_size);
        }
    }

    return ret;
}