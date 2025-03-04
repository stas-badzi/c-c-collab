#include "Console.hpp"
#include "FileSystem.hpp"
#include "System.hpp"
#include "Control.hpp"

#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <vector>
#include <chrono>
#include <cassert>  
#include <random>

#define IsCtrlDown() (Console::IsKeyDown(Key::Enum::CTRL) || Console::IsKeyDown(Key::Enum::CTRLL) || Console::IsKeyDown(Key::Enum::CTRLR))

using namespace std;
using namespace cpp;
using namespace cs;

wchar_t getChar(wchar_t current) {
    wstringstream wstr;
    wstr << "Press 0-9 to change the symbol\nESC to cancel\n...\n";
    auto texture = Console::Symbol::CreateTexture(wstr.str());
    Console::FillScreen(texture);
    do {
        Console::HandleKeyboard();
    } while (!(Console::IsKeyDown(Key::Enum::ONE) || Console::IsKeyDown(Key::Enum::TWO) || Console::IsKeyDown(Key::Enum::THREE) || Console::IsKeyDown(Key::Enum::FOUR) || Console::IsKeyDown(Key::Enum::FIVE) || Console::IsKeyDown(Key::Enum::SIX) || Console::IsKeyDown(Key::Enum::SEVEN) || Console::IsKeyDown(Key::Enum::EIGHT) || Console::IsKeyDown(Key::Enum::NINE) || Console::IsKeyDown(Key::Enum::ZERO) || Console::IsKeyDown(Key::Enum::ESC)));
    if (Console::IsKeyDown(Key::Enum::ZERO)) return L' ';
    if (Console::IsKeyDown(Key::Enum::ONE)) return L'-';
    if (Console::IsKeyDown(Key::Enum::TWO)) return L'|';
    if (Console::IsKeyDown(Key::Enum::THREE)) return L'/';
    if (Console::IsKeyDown(Key::Enum::FOUR)) return L'$';
    if (Console::IsKeyDown(Key::Enum::FIVE)) return L'&';
    if (Console::IsKeyDown(Key::Enum::SIX)) return L'%';
    if (Console::IsKeyDown(Key::Enum::SEVEN)) return L'#';
    if (Console::IsKeyDown(Key::Enum::EIGHT)) return L'@';
    if (Console::IsKeyDown(Key::Enum::NINE)) return L'▒';
    return current;
}
    

wstring getPath(wstring current) {
    return current;
    do {
        Console::HandleKeyboard();
    } while (!(Console::IsKeyDown(Key::Enum::ONE) || Console::IsKeyDown(Key::Enum::TWO) || Console::IsKeyDown(Key::Enum::THREE) || Console::IsKeyDown(Key::Enum::FOUR) || Console::IsKeyDown(Key::Enum::FIVE) || Console::IsKeyDown(Key::Enum::SIX) || Console::IsKeyDown(Key::Enum::SEVEN) || Console::IsKeyDown(Key::Enum::EIGHT) || Console::IsKeyDown(Key::Enum::NINE) || Console::IsKeyDown(Key::Enum::ZERO) || Console::IsKeyDown(Key::Enum::ESC)));
    wstring a = System::GetRootDir();
    if (Console::IsKeyDown(Key::Enum::ONE)) return a.append(L"/assets/one.tux");
    if (Console::IsKeyDown(Key::Enum::TWO)) return a.append(L"/assets/two.tux");
    if (Console::IsKeyDown(Key::Enum::THREE)) return a.append(L"/assets/three.tux");
    if (Console::IsKeyDown(Key::Enum::FOUR)) return a.append(L"/assets/four.tux");
    if (Console::IsKeyDown(Key::Enum::FIVE)) return a.append(L"/assets/five.tux");
    if (Console::IsKeyDown(Key::Enum::SIX)) return a.append(L"/assets/six.tux");
    if (Console::IsKeyDown(Key::Enum::SEVEN)) return a.append(L"/assets/seven.tux");
    if (Console::IsKeyDown(Key::Enum::EIGHT)) return a.append(L"/assets/eight.tux");
    if (Console::IsKeyDown(Key::Enum::NINE)) return a.append(L"/assets/nine.tux");
    if (Console::IsKeyDown(Key::Enum::ZERO)) return a.append(L"/assets/zero.tux");
    return current;
}

