#include "Console.hpp"

#include <iostream>
#include <vector>



using namespace cpp;

using namespace std;

int main() {

    Console::Symbol sym(L'\u2588');

    wcout << sym.character();

   
    while (true) {
        vector<vector<void*>> smbls;
        vector<void*> vsm;
        for (int i = 0; i < Console::GetWindowWidth(); i++)
        {
            vsm.push_back(sym.Get());
        }
        
        for (int i = 0; i < Console::GetWindowHeight(); i++)
        {
            smbls.push_back(vsm);
        }

        array<long unsigned int,2> out = Console::FillScreen(smbls);

        cout << out[0] << ' ' << out[1] << '\n';

        Sleep(1000);
    }
    
    cin.get();
    return 0;
}