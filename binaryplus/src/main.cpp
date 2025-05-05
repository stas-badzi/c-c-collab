#include "Console.hpp"
#include "TextureSystem.hpp"
#include "SoundSystem.hpp"
#include "System.hpp"
#include "Control.hpp"
#include "Game.hpp"

#include <iostream>
#include <string>
#include <array>
#include <sstream>
#include <vector>
#include <chrono>
#include <cassert>
#include <random>
#include <thread>
#include <cwchar>
#if defined(_MSC_VER) || (defined(_WIN32) && defined(__aarch64__))
#define kill_thread(x) TerminateThread(x.native_handle(), 0)
#else
#include <pthread.h>
#include <signal.h>
#define kill_thread(x) pthread_kill(x.native_handle(), SIGTERM)
#endif

typedef std::basic_stringstream<char16_t> u16stringstream;

#define exit(x) cpp::Exit(x)
#define quick_exit(x) cpp::QuickExit(x)
#define _Exit(x) cpp::QuickExit(x)
#define to_u16string(x) uniconv::WStringToU16String(std::to_wstring(x))
#ifdef _WIN32
#define u16toi(x) _wtoi(uniconv::U16StringToWString(x).c_str())
#else
#define u16toi(x) wcstol(uniconv::U16StringToWString(x).c_str(),nullptr,10)
#endif
inline size_t u16strlen(const char16_t* str) { size_t i = 0; while (str[i]) i++; return i; }

#define IsCtrlDown() (Console::IsKeyDown(Key::Enum::CTRL) || Console::IsKeyDown(Key::Enum::CTRLL) || Console::IsKeyDown(Key::Enum::CTRLR))

using namespace std;
using namespace uniconv;
using namespace cpp;
using namespace cs;

