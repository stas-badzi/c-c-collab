#include "Console.hpp"

#include "dllimport.hpp"
#include "FileSystem.hpp"
#include "System.hpp"
#include <vector>
#include <string.h>

using namespace cppimp;
using namespace uniconv;
using namespace std;
using namespace cpp;
using namespace cs;

Console::MouseStatus::MouseStatus(void) {
    this->primary = false;
    this->middle = false;
    this->secondary = false;
    this->scroll = pair<bool,bool>(false,false);
    this->x = -1;
    this->y = -1;
}

void Console::Init(void) {
    cppimp::Console_Init();
}

void Console::Fin(void) {
    cppimp::Console_Fin();
}

int Console::HandleKeyboard(void) {
    return cppimp::Console_HandleKeyboard();
}

bool Console::IsKeyDown(int key) {
    return cppimp::Console_IsKeyDown(key);
}

bool Console::IsKeyToggled(int key) {
    return cppimp::Console_IsKeyToggled(key);
}

int Console::KeyPressed(void) {
    return cppimp::Console_KeyPressed();
}

int Console::KeyReleased(void) {
    return cppimp::Console_KeyReleased();
}

void Console::FillScreen(vector<vector<Console::Symbol> > symbols) {

    void* voidsyms = TextureToPtr(symbols);

    Console_FillScreen(voidsyms);
}

void Console::HandleMouseAndFocus(void) {
    return cppimp::Console_HandleMouseAndFocus();
}

bool Console::IsFocused(void) {
    return cppimp::Console_IsFocused();
}

struct Console::MouseStatus Console::GetMouseStatus(void) {
    Console::MouseStatus* ptr = (Console::MouseStatus*)cppimp::Console_GetMouseStatus();
    Console::MouseStatus val = *ptr;
    System::FreeMemory(ptr);
    return val;
}

pair<uint8_t, uint8_t> Console::MouseButtonClicked(void) {
    uint8_t* func = cppimp::Console_MouseButtonClicked$ret2();
    pair<uint8_t, uint8_t> out(func[0],func[1]);
    delete[] func;
    return out;
}

uint8_t Console::MouseButtonReleased(void) {
    return cppimp::Console_MouseButtonReleased();
}

int16_t Console::GetWindowWidth(void) {
    return Console_GetWindowWidth();
}

int16_t Console::GetWindowHeight(void) {
    return Console_GetWindowHeight();
}


int32_t Console::GetArgC(void) {
    return Console_GetArgC();
}
wchar_t** Console::GetArgV(void) {
    unichar** ret = Console_GetArgV();
    int32_t length = Console_GetArgC();
    wchar_t** out = (wchar_t**)malloc(sizeof(wchar_t*)*length); // 4 is max utf bytes in one char
    for (int i = 0; i < length; i++) {
        wstring arg = NativeToWString(UnicodeToUtf8String(ret[i]));
        out[i] = (wchar_t*)System::AllocateMemory(sizeof(wchar_t)*arg.size());
        for (size_t j = 0; j < arg.size(); j++) out[i][j] = arg[j];
    }
    __dllfree(ret);
    return out;
}

void Console::Sleep(double seconds){
    Console_Sleep(seconds);
}

Console::Symbol::Symbol(void) {
    symbol = Console_Symbol_Construct$cfb(0x0020, 0x10, 0x10);
}

Console::Symbol::Symbol(const Symbol &cp) {
    symbol = Console_Symbol_Construct$smb(cp.symbol);
}


Console::Symbol::Symbol(void* ptr, bool direct) {
    if (direct) symbol = ptr;
    else symbol = Console_Symbol_Construct$smb(ptr);
}

Console::Symbol::Symbol(wchar_t character, uint8_t foreground, uint8_t background) {
    symbol = Console_Symbol_Construct$cfb(Utf8ToUnicode(WCharToNative(character)),foreground,background);
}

Console::Symbol::~Symbol() {
    Console_Symbol_Destruct(symbol);
}

