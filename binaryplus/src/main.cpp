#include "Console.hpp"
#include "TextureSystem.hpp"
#include "SoundSystem.hpp"
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

typedef std::basic_stringstream<char16_t> u16stringstream;

#define exit(x) cpp::Exit(x)
#define quick_exit(x) cpp::QuickExit(x)
#define _Exit(x) cpp::QuickExit(x)
#define to_u16string(x) uniconv::WStringToU16String(std::to_wstring(x))

#define IsCtrlDown() (Console::IsKeyDown(Key::Enum::CTRL) || Console::IsKeyDown(Key::Enum::CTRLL) || Console::IsKeyDown(Key::Enum::CTRLR))

using namespace std;
using namespace uniconv;
using namespace cpp;
using namespace cs;

wchar_t getChar(wchar_t current) {
    auto texture = Console::Symbol::CreateTexture(u"Press 0-9 to change the symbol\nESC to cancel\n...\n");
    Console::FillScreen(texture);
    do {
        Console::HandleKeyboard();
    } while (!( // Waiting for a key to be pressed
        Console::IsKeyDown(Key::Enum::ONE) || 
        Console::IsKeyDown(Key::Enum::TWO) || 
        Console::IsKeyDown(Key::Enum::THREE) || 
        Console::IsKeyDown(Key::Enum::FOUR) || 
        Console::IsKeyDown(Key::Enum::FIVE) || 
        Console::IsKeyDown(Key::Enum::SIX) || 
        Console::IsKeyDown(Key::Enum::SEVEN) || 
        Console::IsKeyDown(Key::Enum::EIGHT) || 
        Console::IsKeyDown(Key::Enum::NINE) ||
        Console::IsKeyDown(Key::Enum::ZERO) || 
        Console::IsKeyDown(Key::Enum::ESC)));

    // Parsing the key pressed into a symbol
    if (Console::IsKeyDown(Key::Enum::ONE)) return L'-';
    if (Console::IsKeyDown(Key::Enum::TWO)) return L'|';
    if (Console::IsKeyDown(Key::Enum::THREE)) return L'/';
    if (Console::IsKeyDown(Key::Enum::FOUR)) return L'$';
    if (Console::IsKeyDown(Key::Enum::FIVE)) return L'&';
    if (Console::IsKeyDown(Key::Enum::SIX)) return L'%';
    if (Console::IsKeyDown(Key::Enum::SEVEN)) return L'#';
    if (Console::IsKeyDown(Key::Enum::EIGHT)) return L'@';
    if (Console::IsKeyDown(Key::Enum::NINE)) return L'▒';
    if (Console::IsKeyDown(Key::Enum::ZERO)) return L' ';
    return current;
}
    

u16string getPath(u16string current) {
    return current;
    do {
        Console::HandleKeyboard();
    } while (!( // Waiting for a key to be pressed
        Console::IsKeyDown(Key::Enum::ONE) || 
        Console::IsKeyDown(Key::Enum::TWO) || 
        Console::IsKeyDown(Key::Enum::THREE) || 
        Console::IsKeyDown(Key::Enum::FOUR) || 
        Console::IsKeyDown(Key::Enum::FIVE) || 
        Console::IsKeyDown(Key::Enum::SIX) || 
        Console::IsKeyDown(Key::Enum::SEVEN) || 
        Console::IsKeyDown(Key::Enum::EIGHT) || 
        Console::IsKeyDown(Key::Enum::NINE) ||
        Console::IsKeyDown(Key::Enum::ZERO) || 
        Console::IsKeyDown(Key::Enum::ESC)));

    // Parsing the key pressed into a path
    u16string a = System::GetRootDir();
    if (Console::IsKeyDown(Key::Enum::ONE)) return a.append(u"/assets/one.tux");
    if (Console::IsKeyDown(Key::Enum::TWO)) return a.append(u"/assets/two.tux");
    if (Console::IsKeyDown(Key::Enum::THREE)) return a.append(u"/assets/three.tux");
    if (Console::IsKeyDown(Key::Enum::FOUR)) return a.append(u"/assets/four.tux");
    if (Console::IsKeyDown(Key::Enum::FIVE)) return a.append(u"/assets/five.tux");
    if (Console::IsKeyDown(Key::Enum::SIX)) return a.append(u"/assets/six.tux");
    if (Console::IsKeyDown(Key::Enum::SEVEN)) return a.append(u"/assets/seven.tux");
    if (Console::IsKeyDown(Key::Enum::EIGHT)) return a.append(u"/assets/eight.tux");
    if (Console::IsKeyDown(Key::Enum::NINE)) return a.append(u"/assets/nine.tux");
    if (Console::IsKeyDown(Key::Enum::ZERO)) return a.append(u"/assets/zero.tux");
    return current;
}