wchar_t getChar(wchar_t current) {
    auto texture = Console::Symbol::CreateTexture(u"Press 0-9 to change the symbol\nESC to cancel\n...\n");
    do {
        Console::FillScreen(texture);
        Console::HandleMouseAndFocus();
        if (Console::IsFocused()) Console::HandleKeyboard();
        else Console::DontHandleKeyboard();
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
tryagain:
    const char16_t* argument[1] = {current.c_str()};
    auto res = Console::PopupWindow(7,1,argument,u"Open File");
    if (res.has_value())
        if (res.value().first == 0)
            return res.value().second;
        else {
            u16string arrmsg = u"\n  Popup Exited with code: " + to_u16string(res.value().first) +u"  \n";
            auto siz = arrmsg.size()-2;
            for (size_t i = 0; i < siz; i++) arrmsg = u' ' + arrmsg;
            for (size_t i = 0; i < siz; i++) arrmsg.push_back(u' ');
            argument[0] = arrmsg.c_str();
            auto res = Console::PopupWindow(1,1,argument,u"Error");
            if (!res.has_value() || res.value().first != 0)
                ThrowMsg(u"Error popup has not launched or exited properly");
        }
    else {
        u16string errmsg = u"Popup has not launched or exited properly";
        argument[0] = errmsg.c_str();
        auto res = Console::PopupWindow(1,1,argument,u"Error");
        if (!res.has_value() || res.value().first != 0)
            ThrowMsg(u"Error popup has not launched or exited properly");
    }
    goto tryagain;
}

pair<int,u16string> ErrorPopup(int argc, const char16_t* argv[]);

int _Main(void) {
    Console::Init();
    Console::SetTitle(u"FactoryRush");

    while (true) {

        auto width = Console::GetWindowWidth();
        auto height = Console::GetWindowHeight();

        vector<vector<Console::Symbol>> screen;

        for (int16_t l = 0; l < height; l++) {
            screen.push_back(vector<Console::Symbol>());
            for (int16_t i = 0; i < width; i++) {
                auto sym = Console::Symbol(L'▒',(uint8_t)16,(uint8_t)16);
                screen.back().push_back(sym);
            }
        }

        Game::Camera camera(9,9, Console::Symbol(L'a',(uint8_t)16,(uint8_t)16));
        camera.DrawToScreen(0, 0, screen);

        Console::FillScreen(screen);
        Console::Sleep(0.5);
    }
    return EXIT_SUCCESS;
}

int Main_(void) {
    Console::Init();
    Console::SetTitle(u"FactoryRush");
    Console::SetCursorSize(0);
    auto ret = Console::PopupWindowAsync(4,0,nullptr,u"Message Client");
    if (!ret.has_value()) return 1;
    auto retval = ret.value().first;
    auto pipe = ret.value().second;
    thread message_get([retval,pipe]() mutable {
        u16string text = u"Client Messages: \n";
        while (!retval.get().has_value()) {
            auto msg = Console::GetChildMessage(pipe);
            if (msg.has_value()) {
                text.append(msg.value());
            }
            auto scr = Console::Symbol::CreateTexture(text);
            Console::FillScreen(scr);
            Console::Sleep(0.3);
        }
    });
    while(!retval.get().has_value()) {
        Console::HandleMouseAndFocus();
        Console::HandleKeyboard();
        Console::HandleOutput();
        wchar_t x;
        while ((x = win.get()) && win.good())
            if (x == L'\177' || x == L'\b') {Console::SendChildMessage(pipe,u" backspace ");}
            else if (x == L'\n' || x == L'\r') {Console::SendChildMessage(pipe,u" enter ");}
            else if (x == L'\t') Console::SendChildMessage(pipe,u" tab ");
            else if (x == L'\033') Console::SendChildMessage(pipe,u" esc ");
            else if (x == L' ') Console::SendChildMessage(pipe,u" space ");
            else Console::SendChildMessage(pipe,WStringToU16String(std::wstring(1,x)));
        Console::Sleep(0.03);
    }
    message_get.join();
    return 0;
}

void ColorPopup(int argc, const char16_t* argv[]);

int Main(void) {
    Console::Init();
    Console::SetTitle(u"FactoryRush");

    while (true) {

        int width = Console::GetWindowWidth();
        int height = Console::GetWindowHeight();

        vector<vector<Console::Symbol>> screen(height, vector<Console::Symbol>(width, Console::Symbol(L'▒',(uint8_t)12,(uint8_t)16)));

        // ERROR HERE
        Game::Camera camera(9,9, Console::Symbol(L'a',(uint8_t)16,(uint8_t)11));
        camera.DrawToScreen(0, 0, screen);

        Console::FillScreen(screen);
        Console::Sleep(0.5);
    }
    return EXIT_SUCCESS;
}

int Main_(void) {
    Console::Init();
    Console::SetTitle(u"FactoryRush");
    Console::SetCursorSize(0);
    //ColorPopup(0,nullptr);
    
    /*
    std::u16string buf;
    std::u16string res;
    std::vector<std::u16string> lines;
    Console::MouseStatus lastmouse = Console::GetMouseStatus();
    int siz = 0;
    while (1) {
        Console::HandleMouseAndFocus();
        Console::HandleOutput();
        if (Console::IsFocused()) Console::HandleKeyboard();
        else Console::DontHandleKeyboard();
        if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown())
            break;

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
    */

    bool edit = false, bop = true, a3 = true;

    // FPS counting
    chrono::time_point<chrono::high_resolution_clock> start = chrono::high_resolution_clock::now();
    chrono::time_point<chrono::high_resolution_clock> startsecs = chrono::high_resolution_clock::now();
    long double avg = 0;
    long double old_avg = -1;
    unsigned long long counter = 0;

    char16_t brushlist[10] = {u' ',u'█',u'▓',u'▒',u'░',u'#',u'$',u'%',u'&',u'@'};
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

    Console::HandleKeyboard();

    u16string a = System::GetRootDir();
    a.append(u"/assets/a.tux");
    u16string file = (argc < 2) ? System::ToNativePath(getPath(u"./unnamed.tux")) : System::ToNativePath(u16string(argv[1])); // Load the texture to edit

    vector<vector<Console::Symbol>> texture;

    if (!System::IsFile(file)) {
        auto res = Console::PopupWindow(6,0,nullptr);
        if (!res.has_value() || res.value().first != 0) return EXIT_FAILURE;
        auto str = res.value().second;
        int nwidth = 0;
        int i; for (i = 0; str[i] != u'\n'; ++i) {
            assert(str[i] >= u'0' && str[i] <= u'9');
            nwidth *= 10;
            nwidth += str[i] - u'0';
        }
        int nheight = 0;
        for (++i; str[i] != u'\0'; ++i) {
            assert(str[i] >= u'0' && str[i] <= u'9');
            nheight *= 10;
            nheight += str[i] - u'0';
        }
        texture = vector<vector<Console::Symbol>>(nheight, vector<Console::Symbol>(nwidth, Console::Symbol()));
    } else texture = TextureSystem::TextureFromFile(file); // Load the texture to edit
    char16_t symchar = brushlist[1];
    uint8_t __symback = 16;
    uint8_t __symfore = 16;

    uint8_t symback = __symback, symfore = __symfore;
    rw_pipe_t color_pipe = {0,0};
    mutex color_lock;

    bool color_popup_active = false;
    stsb::promise<std::optional<std::pair<int, std::u16string>>>* color_popup_handle = nullptr;

    auto handle_color_popup = thread([&color_pipe,&__symback,&__symfore,&color_lock,&color_popup_active]() mutable {
        while (true) {
            color_lock.lock();
            if (color_popup_active) {
                auto local_pipe = color_pipe;
                color_lock.unlock();
                auto msg = Console::GetChildMessage(local_pipe);
                if (msg.has_value()) {
                    auto msgv = msg.value();
                    switch (msgv[0]) {
                    case L'F':
                        color_lock.lock();
                        __symfore = u16toi(msgv.substr(1).c_str());
                        color_lock.unlock();
                        break;
                    case L'B':
                        color_lock.lock();
                        __symback = u16toi(msgv.substr(1).c_str());
                        color_lock.unlock();
                        break;
                    }
                }
            } else color_lock.unlock();
            Console::Sleep(0.1);
        }
    });

    unsigned int frame = 0;
    pair<uint8_t,uint8_t> brush = pair<uint8_t,uint8_t>(16,16);
    // START
    while (true) {
        color_lock.lock();
        symfore = __symfore;
        symback = __symback;
        color_lock.unlock();

        // Get window area
        auto width = Console::GetWindowWidth();
        auto height = Console::GetWindowHeight();

        vector<vector<Console::Symbol>> screen;

        if (color_popup_active && color_popup_handle->get().has_value()) {
            color_popup_active = false;
            delete color_popup_handle;
        }

        auto colorpopupcreate = [&]() mutable {
            if (color_popup_active) return;
            const char16_t* argsx[2];
            u16string symforestr = to_u16string(symfore);
            u16string symbackstr = to_u16string(symback);
            argsx[0] = symforestr.c_str();
            argsx[1] = symbackstr.c_str();
            auto ret = Console::PopupWindowAsync(5,2,argsx,u"Color Picker");
            if (ret.has_value()) {
                auto [await,pipe] = ret.value();
                color_popup_handle = new auto(await);

                color_lock.lock();
                color_pipe = pipe;
                color_popup_active = true;
                color_lock.unlock();
            }
        };

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
        auto menu = Console::Symbol::CreateTexture((edit ? u"| Quit | Save | New | Open | View | Help |     ⦿   ✎|" : u"| Quit | Save | New | Open | Edit | Help |     ⦿   ✎|"), array<uint8_t,100>{'\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7','\7'}.data(), array<uint8_t,100>{'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0'}.data());
        for (int i=0; i<10; i++) menu.front().push_back(Console::Symbol(brushlist[i],0,15));
        menu.front().push_back(Console::Symbol(u' ',0,7));
        menu.front().push_back(Console::Symbol(u'✢',0,7));

        
        ++frame;
        if (frame % 100 == 0) {
            brush.first = rand() % 16;
        } if (frame % 100 == 50) {
            brush.second = rand() % 16;
        }
        menu.front().at(46).background(brush.first);
        menu.front().at(46).foreground(brush.second);
        menu.front().at(47).background(brush.first);
        menu.front().at(47).foreground(brush.second);
        menu.front().at(48).background(brush.first);
        menu.front().at(48).foreground(brush.second);
        
        while (menu.front().size() < (size_t)width) menu.front().push_back({L' ',0,7});

        Console::HandleMouseAndFocus();
        auto mouse = Console::GetMouseStatus();
        if (mouse.x > 0 && mouse.y == 0 && mouse.x < 7) {
            // QUIT
            for (int i = 1; i < 7; ++i) menu[0][i].ReverseColors();
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY) {
                Control::CleanMemory();
                kill_thread(handle_color_popup);
                handle_color_popup.join();
                return EXIT_SUCCESS;
            }
        } else if (mouse.x > 7 && mouse.y == 0 && mouse.x < 14) {
            // SAVE
            for (int i = 8; i < 14; ++i) menu[0][i].ReverseColors();
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY) {
                TextureSystem::FileFromTexture(file, texture);
            }
        } else if (mouse.x > 14 && mouse.y == 0 && mouse.x < 20) {
            // NEW
            for (int i = 15; i < 20; ++i) menu[0][i].ReverseColors();
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY) {
                auto res = Console::PopupWindow(6,0,nullptr);
                if (!res.has_value() || res.value().first != 0) continue;
                auto str = res.value().second;
                int nwidth = 0;
                int i; for (i = 0; str[i] != u'\n'; ++i) {
                    assert(str[i] >= u'0' && str[i] <= u'9');
                    nwidth *= 10;
                    nwidth += str[i] - u'0';
                }
                int nheight = 0;
                for (++i; str[i] != u'\0'; ++i) {
                    assert(str[i] >= u'0' && str[i] <= u'9');
                    nheight *= 10;
                    nheight += str[i] - u'0';
                }
                texture = vector<vector<Console::Symbol>>(nheight, vector<Console::Symbol>(nwidth, Console::Symbol()));
                auto dir = file;
                while (dir.size() && dir.back() != u'/' && dir.back() != u'\\') dir.pop_back();
                file = System::ToNativePath(getPath(dir));
            }
        } else if (mouse.x > 20 && mouse.y == 0 && mouse.x < 27) {
            // OPEN
            for (int i = 21; i < 27; ++i) menu[0][i].ReverseColors();
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY) {
                file = System::ToNativePath(getPath(file));
                if (!System::IsFile(file)) {
                auto res = Console::PopupWindow(6,0,nullptr);
                if (!res.has_value() || res.value().first != 0) return EXIT_FAILURE;
                    auto str = res.value().second;
                    int nwidth = 0;
                    int i; for (i = 0; str[i] != u'\n'; ++i) {
                        assert(str[i] >= u'0' && str[i] <= u'9');
                        nwidth *= 10;
                        nwidth += str[i] - u'0';
                    }
                    int nheight = 0;
                    for (++i; str[i] != u'\0'; ++i) {
                        assert(str[i] >= u'0' && str[i] <= u'9');
                        nheight *= 10;
                        nheight += str[i] - u'0';
                    }
                    texture = vector<vector<Console::Symbol>>(nheight, vector<Console::Symbol>(nwidth, Console::Symbol()));
                } else texture = TextureSystem::TextureFromFile(file);
            }
        } else if (mouse.x > 27 && mouse.y == 0 && mouse.x < 34) {
            // EDIT/VIEW
            for (int i = 28; i < 34; ++i) menu[0][i].ReverseColors();
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY) {
                edit = !edit;
            }
        } else if (mouse.x >= 46 && mouse.x <= 48 && mouse.y == 0){
            // COLOR
            menu[0][46].foreground(symfore);
            menu[0][46].background(symback);
            menu[0][47].foreground(symfore);
            menu[0][47].background(symback);
            menu[0][48].foreground(symfore);
            menu[0][48].background(symback);
            
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY)
                colorpopupcreate();
        } else if ((mouse.x == 51 || mouse.x == 52) && mouse.y == 0){
            // CHANGE SYMBOL TO CUSTOM
            menu[0][51].character(symchar);
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY) {
                char16_t* args[1] = {(char16_t*)(calloc(2,sizeof(char16_t)))};
                args[0][0] = symchar;
                auto ret = Console::PopupWindow(2,1,(const char16_t**)args,u"Enter Character");
                if (ret.has_value()) {
                    auto res = ret.value();
                    if (res.first == 0)
                        symchar = res.second[0];
                }
            }
        } else if (mouse.x >= 53 && mouse.x < 63 && mouse.y == 0) {
            // CHANGE SYMBOL FROM CHART
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY)
                symchar =  brushlist[mouse.x - 53];
        } else if (mouse.x == 64 || mouse.x == 65 && mouse.y == 0) {
            // EDIT CHART
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY) {
                u16string charstr;
                for (int i = 0; i < 10; i++) charstr.push_back(brushlist[i]);
                const char16_t* args[1] = {charstr.c_str()};
                auto res = Console::PopupWindow(3,1,args,u"Edit Quick Symbol Picker");
                if (res.has_value()) {
                    auto ret = res.value();
                    if (ret.first == 0)
                        for (int i = 0; i < 10; i++) brushlist[i] = ret.second[i];
                }
            }
        } else if (mouse.x > 34 && mouse.y == 0 && mouse.x < 41 ) {
            for (int i = 35; i < 41; ++i) menu[0][i].ReverseColors();
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY) {
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
            if (Console::IsMouseButtonDown(MOUSE_BUTTON_PRIMARY)) { // Edit mode
                int line = mouse.y - 2,column = mouse.x - 2;
                if (line < 0 || column < 0) goto endminput;
                if ((size_t)line >= texture.size() || (size_t)column >= texture[line].size()) goto endminput;
                auto&& elem = texture[line][column];
                elem.character(symchar);
                elem.foreground(symfore);
                elem.background(symback);
            } else if (Console::IsMouseButtonDown(MOUSE_BUTTON_SECONDARY)) { // Edit mode
                int line = mouse.y - 2,column = mouse.x - 2;
                if (line < 0 || column < 0) goto endminput;
                if ((size_t)line >= texture.size() || (size_t)column >= texture[line].size()) goto endminput;
                auto&& elem = texture[line][column];
                elem.character(u' ');
                elem.foreground(Color::DEFAULT);
                elem.background(Color::DEFAULT);
            }
        }
