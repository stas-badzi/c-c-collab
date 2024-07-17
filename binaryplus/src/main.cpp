#include "Console.hpp"
#include "FileSystem.hpp"

#include <iostream>
#include <vector>
#include <string>



using namespace cpp;
using namespace cs;
using namespace std;

int main() {
    wstring filename = L"boo.dat";
    while (true) {
        //system("clear");
        vector<wstring> file = FileSystem::ImportText(filename);
        for (size_t i = 0; i < file.size(); i++) {
            wcout << file[i] << L'\n';
        }
        wcout << L"\nOverwrite file? [Y]\n\t";
        wchar_t del;
        wcin >> del;
        vector<wstring> out;
        if (del != L'Y' && del != L'y') {
            out = file;
        }
        out.push_back(wstring());
        wcin >> out.back();
        FileSystem::ExportText(filename,out);
    }

    Console::Init();

    Console::Symbol sym0(L'#');
    Console::Symbol sym1(L' ');
   
    while (true) {
        //wcin >> sym.character;

        const Console::Symbol csym0 = sym0;

        vector<vector<Console::Symbol>> smbls;
        vector<Console::Symbol> vsm0;
        int width = Console::GetWindowWidth();
        int height = Console::GetWindowHeight();

        for (int i = 0; i < width; i++)
        {
            vsm0.push_back(csym0);
        }
        vector<Console::Symbol> vsm1 = vsm0;
        for (int i = 1; i < width - 1; i++)
        {
            vsm1[i] = sym1;
        }
        smbls.push_back(vsm0);
        for (int i = 2; i < height; i++)
        {
            smbls.push_back(vsm1);
        }
        smbls.push_back(vsm0);

        array<long unsigned int,2> out = Console::FillScreen(smbls);

        cout << width << ' ' << height << '|' << vsm1.size() << ' ' << smbls.size() << '\n';

        


        //SHORT state = GetKeyState('0');
        //bool down = state < 0;
        //bool toggle = (state & 1) != 0;


        //if (down) { sym0.character(L'#'); } else { sym0.character(L'@'); }
    }
    return 0;
}