#include "FileSystem.hpp"
#include "System.hpp"
#include "dllimport.hpp"

using namespace csimp;
using namespace uniconv;
using namespace std;
using namespace cpp;

namespace cs {
    inline vector<vector<smart_ref<Console::Symbol> > > PtrToTexture(void* ptr, bool direct = false) {
        auto sym = vector<vector<smart_ref<Console::Symbol> > >();

        const int int32_size = sizeof(int32_t);
        const int intptr_size = sizeof(void*);

        void* now_ptr;

        int32_t height = System::ReadPointer<int32_t>(ptr);
        now_ptr = System::MovePointer(ptr, int32_size);

        for (int32_t i = 0; i < height; i++) {
            int32_t width = System::ReadPointer<int32_t>(now_ptr);
            now_ptr = System::MovePointer(now_ptr, int32_size);
            vector<smart_ref<Console::Symbol> > now;

            for (int32_t j = 0; j < width; j++) {
                Console::Symbol* sym = (Console::Symbol*)System::ReadPointer<nint>(now_ptr);
                if (direct) now.push_back(smart_ref(sym));
                else now.push_back(smart_ref(Console::Symbol(*sym)));
                now_ptr = System::MovePointer(now_ptr, intptr_size);
            }

            if (direct) sym.push_back(now);
            else sym.push_back(now);
        }

        System::FreeMemory(ptr);
        
        return sym;
    }

    inline void* TextureToPtr(vector<vector<Console::Symbol> > &texture) {
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
                System::WritePointer<nint>(where, &texture[i][j]);
                where = System::MovePointer(where, intptr_size);
            }
        }

        return ret;
    }
}

using namespace cs;

vector<wstring> FileSystem::ImportText(wstring filename) {
    unichar** textptr = FileSystem_ImportText(Utf8StringToUnicode(WStringToNative(filename).c_str()));


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
            unilines[i][j] = Utf8ToUnicode(ReadUtfChar(WStringToNative(lines[i]).c_str(),ofst, &ch_size));
            ofst += ch_size;
        }
    }
    unilines[lines.size()] = new unichar[1]{0};
    
    FileSystem_ExportText(Utf8StringToUnicode(WStringToNative(file).c_str()),unilines);
}

vector<vector<Console::Symbol> > FileSystem::TextureFromFile(wstring filepath) {
    unichar* arg1 = Utf8StringToUnicode(WStringToNative(filepath).c_str());
    void* ret = csimp::FileSystem_TextureFromFile(arg1);

    return Convert2dVector<Console::Symbol>(PtrToTexture(ret));
}

void FileSystem::FileFromTexture(wstring filepath, vector<vector<Console::Symbol> > texture, bool recycle) {
    unichar* filepathPtr = Utf8StringToUnicode(WStringToNative(filepath).c_str());
    void* texturePtr = TextureToPtr(texture);

    csimp::FileSystem_FileFromTexture(filepathPtr, texturePtr, recycle);
}

void FileSystem::DrawTextureToScreen(int x, int y, vector<vector<Console::Symbol> > texture, vector<vector<Console::Symbol> > screen)
{
    auto texturePtr = TextureToPtr(texture);
    auto screenPtr = TextureToPtr(screen);

    csimp::FileSystem_DrawTextureToScreen(x, y, texturePtr, screenPtr);
}

void FileSystem::PlaySound(wstring filepath, bool wait)
{
    auto filepathPtr = Utf8StringToUnicode(WStringToNative(filepath).c_str());

    csimp::FileSystem_PlaySound(filepathPtr, wait);
}