endminput:

        TextureSystem::DrawTextureToScreen(2,2,texture,screen);
        TextureSystem::DrawTextureToScreen(0,0,menu,screen);

        // Count FPS
        auto enlapsed_time = chrono::duration_cast<std::chrono::duration<long double, std::milli>>(chrono::high_resolution_clock::now() - start).count();
        if (enlapsed_time < 10) Console::Sleep((10-enlapsed_time)*1e-3,true);
        enlapsed_time = chrono::duration_cast<std::chrono::duration<long double, std::milli>>(chrono::high_resolution_clock::now() - start).count();
        wstringstream u16str;
        if (counter == INT64_MAX) counter = 0;
        avg *= counter;
        avg += (1000.0/enlapsed_time);
        avg /= ++counter;
        u16str << L"[  " << avg << L"FPS  ]";
        auto pos = Console::Symbol::CreateTexture(WStringToU16String(u16str.str()));
        //wout << u16str.str();
        start = chrono::high_resolution_clock::now();
        Console::HandleOutput();
        //TextureSystem::DrawTextureToScreen(2,15,pos,screen);

        // first check y 'cause if screen.size() is 0 than screen[0].size() will crash
        if (mouse.y < screen.size() && mouse.x < screen[0].size()) screen[mouse.y][mouse.x].character(Console::KeysToggled().CapsLock ? L'*' : screen[mouse.y][mouse.x].character());
        if ((!(Console::IsKeyDown(Key::Enum::SUPERL) || Console::IsKeyDown(Key::Enum::SUPERR))) && mouse.y < screen.size() && mouse.x < screen[0].size()) {
            if (screen[mouse.y][mouse.x].background() == 16) screen[mouse.y][mouse.x].background(0);
            if (screen[mouse.y][mouse.x].foreground() == 16) screen[mouse.y][mouse.x].foreground(7);
            Console::ShowCursor();
            Console::MoveCursor(mouse.x,mouse.y);
        } else Console::HideCursor();
        if (Console::KeyPressed() == Key::Enum::r && IsCtrlDown()) Console::ClearScreenBuffer();
        Console::FillScreen(screen);

        if (Console::IsFocused()) {
            Console::HandleKeyboard();
            //cerr << "Keypressed: " << (uint16_t)Console::KeyPressed() << '\n';
        }
        else Console::DontHandleKeyboard();
        //string x;
        //gin >> x;
        //if (!gin.eof()) cout << 'a' << x << '\n' << flush;
        if (Console::KeyPressed() == Key::Enum::r && IsCtrlDown()) Console::ClearScreenBuffer();

        if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) {
            kill_thread(handle_color_popup);
            handle_color_popup.join();
            return EXIT_SUCCESS;
        }
        if (Console::KeyPressed() == Key::Enum::n && IsCtrlDown()) Console::PopupWindow(0,0,nullptr);
        if (Console::KeyPressed() == Key::Enum::s && IsCtrlDown()) symchar = getChar(symchar);

        if (Console::KeyPressed() == Key::Enum::f && IsCtrlDown())
            colorpopupcreate();
        //TextureSystem::DrawTextureToScreen(20,2,pos,screen);
    }
    // END
    kill_thread(handle_color_popup);

    handle_color_popup.join();
    return EXIT_FAILURE;
}

