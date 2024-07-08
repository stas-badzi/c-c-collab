#include "Console.hpp"

#include <iostream>
#include <vector>



using namespace cpp;

using namespace std;

int main() {

    Console::Init();

    Console::Symbol sym0(L'#');
    Console::Symbol sym1(L' ');
   
    while (true) {
        //wcin >> sym.character;

        vector<vector<void*>> smbls;
        vector<void*> vsm0;
        for (int i = 0; i < Console::GetWindowWidth(); i++)
        {
            vsm0.push_back(sym0.Get());
        }
        vector<void*> vsm1 = vsm0;
        for (int i = 1; i < Console::GetWindowWidth() - 1; i++)
        {
            vsm1[i] = sym1.Get();
        }
        smbls.push_back(vsm0);
        for (int i = 2; i < Console::GetWindowHeight(); i++)
        {
            smbls.push_back(vsm1);
        }
        smbls.push_back(vsm0);

        array<long unsigned int,2> out = Console::FillScreen(smbls);

        cout << Console::GetWindowWidth() << ' ' << Console::GetWindowHeight() << '|' << vsm1.size() << ' ' << smbls.size() << '\n';

        


        SHORT state = GetKeyState('0');
        bool down = state < 0;
        bool toggle = (state & 1) != 0;


        if (down) { sym0.character(L'#'); } else { sym0.character(L'@'); }
    }
    return 0;
}