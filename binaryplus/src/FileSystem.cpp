#include "FileSystem.hpp"

#include "dllimport.hpp"

using namespace csimp;
using namespace std;

vector<wstring> cs::FileSystem::ImportText(wstring filename) {
    wchar_t* result = FileSystem_ImportText(filename.c_str());
    
    vector<wstring> out;
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
    return out;
}