pair<int,u16string> ErrorPopup(int argc, const char16_t* argv[]) {
    Console::SetTitle(u"Error");
    u16string err = (argc < 2) ? u"Unkown Error" : u16string(argv[1]);
    wout << U16StringToWString(err) << L'\n'; flush();
    auto lastmouse = Console::GetMouseStatus();
    auto old_width = Console::GetWindowWidth();
    auto old_height = Console::GetWindowHeight();
    int nothingticks = 0;
    while (1) {
        auto height = Console::GetWindowHeight(); auto width = Console::GetWindowWidth();
        if (height < 3 || width < 3) {
            Console::Sleep(0.1);
            continue;
        }
        vector<vector<Console::Symbol>> scr;
        int i; for (i = 0; i < (height*2/3); i++)
            scr.push_back(vector<Console::Symbol>());
        for (int j = 0; j < (width/2)-1; j++) {
            scr.back().push_back(Console::Symbol(L' ',16,16));
            scr[i-2].push_back(Console::Symbol(L' ',16,16));
        }
        scr[i-2].push_back(Console::Symbol(L' ',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr.back().push_back(Console::Symbol(L'|',16,16));
        pair<uint16_t,uint16_t> OK = { scr.back().size(), scr.size()-1 };
        scr.back().push_back(Console::Symbol(L'O',15,0));
        scr.back().push_back(Console::Symbol(L'K',15,0));
        scr.back().push_back(Console::Symbol(L'|',16,16));
        
        scr.push_back(vector<Console::Symbol>());
        for (int j = 0; j < (width/2); j++)
            scr.back().push_back(Console::Symbol(L' ',16,16));
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        while (++i < height)
            scr.push_back(vector<Console::Symbol>());

        uint8_t* back = new uint8_t[err.size()];
        for (size_t j = 0; j < err.size(); j++)
            back[j] = 9;
        uint8_t* fore = new uint8_t[err.size()];
        for (size_t j = 0; j < err.size(); j++)
            fore[j] = 15;
        auto sym = Console::Symbol::CreateTexture(err,back,fore);
        for (size_t j = height/3; j < (unsigned short)height; j++)
            while (scr[j].size() < (uint16_t)width)
                scr[j].push_back(Console::Symbol());
        TextureSystem::DrawTextureToScreen((width/2)+1-(sym[0].size()/2),height/3,sym,scr);

        Console::HandleMouseAndFocus();
        if (Console::IsFocused()) Console::HandleKeyboard();
        else Console::DontHandleKeyboard();
        if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) break;
        if (Console::KeyPressed() == Key::Enum::ENTER) break;
        if (Console::KeyPressed() == Key::Enum::ESC) return {EXIT_FAILURE,u""};
        auto mouse = Console::GetMouseStatus();
        if ( (mouse.x == OK.first || mouse.x == OK.first + 1u ) && mouse.y == OK.second) {
            scr[OK.second][OK.first].ReverseColors();
            scr[OK.second][OK.first+1].ReverseColors();
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY)
                break;
        }
        Console::FillScreen(scr);
        Console::MoveCursor(mouse.x,mouse.y);
        seeifsleep:
        if (width != Console::GetWindowWidth() || height != Console::GetWindowHeight()) {
            old_width = Console::GetWindowWidth();
            old_height = Console::GetWindowHeight();
            lastmouse = mouse;
            nothingticks = 0;
            continue;
        }
        if ((Console::MouseButtonClicked().first == 0 && Console::MouseButtonReleased() == 0 && Console::KeyPressed() == Key::Enum::NONE && old_width == width && old_height == height)) 
            nothingticks++;
        else
            nothingticks = 0;
        if (!Console::IsFocused() || nothingticks > 100) {
        yessleep:
            lastmouse = mouse;
            old_width = width;
            old_height = height;
            Console::Sleep(0.1,true);
            Console::HandleMouseAndFocus();
            if (!Console::IsFocused()) {
                Console::DontHandleKeyboard();
                goto yessleep;
            }
            Console::HandleKeyboard();
            if (Console::KeyPressed() != Key::Enum::NONE) continue;
            goto seeifsleep;
        } else {
            Console::Sleep(0.01);
            lastmouse = mouse;
            old_width = width;
            old_height = height;
        }
    }
    return {EXIT_SUCCESS,u""};
}

pair<int,u16string> GetFilePopup(int argc, const char16_t* argv[]) {
    Console::SetTitle(u"Open File");

    u16string path = (argc < 2) ? u"./" : u16string(argv[1]);

    vector<vector<Console::Symbol>> scr;
    int last_width = 0;
    int last_height = 0;
    int begx = 0, begy = 0, endx = -1, endy = -1;
    while (1) {
        bool newscr = false;

        Console::HandleMouseAndFocus();
        if (Console::IsFocused()) Console::HandleKeyboard();
        else Console::DontHandleKeyboard();
        if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) return {EXIT_FAILURE,(argc < 2) ? u"." : u16string(argv[1])};
        if (Console::KeyPressed() == Key::Enum::ESC) return {EXIT_FAILURE,(argc < 2) ? u"." : u16string(argv[1])};

        auto mouse = Console::GetMouseStatus();
        if (mouse.y >= begy && mouse.y <= endy && mouse.x >= begx && mouse.x <= endx) {
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY)
                return {EXIT_SUCCESS,path};
            else if (Console::MouseButtonClicked().first == MOUSE_BUTTON_SECONDARY)
                return {EXIT_FAILURE,(argc < 2) ? u"." : u16string(argv[1])};
        }

        wchar_t x;
        while ((x = win.get()) && win.good()) {
            newscr = true;
            if (x == L'\177' || x == L'\b') {if (path.size()) path.pop_back();}
            else if (x == L'\n' || x == L'\r') return {EXIT_SUCCESS,path};
            else path.push_back(WCharToChar16(x));
        }

        if (last_width != Console::GetWindowWidth() || last_height != Console::GetWindowHeight()) {
            last_width = Console::GetWindowWidth();
            last_height = Console::GetWindowHeight();
            newscr = true;
        }
        if (last_height < 3 || last_width < 3) {
            Console::Sleep(0.1,true);
            continue;
        }
        if (newscr) {
            scr.clear();
            int i;
            for (i = 0; i < (last_height/2); i++) {
                scr.push_back(vector<Console::Symbol>());
            }
            size_t j;
            for (j = 0; j <= (unsigned short)(last_width/10); j++)
                scr.back().push_back(Console::Symbol(L' ',16,16));
            while (++j < (unsigned short)(last_width - (last_width/10)))
                scr.back().push_back(Console::Symbol(L' ',Color::DEFAULT,Color::LIGHT_BLACK));
            
            scr.push_back(vector<Console::Symbol>());
            for (j = 0; j <= (unsigned short)(last_width/10); j++)
                scr.back().push_back(Console::Symbol(L' ',16,16));

            auto ppath = path;
            if (path.size() > (size_t)last_width - 2*((size_t)(last_width/10))) {
                while (ppath.size() + 3 > (size_t)last_width - 2*((size_t)(last_width/10)))
                    ppath = ppath.substr(1);
                ppath = u"..." + ppath;
            }

            while (++j <= (last_width/10) + (((size_t)last_width - 2*((size_t)(last_width/10)) - path.size())/2))
                scr.back().push_back(Console::Symbol(L' ',Color::DEFAULT,Color::LIGHT_BLACK));

            begx = scr.back().size();
            begy = scr.size() - 1;
            --j; for (auto&& c : ppath) {
                scr.back().push_back({ c,Color::DEFAULT,Color::LIGHT_BLACK });
                ++j;
            }
            endx = scr.back().size() - 1;

            while (++j < (unsigned short)(last_width - (last_width/10)))
                scr.back().push_back(Console::Symbol(L' ',Color::DEFAULT,Color::LIGHT_BLACK));

            scr.push_back(vector<Console::Symbol>());
            for (j = 0; j <= (unsigned short)(last_width/10); j++)
                scr.back().push_back(Console::Symbol(L' ',16,16));
            while (++j < (unsigned short)(last_width - (last_width/10)))
                scr.back().push_back(Console::Symbol(L' ',Color::DEFAULT,Color::LIGHT_BLACK));
            endy = scr.size() - 1;

            Console::FillScreen(scr);
        }
        Console::Sleep(0.01);
    }
}

