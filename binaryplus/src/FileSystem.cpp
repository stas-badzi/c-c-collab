#include "FileSystem.hpp"

#include "dllimport.hpp"

using namespace csimp;
using namespace uniconv;
using namespace std;
using namespace cs;
using namespace cpp;


vector<utfstr> FileSystem::ImportText(utfstr filename) {
    unichar** textptr (FileSystem_ImportText(Utf8StringToUnicode(filename)));


    vector<utfstr> utftext;
    for (size_t i = 0; true; i++) {
        utfstr utfline;
        for (size_t j = 0; textptr[i][j] > 0; j++) {
            utfline.append(to_string(UnicodeToUtf8(textptr[i][j])));
        }
        delete[] textptr[i];
        if (utfline.size() == 0) { break; }
        utftext.push_back(utfline);
    }

    delete[] textptr;

    return utftext;
    
}

void FileSystem::ExportText(utfstr file, vector<utfstr> lines) {
    unichar** unilines = new unichar*[lines.size()];

    for (size_t i = 0; i < lines.size(); i++) {
        unilines[i] = new unichar[lines[i].size() + 1];
        size_t ofst = 0;
        for (size_t j = 0; j < lines[i].size(); j++) {
            size_t ch_size;
            unilines[i][j] = Utf8ToUnicode(ReadUtfChar(lines[i],ofst, &ch_size));
            ofst += ch_size;
        }
    }
    unilines[lines.size()] = new unichar[1]{0};
    
    FileSystem_ExportText(Utf8StringToUnicode(file),unilines);
}

vector<vector<Console::Symbol>> FileSystem::TextureFromFile(utfstr filepath) {
    unichar* arg1 = Utf8StringToUnicode(filepath);
    void* ret = csimp::FileSystem_TextureFromFile(arg1);

    return PtrToTexture(ret);
}

void FileSystem::FileFromTexture(utfstr filepath, vector<vector<Console::Symbol>> texture, bool recycle = false) {
    unichar* filepathPtr = uniconv::Utf8StringToUnicode(filepath);
    void* texturePtr = TextureToPtr(texture);

    csimp::FileSystem_FileFromTexture(filepathPtr, texturePtr, recycle);
    free(texturePtr);
}

vector<vector<Console::Symbol>> PtrToTexture(void* ptr)
{
    vector<vector<Console::Symbol>> ret;

    const int int32_size = sizeof(int32_t);
    const int intptr_size = sizeof(void*);

    void* now_ptr = ptr;

    int32_t height = *(int32_t*)(now_ptr);
    now_ptr += int32_size;

    for (size_t i = 0; i < height; i++) {
        int32_t width = *(int32_t*)(now_ptr);
        now_ptr += int32_size;
        ret.push_back(vector<Console::Symbol>());

        for (size_t j = 0; j < width; j++) {
            void* ptr = *(void**)(now_ptr);
            Console::Symbol sym = Console::Symbol(ptr,true);
            ret.back().push_back(sym);
            now_ptr += intptr_size;
        }
    }

    free(ptr);
    
    return ret;
}
void* TextureToPtr(vector<vector<Console::Symbol>> texture) {
    const int int32_size = sizeof(int32_t);
    const int intptr_size = sizeof(void*);
    size_t size, count;

    size = texture.size();
    count = 0;
    for (size_t i = 0; i < size; i++) {
        count += texture[i].size();
    }

    void* ret = malloc((size + 1) * int32_size + count * intptr_size);

    count = 0;
    void* where = ret;
    *(int*) where = size;
    where += int32_size;
    for (size_t i = 0; i < size; i++) {
        *(int*) where = texture[i].size();
        where += int32_size;
        for (size_t j = 0; j < texture[i].size(); j++) {
            *(void**) where = texture[i][j].Get();
            where += intptr_size;
        }
    }
    return ret;
}
