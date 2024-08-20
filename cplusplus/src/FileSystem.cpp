#include "FileSystem.hpp"

#include "dllimport.hpp"

using namespace csimp;
using namespace uniconv;
using namespace std;
using namespace cs;

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