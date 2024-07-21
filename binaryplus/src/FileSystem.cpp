#include "FileSystem.hpp"

#include "dllimport.hpp"

using namespace csimp;
using namespace std;

vector<wstring> cs::FileSystem::ImportText(wstring filename) {
    wchar_t* result = FileSystem_ImportText(filename.c_str());
    if (result[0] == '\0') { return vector<wstring>(); }
    
    vector<wstring> out;

    out.push_back(wstring());
    for (size_t i = 0; result[i] != '\0'; i++) {
        switch (result[i]) {
        case '\n':
            out.push_back(wstring());
            break;
        case '\t':
            out.back().append(L"    ");
            break;
        default:
            out.back().push_back(result[i]);
            break;
        }
    }

    delete[] result;
    return out;
}

void cs::FileSystem::ExportText(wstring file, vector<wstring> lines) {
    wstring content;
    if (lines.size() == 0) { return FileSystem_ExportText(file.c_str(),content.c_str()); }

    for (size_t i = 0; i < lines.size(); i++) {
        content.append(lines[i]);
        content.push_back('\n');
    }
    content.pop_back();
    
    FileSystem_ExportText(file.c_str(),content.c_str());
}