int main(void) {
    Console::Init();
    Console::SetTitle(L"FactoryRush");
    Console::SetCursorSize(0);
    std::wstring buf;
    Console::MouseStatus lastmouse = Console::GetMouseStatus();
    int siz = 0;
    while (1) {
        Console::HandleMouseAndFocus();
        wchar_t x;
        Console::Symbol sym = Console::Symbol(L'~',16,16);
        while ((x = gin.get()) && gin.good())
            if (isalnum(x)) buf.push_back(x);
            else if (x == L'\177') {if (buf.size()) buf.pop_back();}
            else wcout << x << endl;
        bool focs = Console::IsFocused();
        auto focsym = focs ? Console::Symbol(L'✓',16,16) : Console::Symbol(L'X',16,16);
        auto sym1 = Console::Symbol::CreateTexture(to_wstring(Console::GetMouseStatus().x));
        auto sym2 = Console::Symbol::CreateTexture(to_wstring(Console::GetMouseStatus().y));
        if (Console::GetMouseStatus().x == 0 && Console::GetMouseStatus().y == 4) {
            if (Console::GetMouseStatus().primary && !lastmouse.primary) return EXIT_SUCCESS; 
            if (Console::GetMouseStatus().secondary && !lastmouse.secondary) Console::PopupWindow(0,0,nullptr);
            if (Console::GetMouseStatus().middle && !lastmouse.middle) Console::PopupWindow(1,0,nullptr);
            sym = Console::Symbol(L'@',16,16);
        }
        Console::MoveCursor(Console::GetMouseStatus().x,Console::GetMouseStatus().y);
        if (Console::GetMouseStatus().secondary) Console::HideCursor();
        if (Console::GetMouseStatus().scroll.first) {
            sym = Console::GetMouseStatus().scroll.second ? Console::Symbol(L'▲',16,16) : Console::Symbol(L'▼',16,16);
            siz += 10*Console::GetMouseStatus().scroll.second-5;
            siz %= 100;
            Console::SetCursorSize(siz);
        }
        lastmouse = Console::GetMouseStatus();
        
        vector<vector<Console::Symbol>> screen = Console::Symbol::CreateTexture(buf);
        screen.push_back(sym1[0]);
        screen.push_back(sym2[0]);
        screen.push_back(vector<Console::Symbol>());
        screen.back().push_back(focsym);
        screen.push_back(vector<Console::Symbol>());
        screen.back().push_back(sym);
        Console::FillScreen(screen);
        Console::Sleep(0.03);
    }
    return 0;

    bool edit = false, bop = true, a3 = true;

    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
    long double avg = 0;
    long double old_avg = -1;
    unsigned long long counter = 0;
    //auto last_mouse = Console::GetMouseStatus();
    //while (true){
    /*
        uint16_t width = Console::GetWindowWidth();
        uint16_t height = Console::GetWindowHeight();

        vector<vector<Console::Symbol>> screen;
        for (int16_t l = 0; l < height; l++) {
            screen.push_back(vector<Console::Symbol>());
            for (int16_t i = 0; i < width; i++) {
                auto sym = Console::Symbol(L'▒',(uint8_t)16,(uint8_t)16);
                screen.back().push_back(sym);
            }
        }
    //*/
        //Console::HandleMouseAndFocus();
        //if (Console::IsFocused()) Console::HandleKeyboard();
        //if (Console::KeyPressed() != Key::Enum::NONE) cerr << (int)Console::KeyPressed() << '\n';
        //if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) return 0;
        /*
        auto mouse = Console::GetMouseStatus();
        if (0 <= mouse.x && mouse.x < width && 0 <= mouse.y && mouse.y < height) {
            screen[mouse.y][mouse.x].character(L'🖱');
            if (mouse.x > 0) screen[mouse.y][mouse.x-1].character(L'▋');
            if (mouse.x < width-1) { screen[mouse.y][mouse.x+1].character(L'▐'); screen[mouse.y][mouse.x+1].foreground(9); screen[mouse.y][mouse.x+1].background(3); }
        }
        //if (mouse.x != last_mouse.x || mouse.y != last_mouse.y || mouse.primary != last_mouse.primary || mouse.secondary != last_mouse.secondary || mouse.middle != last_mouse.middle || mouse.scroll.first != last_mouse.scroll.first || mouse.scroll.second != last_mouse.scroll.second)
        //    cerr << mouse.x << ' ' << mouse.y << '\n' << mouse.primary << ' ' << mouse.secondary << ' ' << mouse.middle << ' ' << mouse.scroll.first << ' ' << mouse.scroll.second << '\n';
        //last_mouse = mouse;
        //vector<vector<Console::Symbol>> sym = {1,{1,{L'▦',16,16}}}; 
        Console::FillScreen(screen);
        Console::Sleep(0.1);

        //FileSystem::DrawTextureToScreen(20,2,pos,screen);
        */
        /**//* 
        if (max(mouse.y-1,0u) >= 0 && max(mouse.x-1,0u) >= 0 && max(mouse.y-1,0u) < height && max(mouse.x-1,0u) < width)
            screen[max(mouse.y-1,0u)][max(mouse.x-1,0u)].background(5);
        else
            screen[0][0].foreground(5);

        Console::FillScreen(screen);

        //Control::CleanMemory();

        auto enlapsed_time = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count();
        wstringstream wstr;
        wstr << "[  " << enlapsed_time << "MPF  ]";
        auto pos = Console::Symbol::CreateTexture(wstr.str());
        Console::Sleep(1);
        Console::FillScreen(pos);
        Console::Sleep(1);
        start = chrono::high_resolution_clock::now();

        //*/
    //}
    //return 0;
    
    //*}

    int argc = Console::GetArgC();
    wchar_t** argv = Console::GetArgV();

    wstring a = System::GetRootDir();
    a.append(L"/assets/a.tux");
    wstring file = (argc < 2) ? System::ToNativePath(getPath(System::GetRootDir() + L"/assets/a.tux")) : System::ToNativePath(wstring(argv[1]));
    auto texture = FileSystem::TextureFromFile(file);
    wchar_t symchar = getChar(L'~');
    uint8_t symback = 16;
    uint8_t symfore = 16;
    while (true) {
        auto width = Console::GetWindowWidth();
        auto height = Console::GetWindowHeight();

        vector<vector<Console::Symbol>> screen;

        //wstring* characters = new wstring[(width+1)*height - 1]();
        //uint8_t* back = new uint8_t[width]();
        //uint8_t* fore = new uint8_t[width]();
        //po new jest delete;
        //ale jakby co masz vectory i vector.data() / vector.dat() nie pamietam
        for (int16_t l = 0; l < height; l++) {
            screen.push_back(vector<Console::Symbol>());
            for (int16_t i = 0; i < width; i++) {
                auto sym = Console::Symbol(L'▒',(uint8_t)16,(uint8_t)16);
                screen.back().push_back(sym);
            }
        }
        //delete[] characters; delete[] back; delete[] fore;
        //cerr << mouse.x << ' ' << mouse.y << '\n';
        //return 200;
        //screen[mouse.y][mouse.x].character(L'▦');

        //Console::FillScreen(screen);

        auto menu = Console::Symbol::CreateTexture((edit ? L"| Quit | Save | New | Load | View | Help |" : L"| Quit | Save | New | Load | Edit | Help |"), array<uint8_t,70>{'\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7'}.data(), array<uint8_t,70>{'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'}.data());
        while (menu[0].size() < (size_t)width) menu[0].push_back({L' ','\0','\7'});

        Console::HandleMouseAndFocus();
        auto mouse = Console::GetMouseStatus();
        if (mouse.x > 0 && mouse.y == 0 && mouse.x < 7) {
            // QUIT
            for (int i = 1; i < 7; ++i) menu[0][i].ReverseColors();
            if (mouse.primary) {
                Control::CleanMemory();
                Console::Fin();
                return EXIT_SUCCESS;
            }
        } else if (mouse.x > 7 && mouse.y == 0 && mouse.x < 14) {
            // SAVE
            for (int i = 8; i < 14; ++i) menu[0][i].ReverseColors();
            if (mouse.primary) {
                FileSystem::FileFromTexture(file, texture);
            }
        } else if (mouse.x > 14 && mouse.y == 0 && mouse.x < 20) {
            // NEW
            for (int i = 15; i < 20; ++i) menu[0][i].ReverseColors();
            if (mouse.primary) {
                texture = Console::Symbol::CreateTexture(L"                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n");
                file = System::ToNativePath(getPath(L"./file.tux"));
            }
        } else if (mouse.x > 20 && mouse.y == 0 && mouse.x < 27) {
            // LOAD
            for (int i = 21; i < 27; ++i) menu[0][i].ReverseColors();
            if (mouse.primary) {
                file = System::ToNativePath(getPath(file));
                texture = FileSystem::TextureFromFile(file);
            }
        } else if (mouse.x > 27 && mouse.y == 0 && mouse.x < 34) {
            // EDIT/VIEW
            for (int i = 28; i < 34; ++i) menu[0][i].ReverseColors();
            if (mouse.primary && bop) {
                edit = !edit;
                // change cursor or something
            }
        } else if (mouse.x > 34 && mouse.y == 0 && mouse.x < 41 ) {
            for (int i = 35; i < 41; ++i) menu[0][i].ReverseColors();
            if (mouse.primary && bop) {
                Console::PopupWindow(1,0,nullptr);
                //wstring fl = System::GetRootDir();
                //string fls = "gnome-terminal -- less ";
                //for (auto i = 0ul; i < fl.size(); ++i) fls.push_back(fl[i]);
                //fls.append("/share/factoryrush/assets/README.md");
                //system(fls.c_str());
                /*
                const char* arg1 = "/bin/su";
                const char* arg2 = "stas";
                const char* arg3 = "-c";
                const char* arg4 = fls.c_str(); // "gnome-terminal -- less " + fls + "\"" is one argument so put in one argument + not quotes !!!!!!
                char* const argv[] = {(char*)arg1, (char*)arg2, (char*)arg3, (char*)arg4, NULL};

                // ("su stas -c \"gnome-terminal -- less " + fls + "\"").c_str()
                pid_t pid;
                try {
                    posix_spawn(&pid,"/bin/su", nullptr, nullptr, argv, environ);
                    waitpid(pid, nullptr, 0);
                } catch (...) {
                    cerr << "error: " << strerror(errno) << '\n';
                    Console::Sleep(10);
                    return EXIT_FAILURE;
                }
*/
                //const char* arg21 = "/bin/sleep";
                //const char* arg22 = "20";
                //char* const argv2[] = {(char*)arg21, (char*)arg22, NULL};
                //posix_spawnp(&pid,"/bin/sleep", NULL, NULL, argv2, environ);
                // change cursor or something
            }
        } else if (edit) {
            if (mouse.primary) {
                int line = mouse.y - 2,column = mouse.x - 2;
                if (line < 0 || column < 0) goto endminput;
                if ((size_t)line >= texture.size() || (size_t)column >= texture[line].size()) goto endminput;
                auto&& elem = texture[line][column];
                elem.character(symchar);
                elem.foreground(symfore);
                elem.background(symback);
            }
        }
endminput:
        bop = !mouse.primary;

        FileSystem::DrawTextureToScreen(2,2,texture,screen);
        FileSystem::DrawTextureToScreen(0,0,menu,screen);

        auto enlapsed_time = chrono::duration_cast<std::chrono::duration<long double, std::milli>>(chrono::high_resolution_clock::now() - start).count();
        wstringstream wstr;
        if (counter == INT64_MAX) counter = 0;
        avg *= counter;
        avg += (1000.0/enlapsed_time);
        avg /= ++counter;
        wstr << "[  " << avg << "FPS  ]";
        auto pos = Console::Symbol::CreateTexture(wstr.str());
        start = chrono::high_resolution_clock::now();

        FileSystem::DrawTextureToScreen(2,15,pos,screen);

        // first check y 'cause if screen.size() is 0 than screen[0].size() will crash
        if (mouse.y < screen.size() && mouse.x < screen[0].size()) screen[mouse.y][mouse.x].character(Console::KeysToggled().CapsLock ? L'*' : screen[mouse.y][mouse.x].character());
        if ((!(Console::IsKeyDown(Key::Enum::SUPERL) || Console::IsKeyDown(Key::Enum::SUPERR))) && mouse.y < screen.size() && mouse.x < screen[0].size()) {
            if (screen[mouse.y][mouse.x].background() == 16) screen[mouse.y][mouse.x].background(0);
            if (screen[mouse.y][mouse.x].foreground() == 16) screen[mouse.y][mouse.x].foreground(7);
            screen[mouse.y][mouse.x].ReverseColors();
        }
        Console::FillScreen(screen);

        if (Console::IsFocused()) Console::HandleKeyboard();
        //string x;
        //gin >> x;
        //if (!gin.eof()) cout << 'a' << x << '\n' << flush;
        if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) return EXIT_SUCCESS;
        if (Console::KeyPressed() == Key::Enum::s && IsCtrlDown()) symchar = getChar(symchar);
        if (Console::KeyPressed() == Key::Enum::f && IsCtrlDown()) symfore = symfore;
        if (Console::KeyPressed() == Key::Enum::THREE && Console::IsKeyDown(Key::Enum::a) && a3) { Console::PopupWindow(0,0,nullptr); a3 = false; }
        if (!Console::IsKeyDown(Key::Enum::THREE)) a3 = true;
        //FileSystem::DrawTextureToScreen(20,2,pos,screen);

        Control::CleanMemory();
    }
    return EXIT_FAILURE;
}

int sub(int type) {
    assert(type != 0);
    auto&& sym = Console::Symbol::CreateTexture(L"▒frfjyyjyjt\n");
    Console::FillScreen(sym);
    Console::Sleep(10);
    return type;
}
