#include "FileSystem.hpp"
#include "System.hpp"
#include "dllimport.hpp"

using namespace csimp;
using namespace uniconv;
using namespace std;
using namespace cs;
using namespace cpp;
void* TextureToPtr(vector<vector<Console::Symbol>> texture);
vector<vector<Console::Symbol>> PtrToTexture(void* ptr);


vector<wstring> FileSystem::ImportText(wstring filename) {
    unichar** textptr = FileSystem_ImportText(Utf8StringToUnicode(WStringToNative(filename)));


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

void FileSystem::ExportText(wstring file, vector<wstring> lines) {
    unichar** unilines = new unichar*[lines.size()];

    for (size_t i = 0; i < lines.size(); i++) {
        unilines[i] = new unichar[lines[i].size() + 1];
        size_t ofst = 0;
        for (size_t j = 0; j < lines[i].size(); j++) {
            size_t ch_size;
            unilines[i][j] = Utf8ToUnicode(ReadUtfChar(WStringToNative(lines[i]),ofst, &ch_size));
            ofst += ch_size;
        }
    }
    unilines[lines.size()] = new unichar[1]{0};
    
    FileSystem_ExportText(Utf8StringToUnicode(WStringToNative(file)),unilines);
}

vector<vector<Console::Symbol>> FileSystem::TextureFromFile(wstring filepath) {
    unichar* arg1 = Utf8StringToUnicode(WStringToNative(filepath));
    void* ret = csimp::FileSystem_TextureFromFile(arg1);

    return PtrToTexture(ret);
}

void FileSystem::FileFromTexture(wstring filepath, vector<vector<Console::Symbol>> texture, bool recycle) {
    unichar* filepathPtr = Utf8StringToUnicode(WStringToNative(filepath));
    void* texturePtr = TextureToPtr(texture);

    csimp::FileSystem_FileFromTexture(filepathPtr, texturePtr, recycle);
}

void FileSystem::DrawTextureToScreen(int x, int y, vector<vector<Console::Symbol>> texture, vector<vector<Console::Symbol>> screen)
{
    auto texturePtr = TextureToPtr(texture);
    auto screenPtr = TextureToPtr(screen);

    csimp::FileSystem_DrawTextureToScreen(x, y, texturePtr, screenPtr);
}

void FileSystem::PlaySound(wstring filepath, bool wait)
{
    auto filepathPtr = Utf8StringToUnicode(WStringToNative(filepath));

    csimp::FileSystem_PlaySound(filepathPtr, wait);
}

vector<vector<Console::Symbol>> PtrToTexture(void* ptr) {
    vector<vector<Console::Symbol>> ret;

    const int int32_size = sizeof(int32_t);
    const int intptr_size = sizeof(void*);

    void* now_ptr = ptr;

    int32_t height = *(int32_t*)(now_ptr);
    System::MovePointer(now_ptr, int32_size);

    for (size_t i = 0; i < height; i++) {
        int32_t width = System::ReadPointer<int32_t>(now_ptr);
        System::MovePointer(now_ptr, int32_size);
        ret.push_back(vector<Console::Symbol>());

        for (size_t j = 0; j < width; j++) {
            nint ptr = System::ReadPointer<nint>(now_ptr);
            Console::Symbol sym = Console::Symbol(ptr,true);
            ret.back().push_back(sym);
            System::MovePointer(now_ptr, intptr_size);
        }
    }

    System::FreeMemory(ptr);
    
    return ret;
}

void* TextureToPtr(vector<vector<Console::Symbol>> texture) {
    const int int32_size = sizeof(int32_t);
    const int intptr_size = sizeof(void*);
    int32_t size, count;

    size = texture.size();
    count = 0;
    for (size_t i = 0; i < size; i++) {
        count += texture[i].size();
    }

    void* ret = System::AllocateMemory((size + 1) * int32_size + count * intptr_size);

    count = 0;
    void* where = ret;
    System::WritePointer(where, size);
    System::MovePointer(where, int32_size);
    for (size_t i = 0; i < size; i++) {
        System::WritePointer(where,texture[i].size());
        System::MovePointer(where, int32_size);
        for (size_t j = 0; j < texture[i].size(); j++) {
            System::WritePointer(where, texture[i][j].Get());
            System::MovePointer(where, intptr_size);
        }
    }

    return ret;
}