pair<int,u16string> NewFilePopup(int argc, const char16_t* argv[]) {
    Console::SetTitle(u"New File");
    int flwidth = 20;
    int flheight = 10;
    auto lastmouse = Console::GetMouseStatus();
    auto old_width = Console::GetWindowWidth();
    auto old_height = Console::GetWindowHeight();
    int nothingticks = 0;
    bool first = true;
    while (1) {
        flwidth = max<int16_t>(0,flwidth);
        flheight = max<int16_t>(0,flheight);

        auto height = Console::GetWindowHeight(); auto width = Console::GetWindowWidth();
        height = max<int16_t>(height,4); width = max<int16_t>(width,15);
        vector<vector<Console::Symbol>> scr;
        int i; for (i = 0; i < (height*3/4); i++)
            scr.push_back(vector<Console::Symbol>());
        for (int j = 0; j < (width/3)-5; j++) {
            scr.back().push_back(Console::Symbol(L' ',16,16));
            scr[i-2].push_back(Console::Symbol(L' ',16,16));
        }
        scr[i-2].push_back(Console::Symbol(L' ',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr[i-2].push_back(Console::Symbol(L' ',16,16));

        scr.back().push_back(Console::Symbol(L'|',16,16));
        pair<uint16_t,uint16_t> CANCEL = { scr.back().size(), scr.size()-1 };
        scr.back().push_back(Console::Symbol(L' ',15,0));
        scr.back().push_back(Console::Symbol(L'C',15,0));
        scr.back().push_back(Console::Symbol(L'A',15,0));
        scr.back().push_back(Console::Symbol(L'N',15,0));
        scr.back().push_back(Console::Symbol(L'C',15,0));
        scr.back().push_back(Console::Symbol(L'E',15,0));
        scr.back().push_back(Console::Symbol(L'L',15,0));
        scr.back().push_back(Console::Symbol(L' ',15,0));
        scr.back().push_back(Console::Symbol(L'|',16,16));
        

        for (int j = scr.back().size(); j < (width*2/3)-3; j++) {
            scr.back().push_back(Console::Symbol(L' ',16,16));
            scr[i-2].push_back(Console::Symbol(L' ',16,16));
        }

        scr[i-2].push_back(Console::Symbol(L' ',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr[i-2].push_back(Console::Symbol(L'_',16,16));
        scr[i-2].push_back(Console::Symbol(L' ',16,16));
        scr.back().push_back(Console::Symbol(L'|',16,16));
        pair<uint16_t,uint16_t> OK = { scr.back().size(), scr.size()-1 };
        scr.back().push_back(Console::Symbol(L' ',15,0));
        scr.back().push_back(Console::Symbol(L'O',15,0));
        scr.back().push_back(Console::Symbol(L'K',15,0));
        scr.back().push_back(Console::Symbol(L' ',15,0));
        scr.back().push_back(Console::Symbol(L'|',16,16));
        
        scr.push_back(vector<Console::Symbol>());
        for (int j = 0; j < (width/3)-5; j++)
            scr.back().push_back(Console::Symbol(L' ',16,16));

        scr.back().push_back(Console::Symbol(L' ',16,16));  
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        scr.back().push_back(Console::Symbol(L' ',16,16));
        
        for (int j = scr.back().size(); j < (width*2/3)-3; j++)
            scr.back().push_back(Console::Symbol(L' ',16,16));

        scr.back().push_back(Console::Symbol(L' ',16,16));
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        scr.back().push_back(Console::Symbol(L'‾',16,16));
        scr.back().push_back(Console::Symbol(L' ',16,16));

        while (++i < height)
            scr.push_back(vector<Console::Symbol>());

        auto wistr = u"width: " + to_u16string(flwidth);
        auto hestr = u"height: " + to_u16string(flheight);

        auto wbegin = max<size_t>(3,width/2-wistr.size()/2);
        auto hbegin = max<size_t>(3,width/2-hestr.size()/2);

        for (size_t j = 0; j < wbegin-(first*3); j++)
            scr[height/4].push_back(Console::Symbol());
        
        if (first) {
            scr[height/4].push_back(Console::Symbol(u'-',16,16));
            scr[height/4].push_back(Console::Symbol(u'>',16,16));
            scr[height/4].push_back(Console::Symbol(u' ',16,16));
        }
  
        pair<uint16_t,uint16_t> wbeg = { scr[height/4].size(), height/4 };
        for (size_t j = 0; j < wistr.size(); j++)
            scr[height/4].push_back(Console::Symbol(wistr[j],15,0));

        for (size_t j = 0; j < hbegin-3+(first*3); j++)
            scr[height/2].push_back(Console::Symbol());

        if (!first) {
            scr[height/2].push_back(Console::Symbol(u'-',16,16));
            scr[height/2].push_back(Console::Symbol(u'>',16,16));
            scr[height/2].push_back(Console::Symbol(u' ',16,16));
        }
        
        pair<uint16_t,uint16_t> hbeg = { scr[height/2].size(), height/2 };
        for (size_t j = 0; j < hestr.size(); j++)
            scr[height/2].push_back(Console::Symbol(hestr[j],15,0));

        Console::HandleMouseAndFocus();
        auto mouse = Console::GetMouseStatus();
        if (Console::IsFocused()) Console::HandleKeyboard();
        else Console::DontHandleKeyboard();
        if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) break;
        if (Console::KeyPressed() == Key::Enum::ENTER) break;
        if (Console::KeyPressed() == Key::Enum::ESC) return {EXIT_FAILURE,u""};
        if (Console::KeyPressed() == Key::Enum::TAB) first = !first;
        if (Console::KeyPressed() == Key::Enum::ARROW_UP || Console::KeyPressed() == Key::Enum::PLUS || Console::KeyPressed() == Key::Enum::EQUAL || Console::IsMouseButtonDown(MOUSE_SCROLL_UP)) { // + isn't on most keyboards, but is under the = sign
            if (first) ++flwidth;
            else ++flheight;
        } else if (Console::KeyPressed() == Key::Enum::ARROW_DOWN || Console::KeyPressed() == Key::Enum::MINUS || Console::IsMouseButtonDown(MOUSE_SCROLL_DOWN)) {
            if (first) --flwidth;
            else --flheight;
        }
        if ( (mouse.x == OK.first || mouse.x == OK.first + 1u || mouse.x == OK.first + 2u || mouse.x == OK.first + 3u) && mouse.y == OK.second) {
            scr[OK.second][OK.first].ReverseColors();
            scr[OK.second][OK.first+1].ReverseColors();
            scr[OK.second][OK.first+2].ReverseColors();
            scr[OK.second][OK.first+3].ReverseColors();
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY)
                break;
        }
        if ( (mouse.x == CANCEL.first || mouse.x == CANCEL.first + 1u || mouse.x == CANCEL.first + 2u || mouse.x == CANCEL.first + 3u || mouse.x == CANCEL.first + 4u || mouse.x == CANCEL.first + 5u || mouse.x == CANCEL.first + 6u || mouse.x == CANCEL.first + 7u) && mouse.y == CANCEL.second) {
            scr[CANCEL.second][CANCEL.first].ReverseColors();
            scr[CANCEL.second][CANCEL.first+1].ReverseColors();
            scr[CANCEL.second][CANCEL.first+2].ReverseColors();
            scr[CANCEL.second][CANCEL.first+3].ReverseColors();
            scr[CANCEL.second][CANCEL.first+4].ReverseColors();
            scr[CANCEL.second][CANCEL.first+5].ReverseColors();
            scr[CANCEL.second][CANCEL.first+6].ReverseColors();
            scr[CANCEL.second][CANCEL.first+7].ReverseColors();
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY)
                return {EXIT_FAILURE,u""};
        }

        if (mouse.y == wbeg.second && (mouse.x >= wbeg.first && mouse.x < wbeg.first + wistr.size())) {
            for (size_t i = 0; i < wistr.size(); ++i){
                scr[wbeg.second][wbeg.first + i].ReverseColors();
            }
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY)
                first = true;
        }

        if (mouse.y == hbeg.second && (mouse.x >= hbeg.first && mouse.x < hbeg.first + hestr.size())) {
            for (size_t i = 0; i < hestr.size(); ++i){
                scr[hbeg.second][hbeg.first + i].ReverseColors();
            }
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY)
                first = false;
        }

        if (win.good()) {
            wchar_t x;
            while ((x = win.get()) && win.good())
                if (x == L'\177' || x == L'\b')
                    if (first) flwidth/=10;
                    else flheight/=10;
                else if (x >= L'0' && x <= L'9') {
                    if (first) {
                        flwidth *= 10;
                        flwidth += x - L'0';
                    } else {
                        flheight *= 10;
                        flheight += x - L'0';
                    }
                }
        }


        Console::FillScreen(scr);
        Console::MoveCursor(mouse.x,mouse.y);
        if (width != Console::GetWindowWidth() || height != Console::GetWindowHeight()) {
            old_width = Console::GetWindowWidth();
            old_height = Console::GetWindowHeight();
            lastmouse = mouse;
            nothingticks = 0;
            continue;
        }
        if ((Console::MouseButtonClicked().first == 0 && Console::MouseButtonReleased() == 0 && Console::KeyPressed() == Key::Enum::NONE && old_width == width && old_height == height)) 
            nothingticks++;
        else
            nothingticks = 0;
        seeifsleep:
        if (!Console::IsFocused() || nothingticks > 100) {
        yessleep:
            lastmouse = mouse;
            old_width = width;
            old_height = height;
            Console::Sleep(0.1,true);
            Console::HandleMouseAndFocus();
            if (!Console::IsFocused()) {
                Console::DontHandleKeyboard();
                goto yessleep;
            }
            Console::HandleKeyboard();
            if (Console::KeyPressed() != Key::Enum::NONE) continue;
            goto seeifsleep;
        } else {
            Console::Sleep(0.01);
            lastmouse = mouse;
            old_width = width;
            old_height = height;
        }
    }
    return {EXIT_SUCCESS,to_u16string(max(1,flwidth)) + u'\n' + to_u16string(max(1,flheight))};
}

