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
    
    void * ret = csimp::FileSystem_TextureFromFile(arg1);

    vector<vector<Console::Symbol>> out;

    int int32_size = sizeof(int32_t);
    int intptr_size = sizeof(void*);

    void* now_ptr = ret;

    int32_t height = *(int32_t*)(now_ptr);
    now_ptr += int32_size;

    for (size_t i = 0; i < height; i++) {
        int32_t width = *(int32_t*)(now_ptr);
        now_ptr += int32_size;
        out.push_back(vector<Console::Symbol>());

        for (size_t j = 0; j < width; j++) {
            void* ptr = *(void**)(now_ptr);
            Console::Symbol sym = Console::Symbol(ptr,true);
            out.back().push_back(sym);
            now_ptr += intptr_size;
        }
    }

    delete[] ret;
    
    return out;
}