Console::Symbol Console::Symbol::operator=(const Console::Symbol &src) {
    Console_Symbol_operator$eq(this->symbol,src.symbol);
    //set own local variables
    return *this;
}

vector<vector<Console::Symbol> > Console::Symbol::CreateTexture(wstring characters, uint8_t backgrounds[], uint8_t foregrounds[]) {
    size_t count = 0;
    vector<wstring> out;
    out.push_back(wstring());
    for (size_t i = 0; i < characters.size(); i++) {
        if (characters[i] != '\n') out.back().push_back(characters[i]);
        else {
            ++count;
            out.push_back(wstring());
        }
    }
    
    return Console::Symbol::CreateTexture(out.data(), ++count, backgrounds, foregrounds); 
}

vector<vector<Console::Symbol> > Console::Symbol::CreateTexture(wstring characters) {
    size_t count = 0;
    vector<wstring> out;
    out.push_back(wstring());
    for (size_t i = 0; i < characters.size(); i++) {
        if (characters[i] != '\n') out.back().push_back(characters[i]);
        else {
            ++count;
            out.push_back(wstring());
        }
    }
    
    return Console::Symbol::CreateTexture(out.data(), ++count); 
}

vector<vector<Console::Symbol> > Console::Symbol::CreateTexture(wstring characters[], uint8_t backgrounds[], uint8_t foregrounds[]) {
    size_t count = 0;
    while (characters[count].size()) ++count;
    return Console::Symbol::CreateTexture(characters, count, backgrounds, foregrounds); 
}

vector<vector<Console::Symbol> > Console::Symbol::CreateTexture(wstring characters[]) {
    size_t count = 0;
    while (characters[count].size()) ++count;
    return Console::Symbol::CreateTexture(characters, count); 
}

vector<vector<Console::Symbol> > Console::Symbol::CreateTexture(wstring characters[], size_t size) {
    vector<vector<Console::Symbol> > out;
    for (size_t i = 0; i < size; i++) {
        out.push_back(vector<Console::Symbol>());
        wstring &elem = characters[i];
        for (size_t j = 0; j < elem.size(); j++) {
            out.back().push_back(Console::Symbol(elem[j]));
        }
    }
    return out;
}

vector<vector<Console::Symbol> > Console::Symbol::CreateTexture(wstring characters[], size_t size, uint8_t backgrounds[], uint8_t foregrounds[]) {
    vector<vector<Console::Symbol> > out;
    int count = 0;
    for (size_t i = 0; i < size; i++) {
        out.push_back(vector<Console::Symbol>());
        wstring &str = characters[i];
        uint8_t *bkg = backgrounds + count;
        uint8_t *frg = foregrounds + count;
        for (size_t j = 0; j < str.size(); j++) {
            out.back().push_back(Console::Symbol(str[j],frg[j],bkg[j]));
            ++count;
        }
    }
    return out;
}

wchar_t Console::Symbol::character(void)
{
    return NativeToWChar(UnicodeToUtf8(Console_Symbol_character$get(symbol)));
}

void Console::Symbol::character(wchar_t val) {
    return Console_Symbol_character$set(symbol, Utf8ToUnicode(WCharToNative(val)));
}

uint8_t Console::Symbol::foreground(void) {
    return Console_Symbol_foreground$get(symbol);
}

void Console::Symbol::foreground(uint8_t val) {
    return Console_Symbol_foreground$set(symbol, val);
}

uint8_t Console::Symbol::background(void) {
    return Console_Symbol_background$get(symbol);
}

void Console::Symbol::background(uint8_t val) {
    return Console_Symbol_background$set(symbol, val);
}

void* Console::Symbol::Get() {
    return symbol;
}

#ifdef _WIN32
    uint8_t Console::Symbol::GetAttribute(void) {
        return Console_Symbol_GetAttribute(symbol);
    }

    void Console::Symbol::SetAttribute(uint8_t attribute) {
        return Console_Symbol_SetAttribute(symbol,attribute);
    }
#endif