pair<int,u16string> MsgPopup(int argc, const char16_t* argv[]) {
    Console::SetTitle(u"Message Client");

    int last_width = 0;
    int last_height = 0;
    u16string text = u"Messages:\n";
    bool done = false;
    thread sendmsg_thread([done]() {
        u16string text;
        while (!done) {
            Console::HandleMouseAndFocus();
            //if (Console::IsFocused()) Console::HandleKeyboard();
            //else Console::DontHandleKeyboard();
            if (win.good()) {
                wchar_t x;
                while ((x = win.get()) && win.good())
                    if (x == L'\177' || x == L'\b') {if (text.size()) text.pop_back();}
                    else if (x == L'\n' || x == L'\r') {Console::SendParentMessage(text); text.clear();}
                    else if (x == L'\t') text = u'\t' + text;
                    else if (x == L'\033') text.clear();
                    else text.push_back(WCharToChar16(x));
            }
        }
    });

    while (1) {
        auto msg = Console::GetParentMessage();
        if (msg.has_value()) {
            text.append(msg.value());
        }
        auto scr = Console::Symbol::CreateTexture(text);
        Console::FillScreen(scr);
        Console::Sleep(1,true);
        Console::HandleKeyboard();
        if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) break;
    }
    done = true;
    sendmsg_thread.join();
    Console::Fin();
    Console::Sleep(60);
    return {EXIT_SUCCESS,u""};
}

