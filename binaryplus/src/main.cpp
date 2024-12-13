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

#ifndef VK_CONTROL
    #define VK_CONTROL 10
#endif

#define IsCtrlDown() (Console::IsKeyDown(Key::Enum::CTRL) || Console::IsKeyDown(Key::Enum::CTRLL) || Console::IsKeyDown(Key::Enum::CTRLR))

using namespace std;
using namespace cpp;
using namespace cs;

wchar_t getChar(wchar_t current) {
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
    do {
        Console::HandleKeyboard();
    } while (!(Console::IsKeyDown(Key::Enum::ONE) || Console::IsKeyDown(Key::Enum::TWO) || Console::IsKeyDown(Key::Enum::THREE) || Console::IsKeyDown(Key::Enum::FOUR) || Console::IsKeyDown(Key::Enum::FIVE) || Console::IsKeyDown(Key::Enum::SIX) || Console::IsKeyDown(Key::Enum::SEVEN) || Console::IsKeyDown(Key::Enum::EIGHT) || Console::IsKeyDown(Key::Enum::NINE) || Console::IsKeyDown(Key::Enum::ZERO) || Console::IsKeyDown(Key::Enum::ESC)));
    wstring a = System::GetRootPath();
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
    
    bool edit = false;

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

    wstring a = System::GetRootPath();
    a.append(L"/assets/a.tux");
    wstring file = (argc < 2) ? getPath(System::GetRootPath() + L"/assets/a.tux") : wstring(argv[1]);
    auto texture = FileSystem::TextureFromFile(System::ToNativePath(file));
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
                file = getPath(L"./file.tux");
            }
        } else if (mouse.x > 20 && mouse.y == 0 && mouse.x < 27) {
            // LOAD
            for (int i = 21; i < 27; ++i) menu[0][i].ReverseColors();
            if (mouse.primary) {
                file = getPath(file);
                texture = FileSystem::TextureFromFile(file);
            }
        } else if (mouse.x > 27 && mouse.y == 0 && mouse.x < 34) {
            // EDIT/VIEW
            for (int i = 27; i < 34; ++i) menu[0][i].ReverseColors();
            if (mouse.primary) {
                edit = !edit;
                // change cursor or something
            }
        } else if (mouse.x > 34 && mouse.y == 0 && mouse.x < 41 ) {
            for (int i = 35; i < 41; ++i) menu[0][i].ReverseColors();
            if (mouse.primary) {
                system("wget https://raw.githubusercontent.com/stas-badzi/c-c-collab/refs/heads/main/README.md -O /tmp/.factoryrush/readme.md -o /dev/null");
                system("gnome-terminal -- less /tmp/.factoryrush/readme.md&");
                system("sleep 20");
                // change cursor or something
            }
        } else if (edit) {
            int line = mouse.y - 2,column = mouse.x - 2;
            if (line < 0 || column < 0) goto endminput;
            if (line >= height || column >= width) goto endminput;
            auto&& elem = screen[line][column];
            elem.character(symchar);
            elem.foreground(16);
            elem.background(16);
        }
endminput:

        FileSystem::DrawTextureToScreen(2,2,texture,screen); // już zaziała //to nie zadziala bo screen jest za maly;
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

        Console::FillScreen(screen);

        Control::CleanMemory();

        if (Console::IsFocused()) Console::HandleKeyboard();
        //string x;
        //gin >> x;
        //if (!gin.eof()) cout << 'a' << x << '\n' << flush;
        if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) return EXIT_SUCCESS;
        if (Console::KeyPressed() == Key::Enum::s && IsCtrlDown()) symchar = getChar(symchar);
        if (Console::KeyPressed() == Key::Enum::d && IsCtrlDown()) symback = symback;
        if (Console::KeyPressed() == Key::Enum::f && IsCtrlDown()) symfore = symfore;
        //FileSystem::DrawTextureToScreen(20,2,pos,screen);
    }
    return EXIT_FAILURE;
}