int Main(void) {
    Console::Init();
    Console::SetTitle(u"FactoryRush");
    Console::SetCursorSize(0);
    std::u16string buf;
    std::u16string res;
    std::vector<std::u16string> lines;
    Console::MouseStatus lastmouse = Console::GetMouseStatus();
    int siz = 0;
    while (1) {
        Console::HandleMouseAndFocus();
        Console::HandleOutput();
        wchar_t x;
        Console::Symbol sym = Console::Symbol(L'~',16,16);
        while ((x = win.get()) && win.good())
            if (x == L'\177' || x == '\b') {if (buf.size()) buf.pop_back();}
            else if (x == L'\t') buf.push_back(L' ');
            else if (x == L'\n' || x == L'\r') {lines.push_back(buf); buf.clear();}
            else buf.push_back(WCharToChar16(x));
        bool focs = Console::IsFocused();
        auto focsym = focs ? Console::Symbol(L'✓',16,16) : Console::Symbol(L'X',16,16);
        auto sym1 = Console::Symbol::CreateTexture(to_u16string(Console::GetMouseStatus().x));
        auto sym2 = Console::Symbol::CreateTexture(to_u16string(Console::GetMouseStatus().y));
        if (Console::GetMouseStatus().x == 0 && Console::GetMouseStatus().y == 4) {
            if (Console::GetMouseStatus().primary && !lastmouse.primary) return EXIT_SUCCESS; 
            if (Console::GetMouseStatus().secondary && !lastmouse.secondary) Console::PopupWindow(0,0,nullptr);
            if (Console::GetMouseStatus().middle && !lastmouse.middle) {
                std::vector<const char16_t*> bufs;
                for (auto&& line : lines) bufs.push_back(line.data());
                auto ret = Console::PopupWindow(lines.size()+1,lines.size(),bufs.data());
                if (!ret.has_value()) SoundSystem::PlaySound(System::ToNativePath(System::GetRootDir() + u"\\assets\\illegal-operation.wav"),1);
                else {
                    res = ret.value().second;
                }
            }
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
        screen.push_back(vector<Console::Symbol>());
        vector<vector<Console::Symbol>> sres = Console::Symbol::CreateTexture(res);
        for (size_t i = 0; i < sres.size(); i++) screen.push_back(sres[i]);
        Console::FillScreen(screen);
        Console::HandleKeyboard();
        if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) return EXIT_SUCCESS;
        Console::Sleep(0.03);
    }
    return 0;

    bool edit = false, bop = true, a3 = true;

    // FPS counting
    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
    chrono::time_point<chrono::high_resolution_clock> startsecs = chrono::high_resolution_clock::now();
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

        //TextureSystem::DrawTextureToScreen(20,2,pos,screen);
        */
        /**//* 
        if (max(mouse.y-1,0u) >= 0 && max(mouse.x-1,0u) >= 0 && max(mouse.y-1,0u) < height && max(mouse.x-1,0u) < width)
            screen[max(mouse.y-1,0u)][max(mouse.x-1,0u)].background(5);
        else
            screen[0][0].foreground(5);

        Console::FillScreen(screen);

        //Control::CleanMemory();

        auto enlapsed_time = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count();
        u16stringstream u16str;
        u16str << "[  " << enlapsed_time << "MPF  ]";
        auto pos = Console::Symbol::CreateTexture(u16str.str());
        Console::Sleep(1);
        Console::FillScreen(pos);
        Console::Sleep(1);
        start = chrono::high_resolution_clock::now();

        //*/
    //}
    //return 0;
    
    //*}

    // Getting the arguments
    int argc = Console::GetArgC();
    char16_t** argv = Console::GetArgV();

    u16string a = System::GetRootDir();
    a.append(u"/assets/a.tux");
    u16string file = (argc < 2) ? System::ToNativePath(getPath(System::GetRootDir() + u"/assets/a.tux")) : System::ToNativePath(u16string(argv[1])); // Load the texture to edit
    auto texture = TextureSystem::TextureFromFile(file); // Load the texture to edit
    char16_t symchar = getChar(L'~');
    uint8_t symback = 16;
    uint8_t symfore = 16;

    // START
    while (true) {
        // Get window area
        auto width = Console::GetWindowWidth();
        auto height = Console::GetWindowHeight();

        vector<vector<Console::Symbol>> screen;

        //u16string* characters = new u16string[(width+1)*height - 1]();
        //uint8_t* back = new uint8_t[width]();
        //uint8_t* fore = new uint8_t[width]();
        //po new jest delete;
        //ale jakby co masz vectory i vector.data() / vector.dat() nie pamietam
        for (int16_t l = 0; l < height; l++) { // Fill screen with empty symbols
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

        // Menu buttons

        auto menu = Console::Symbol::CreateTexture((edit ? u"| Quit | Save | New | Load | View | Help |" : u"| Quit | Save | New | Load | Edit | Help |"), array<uint8_t,70>{'\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7'}.data(), array<uint8_t,70>{'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'}.data());
        while (menu[0].size() < (size_t)width) menu[0].push_back({L' ','\0','\7'});

        Console::HandleMouseAndFocus();
        auto mouse = Console::GetMouseStatus();
        if (mouse.x > 0 && mouse.y == 0 && mouse.x < 7) {
            // QUIT
            for (int i = 1; i < 7; ++i) menu[0][i].ReverseColors();
            if (mouse.primary) {
                Control::CleanMemory();
                return EXIT_SUCCESS;
            }
        } else if (mouse.x > 7 && mouse.y == 0 && mouse.x < 14) {
            // SAVE
            for (int i = 8; i < 14; ++i) menu[0][i].ReverseColors();
            if (mouse.primary) {
                TextureSystem::FileFromTexture(file, texture);
            }
        } else if (mouse.x > 14 && mouse.y == 0 && mouse.x < 20) {
            // NEW
            for (int i = 15; i < 20; ++i) menu[0][i].ReverseColors();
            if (mouse.primary) {
                texture = Console::Symbol::CreateTexture(u"                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n                \n");
                file = System::ToNativePath(getPath(u"./file.tux"));
            }
        } else if (mouse.x > 20 && mouse.y == 0 && mouse.x < 27) {
            // LOAD
            for (int i = 21; i < 27; ++i) menu[0][i].ReverseColors();
            if (mouse.primary) {
                file = System::ToNativePath(getPath(file));
                texture = TextureSystem::TextureFromFile(file);
            }
        } else if (mouse.x > 27 && mouse.y == 0 && mouse.x < 34) {
            // EDIT/VIEW
            for (int i = 28; i < 34; ++i) menu[0][i].ReverseColors();
            if (mouse.primary && bop) {
                edit = !edit;
            }
        } else if (mouse.x > 34 && mouse.y == 0 && mouse.x < 41 ) {
            for (int i = 35; i < 41; ++i) menu[0][i].ReverseColors();
            if (mouse.primary && bop) {
                Console::PopupWindow(1,0,nullptr);
                //u16string fl = System::GetRootDir();
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
            if (mouse.primary) { // Edit mode
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

        TextureSystem::DrawTextureToScreen(2,2,texture,screen);
        TextureSystem::DrawTextureToScreen(0,0,menu,screen);

        // Count FPS
        auto enlapsed_time = chrono::duration_cast<std::chrono::duration<long double, std::milli>>(chrono::high_resolution_clock::now() - start).count();
        wstringstream u16str;
        if (counter == INT64_MAX) counter = 0;
        avg *= counter;
        avg += (1000.0/enlapsed_time);
        avg /= ++counter;
        u16str << L"[  " << avg << L"FPS  ]";
        auto pos = Console::Symbol::CreateTexture(WStringToU16String(u16str.str()));
        wout << u16str.str();
        start = chrono::high_resolution_clock::now();
        Console::HandleOutput();
        //TextureSystem::DrawTextureToScreen(2,15,pos,screen);

        // first check y 'cause if screen.size() is 0 than screen[0].size() will crash
        if (mouse.y < screen.size() && mouse.x < screen[0].size()) screen[mouse.y][mouse.x].character(Console::KeysToggled().CapsLock ? L'*' : screen[mouse.y][mouse.x].character());
        if ((!(Console::IsKeyDown(Key::Enum::SUPERL) || Console::IsKeyDown(Key::Enum::SUPERR))) && mouse.y < screen.size() && mouse.x < screen[0].size()) {
            if (screen[mouse.y][mouse.x].background() == 16) screen[mouse.y][mouse.x].background(0);
            if (screen[mouse.y][mouse.x].foreground() == 16) screen[mouse.y][mouse.x].foreground(7);
            screen[mouse.y][mouse.x].ReverseColors();
        }
        if (Console::KeyPressed() == Key::Enum::r && IsCtrlDown()) Console::ClearScreenBuffer();
        Console::FillScreen(screen);

        if (Console::IsFocused()) Console::HandleKeyboard();
        else Console::DontHandleKeyboard();
        //string x;
        //gin >> x;
        //if (!gin.eof()) cout << 'a' << x << '\n' << flush;
        if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) return EXIT_SUCCESS;
        if (Console::KeyPressed() == Key::Enum::n && IsCtrlDown()) Console::PopupWindow(0,0,nullptr);
        if (Console::KeyPressed() == Key::Enum::s && IsCtrlDown()) symchar = getChar(symchar);
        if (Console::KeyPressed() == Key::Enum::f && IsCtrlDown()) symfore = symfore;
        //TextureSystem::DrawTextureToScreen(20,2,pos,screen);
    }
    // END
    return EXIT_FAILURE;
}

int sub(int type) {
    assert(type != 0);
    auto&& sym = Console::Symbol::CreateTexture(u"▒frfjyyjyjt\n");
    Console::FillScreen(sym);
    Console::Sleep(10);
    do {Console::HandleMouseAndFocus();} while (!Console::IsFocused());
    auto argc = Console::GetArgC();
    if (argc == 1) Console::SetResult(to_u16string(type));
    else {
        auto args = Console::GetArgV();
        u16string result;
        wout << L"PopupWindow" << type << L':' << L' ';
        for (int i = 1; i < argc; ++i) {
            result.append(args[i]).push_back(u'\n');
            wout << U16StringToWString(args[i]) << L' ';
        }
        wout << L'\n';
        Console::HandleOutput();
        wout << U16StringToWString(result) << L'\n';
        Console::SetResult(result);
    }
    return type;
}