void ColorPopup(int argc, const char16_t* argv[]) {
    Console::SetTitle(u"Color Picker");
    Console::SetCursorSize(100);
    Console::HideCursor();
    pair<uint8_t,uint8_t> color = pair<uint8_t,uint8_t>(u16toi(argv[1]),u16toi(argv[2]));

    bool curon = false;
    pair<uint16_t,uint16_t> lastcurpos = pair<uint16_t,uint16_t>(-1,-1);

    while (1) {
        auto width = Console::GetWindowWidth();
        auto height = Console::GetWindowHeight();

        if (width < 25 || height < 4) { Console::Sleep(0.1); continue; }

        pair<array<pair<uint16_t,uint16_t>,17>,array<pair<uint16_t,uint16_t>,17>> colortable;

        vector<vector<Console::Symbol>> screen;
        screen.push_back(vector<Console::Symbol>(width,Console::Symbol(u'▒',16,16)));
        screen.push_back(vector<Console::Symbol>());
        auto textcur = u"Current Brush Style: \u25cf";
        for (size_t i = 0; i <= (width-u16strlen(textcur))/2; i++) 
            screen.back().push_back(Console::Symbol(L'▒',16,16));
        for (size_t i = 0; i < u16strlen(textcur); i++)
            screen.back().push_back(Console::Symbol(textcur[i],16,16));
        screen.back().back().foreground(color.first); 
        screen.back().back().background(color.second);
        while (screen.back().size() < (uint16_t)width)
            screen.back().push_back(Console::Symbol(u'▒',16,16));

        screen.push_back(vector<Console::Symbol>(width,Console::Symbol(u'▒',16,16)));
        screen.push_back(vector<Console::Symbol>());

        auto textcur2 = u"Background Color:";
        int i = 0;
    nextline1:
        --i; while (++i < width && textcur2[i])
            screen.back().push_back(Console::Symbol(textcur2[i],16,16));
        if (i >= width) {
            screen.push_back(vector<Console::Symbol>());
            i = 0;
            goto nextline1;
        }

        int colnum = 0;
    nextline2:
        i-=2; --colnum; while ((i+=2) < width-1 && ++colnum <= 16) {
            screen.back().push_back(Console::Symbol(u'▒',16,16));
            screen.back().push_back(Console::Symbol(u'\u25cf',color.first,colnum));
            colortable.second[colnum] = pair<uint16_t,uint16_t>({screen.back().size()-1,screen.size()-1});
        }
        if (i >= width-1) {
            if (i < width) screen.back().push_back(Console::Symbol(u'▒',16,16));
            screen.push_back(vector<Console::Symbol>());
            i = 0;
            goto nextline2;
        } else while (++i <= width)
            screen.back().push_back(Console::Symbol(u'▒',16,16));

        screen.push_back(vector<Console::Symbol>(width,Console::Symbol(u'▒',16,16)));
        screen.push_back(vector<Console::Symbol>());

        auto textcur3 = u"Foreground Color:";
        i = 0;
    nextline3:
        --i; while (++i < width && textcur3[i])
            screen.back().push_back(Console::Symbol(textcur3[i],16,16));
        if (i >= width) {
            screen.push_back(vector<Console::Symbol>());
            i = 0;
            goto nextline3;
        }

        colnum = 0;
    nextline4:
        i -= 2; --colnum; while ((i+=2) < width-1 && ++colnum <= 16) {
            screen.back().push_back(Console::Symbol(u'▒',16,16));
            screen.back().push_back(Console::Symbol(u'\u25cf',colnum, color.second));
            colortable.first[colnum] = pair<uint16_t,uint16_t>({screen.back().size()-1,screen.size()-1});
        }
        if (i >= width-1) {
            if (i < width) screen.back().push_back(Console::Symbol(u'▒',16,16));
            screen.push_back(vector<Console::Symbol>());
            i = 0;
            goto nextline4;
        } else while (++i <= width)
            screen.back().push_back(Console::Symbol(u'▒',16,16));

        auto endtext = screen.size();

        screen.push_back(vector<Console::Symbol>(width,Console::Symbol(u'▒',16,16)));

        while (screen.size() < (uint16_t)(endtext+(3*(height-endtext)/4)))
            screen.push_back(vector<Console::Symbol>(width,Console::Symbol(u'▒',16,16)));

        auto textcur4 = u" CLOSE ";
        screen.push_back(vector<Console::Symbol>((width-u16strlen(textcur4))/2,Console::Symbol(u'▒',16,16)));
        for (size_t i = 0; i < u16strlen(textcur4); i++)
            screen.back().push_back(Console::Symbol(textcur4[i],16,16));

        pair<uint16_t,uint16_t> closepos = pair<uint16_t,uint16_t>(screen.back().size(),screen.size()-1);
        while (screen.back().size() < (uint16_t)width)
            screen.back().push_back(Console::Symbol(u'▒',16,16));

        while (screen.size() < (uint16_t)height)
            screen.push_back(vector<Console::Symbol>(width,Console::Symbol(u'▒',16,16)));

        Console::HandleMouseAndFocus();
        
        if (Console::IsFocused()) Console::HandleKeyboard();
        else Console::DontHandleKeyboard(); // do smth with resize window custom

        auto mouse = Console::GetMouseStatus();

        if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY) {
            bool found = false;
            for (int i = 0; i <= 16; i++)
                if (colortable.first[i].first == mouse.x && colortable.first[i].second == mouse.y) {
                    found = true;
                    color.first = i;
                    auto msg = u"F" + to_u16string(i);
                    Console::SendParentMessage(msg);
                    break;
                }
            if (!found) for (int i = 0; i <= 16; i++)
                if (colortable.second[i].first == mouse.x && colortable.second[i].second == mouse.y) {
                    color.second = i;
                    auto msg = u"B" + to_u16string(i);
                    Console::SendParentMessage(msg);
                    break;
                }
        }

        if (mouse.y == closepos.second && mouse.x < closepos.first && mouse.x >= closepos.first - u16strlen(textcur4)) {
            if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY)
                return;
            if (!curon) {
                Console::ShowCursor();
                curon = true;
            }
            if (mouse.x != lastcurpos.first || mouse.y != lastcurpos.second) {
                Console::MoveCursor(mouse.x,mouse.y);
                lastcurpos.first = mouse.x;
                lastcurpos.second = mouse.y;
            }
        } else if (curon) {
            Console::HideCursor();
            curon = false;
        }
        
        Console::FillScreen(screen);

        Console::Sleep(0.2,true);
    }
}

pair<int,u16string> GetSymChartPopup(int argc, const char16_t* argv[]) {
    Console::SetTitle(u"Edit Quick Symbol Picker");
    Console::ShowCursor();
    Console::SetCursorSize(100);

    u16string pickerstr = (argc < 2) ? u"          " : argv[1];
    auto initstr = pickerstr;
    int cursorpos = 0;

    vector<vector<Console::Symbol>> scr;
    int last_width = 0;
    int last_height = 0;
    while (1) {
        bool newscr = false;

        Console::HandleMouseAndFocus();
        if (Console::IsFocused()) Console::HandleKeyboard();
        else Console::DontHandleKeyboard();
        if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) return {EXIT_FAILURE,(argc < 2) ? u"          " : argv[1]};
        if (Console::KeyPressed() == Key::Enum::ESC) return {EXIT_FAILURE,(argc < 2) ? u"          " : argv[1]};

        if (Console::KeyPressed() == Key::Enum::ARROW_RIGHT) {newscr = true; cursorpos = min<size_t>(cursorpos+1,pickerstr.size()-1);}
        if (Console::KeyPressed() == Key::Enum::ARROW_LEFT) {newscr = true; cursorpos = max(0,cursorpos-1);}

        wchar_t x;
        while ((x = win.get()) && win.good()) {
            newscr = true;
            if (x == L'\177' || x == L'\b') {
                --cursorpos;
                if (cursorpos < 0) cursorpos = 0;
                else pickerstr[cursorpos] = initstr[cursorpos];
            } else if (x == L'\n' || x == L'\r') return {EXIT_SUCCESS,pickerstr};
            else {
                pickerstr[cursorpos] = WCharToChar16(x);
                cursorpos = min<size_t>(cursorpos+1,pickerstr.size()-1);
            }
        }

        if (last_width != Console::GetWindowWidth() || last_height != Console::GetWindowHeight()) {
            last_width = Console::GetWindowWidth();
            last_height = Console::GetWindowHeight();
            newscr = true;
        }
        if (last_height < 3 || last_width < 3) {
            Console::Sleep(0.1,true);
            continue;
        }
        if (newscr) {
            scr.clear();
            int i;
            for (i = 0; i < (last_height/2); i++) {
                scr.push_back(vector<Console::Symbol>());
            }
            size_t j;
            for (j = 0; j <= (unsigned short)(last_width/10); j++)
                scr.back().push_back(Console::Symbol(L' ',16,16));
            while (++j < (unsigned short)(last_width - (last_width/10)))
                scr.back().push_back(Console::Symbol(L' ',Color::DEFAULT,Color::LIGHT_BLACK));
            
            scr.push_back(vector<Console::Symbol>());
            for (j = 0; j <= (unsigned short)(last_width/10); j++)
                scr.back().push_back(Console::Symbol(L' ',16,16));

            while (++j <= (last_width/10) + (((size_t)last_width - 2*((size_t)(last_width/10)) - pickerstr.size())/2))
                scr.back().push_back(Console::Symbol(L' ',Color::DEFAULT,Color::LIGHT_BLACK));

            auto strbeg = scr.back().size();
            auto strh = scr.size() - 1;
            --j; for (auto&& c : pickerstr) {
                scr.back().push_back({ c,Color::DEFAULT,Color::LIGHT_BLACK });
                ++j;
            }

            while (++j < (unsigned short)(last_width - (last_width/10)))
                scr.back().push_back(Console::Symbol(L' ',Color::DEFAULT,Color::LIGHT_BLACK));

            scr.push_back(vector<Console::Symbol>());
            for (j = 0; j <= (unsigned short)(last_width/10); j++)
                scr.back().push_back(Console::Symbol(L' ',16,16));
            while (++j < (unsigned short)(last_width - (last_width/10)))
                scr.back().push_back(Console::Symbol(L' ',Color::DEFAULT,Color::LIGHT_BLACK));

            auto mouse = Console::GetMouseStatus();
            if (mouse.y == strh && mouse.x >= strbeg && mouse.x < strbeg + pickerstr.size()) {
                if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY)
                    cursorpos = mouse.x - strbeg;
            }
            Console::MoveCursor(strbeg+cursorpos,strh);

            Console::FillScreen(scr);
        }
        Console::Sleep(0.01);
    }
}

pair<int,char16_t> GetCustomCharPopup(int argc, const char16_t* argv[]) {
    Console::SetTitle(u"Enter Character");
    Console::ShowCursor();
    Console::SetCursorSize(100);

    u16string charstr;
    int cursorpos = 0;

    vector<vector<Console::Symbol>> scr;
    int last_width = 0;
    int last_height = 0;
    while (1) {
        bool newscr = false;

        Console::HandleMouseAndFocus();
        if (Console::IsFocused()) Console::HandleKeyboard();
        else Console::DontHandleKeyboard();
        if (Console::KeyPressed() == Key::Enum::q && IsCtrlDown()) return {EXIT_FAILURE,(argc < 2) ? u' ' : argv[1][0]};
        if (Console::KeyPressed() == Key::Enum::ESC) return {EXIT_FAILURE,(argc < 2) ? u' ' : argv[1][0]};

        if (Console::KeyPressed() == Key::Enum::ARROW_RIGHT) {newscr = true; cursorpos = min<size_t>(cursorpos+1,charstr.size()-1);}
        if (Console::KeyPressed() == Key::Enum::ARROW_LEFT) {newscr = true; cursorpos = max(0,cursorpos-1);}

        wchar_t x;
        while ((x = win.get()) && win.good()) {
            newscr = true;
            if (x == L'\177' || x == L'\b') {if (charstr.size()) charstr.pop_back(); cursorpos = min<size_t>(charstr.size()-1,cursorpos);}
            else if (x == L'\n' || x == L'\r') {
                if (charstr.size()) return {EXIT_SUCCESS,charstr[cursorpos]};
                else return {EXIT_FAILURE,(argc < 2) ? u' ' : argv[1][0]};
            }
            else charstr.push_back(WCharToChar16(x));
        }

        if (last_width != Console::GetWindowWidth() || last_height != Console::GetWindowHeight()) {
            last_width = Console::GetWindowWidth();
            last_height = Console::GetWindowHeight();
            newscr = true;
        }
        if (last_height < 3 || last_width < 3) {
            Console::Sleep(0.1,true);
            continue;
        }
        if (newscr) {
            scr.clear();
            int i;
            for (i = 0; i < (last_height/2); i++) {
                scr.push_back(vector<Console::Symbol>());
            }
            size_t j;
            for (j = 0; j <= (unsigned short)(last_width/10); j++)
                scr.back().push_back(Console::Symbol(L' ',16,16));
            while (++j < (unsigned short)(last_width - (last_width/10)))
                scr.back().push_back(Console::Symbol(L' ',Color::DEFAULT,Color::LIGHT_BLACK));
            
            scr.push_back(vector<Console::Symbol>());
            for (j = 0; j <= (unsigned short)(last_width/10); j++)
                scr.back().push_back(Console::Symbol(L' ',16,16));

            while (++j <= (last_width/10) + (((size_t)last_width - 2*((size_t)(last_width/10)) - charstr.size())/2))
                scr.back().push_back(Console::Symbol(L' ',Color::DEFAULT,Color::LIGHT_BLACK));

            auto strbeg = scr.back().size();
            auto strh = scr.size() - 1;
            --j; for (auto&& c : charstr) {
                scr.back().push_back({ c,Color::DEFAULT,Color::LIGHT_BLACK });
                ++j;
            }

            while (++j < (unsigned short)(last_width - (last_width/10)))
                scr.back().push_back(Console::Symbol(L' ',Color::DEFAULT,Color::LIGHT_BLACK));

            scr.push_back(vector<Console::Symbol>());
            for (j = 0; j <= (unsigned short)(last_width/10); j++)
                scr.back().push_back(Console::Symbol(L' ',16,16));
            while (++j < (unsigned short)(last_width - (last_width/10)))
                scr.back().push_back(Console::Symbol(L' ',Color::DEFAULT,Color::LIGHT_BLACK));

            auto mouse = Console::GetMouseStatus();
            if (mouse.y == strh && mouse.x >= strbeg && mouse.x < strbeg + charstr.size()) {
                if (Console::MouseButtonClicked().first == MOUSE_BUTTON_PRIMARY)
                    cursorpos = mouse.x - strbeg;
            }

            Console::MoveCursor(strbeg+cursorpos,strh);

            Console::FillScreen(scr);
        }
        Console::Sleep(0.01);
    }
}

int sub(int type) {
    assert(type != 0);
    auto argc = Console::GetArgC();
    auto argv = (const char16_t**)Console::GetArgV();
    wout << L"sub: " << type << L'\n' << "argc: " << argc << L'\n';
    for (int i = 0; i < argc; i++) wout << L"argv[" << i << L"]: " << U16StringToWString(argv[i]) << L'\n';
    flush();
    pair<int,u16string> res;
    switch (type) { 
        case 1:
            res = ErrorPopup(argc,argv);
            return res.first;
        case 2:
            {auto ret = GetCustomCharPopup(argc,argv);
            res.second.push_back(ret.second);
            res.first = ret.first;}
            Console::SetResult(res.second);
            return res.first;
        case 3:
            res = GetSymChartPopup(argc,argv);
            Console::SetResult(res.second);
            return res.first;
        case 4:
            res = MsgPopup(argc,argv);
            Console::SetResult(res.second);
            return res.first;
        case 5:
            ColorPopup(argc,argv);
            return 0;
        case 6:
            res = NewFilePopup(argc,argv);
            Console::SetResult(res.second);
            return res.first;
        case 7:
            res = GetFilePopup(argc,argv);
            Console::SetResult(res.second);
            return res.first;
    }
    return EXIT_FAILURE;
}
