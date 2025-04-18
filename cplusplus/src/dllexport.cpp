#include <dynamic_library.h>
#include <unicode_conversion.hpp>
#include <vector>
#include <string.h>

#include "Console.hpp"
#include "System.hpp"
#include "TextureSystem.hpp"
#include "Game.hpp"
#include <control_heap.h>


using namespace uniconv;

// Console

    libexport void ThrowMsg(uniconv::unichar* msg) {
        cpp::Console::ThrowMsg(uniconv::UnicodeToNativeString(msg).c_str());
    }

    libexport void Exit(int code) {
        cpp::Console::Exit(code);
    }

    libexport void QuickExit(int code) {
        cpp::Console::QuickExit(code);
    }

    libexport void Console_Init(void) {
        cpp::Console::Init();
    }

    libexport void Console_Fin(void) {
        cpp::Console::Fin();
    }

    libexport void Console_Sleep(double seconds, bool sleep_input_thread) {
        cpp::Console::Sleep(seconds, sleep_input_thread);
    }

    libexport void Console_HandleKeyboard(void) {
        cpp::Console::HandleKeyboard();
    }

    libexport void Console_DontHandleKeyboard(void) {
        cpp::Console::DontHandleKeyboard();
    }

    libexport void Console_ResetKeyboard(void) {
        cpp::Console::ResetKeyboard();
    }

    libexport bool Console_IsKeyDown(enum Key::Enum arg1) {
        return cpp::Console::IsKeyDown(arg1);
    }

    libexport uint8_t Console_KeysToggled(void) {
        auto kt = cpp::Console::KeysToggled();
        return 0b1*kt.CapsLock + 0b10*kt.NumLock + 0b100*kt.ScrollLock;
    }

    libexport enum Key::Enum Console_KeyPressed(void) {
        return cpp::Console::KeyPressed();
    }

    libexport enum Key::Enum Console_KeyReleased(void) {
        return cpp::Console::KeyReleased();
    }

    libexport void Console_HandleMouseAndFocus(void) {
        return cpp::Console::HandleMouseAndFocus();
    }

    libexport void* Console_GetMouseStatus(void) {
        void* out = System::AllocateMemory(sizeof(cpp::Console::MouseStatus));
        System::WritePointer<cpp::Console::MouseStatus>(out,cpp::Console::GetMouseStatus());
        return out;
    }

    libexport uint8_t* Console_MouseButtonClicked$ret2(void) {
        std::pair<uint8_t, uint8_t> func = cpp::Console::MouseButtonClicked();
        nint ret = (uint8_t*)System::AllocateMemory(sizeof(uint8_t)*2);
        System::WritePointer(ret,func.first);
        System::WritePointer(ret,sizeof(uint8_t),func.first);
        return (uint8_t*)ret;
    }

    libexport uint8_t Console_MouseButtonReleased(void) {
        return cpp::Console::MouseButtonReleased();
    }

    libexport bool Console_IsMouseButtonDown(uint8_t button) {
        return cpp::Console::IsMouseButtonDown(button);
    }

    libexport bool Console_IsFocused(void) {
        return cpp::Console::IsFocused();
    }

    libexport void Console_FillScreen(void* ptr) {
        std::vector<std::vector<cpp::Console::Symbol> > texture = cs::Convert2dVector<cpp::Console::Symbol>(cs::PtrToTexture(ptr));
        return cpp::Console::FillScreen(texture);
    }

    libexport void Console_ClearScreenBuffer(void) {
        return cpp::Console::ClearScreenBuffer();
    }

    libexport int16_t Console_GetWindowWidth(void) {
        return cpp::Console::GetWindowWidth();
    }

    libexport int16_t Console_GetWindowHeight(void) {
        return cpp::Console::GetWindowHeight();
    }

    libexport void Console_HandleOutput(void) {
        return cpp::Console::HandleOutput();
    }

    libexport void Console_Update(void) {
        return cpp::Console::Update();
    }

    libexport void Console_SetResult(unichar* result) {
        return cpp::Console::SetResult(UnicodeToNativeString(result).c_str());
    }

    libexport void Console_MoveCursor(int x, int y) {
        return cpp::Console::MoveCursor(x,y);
    }

    libexport void Console_ShowCursor(void) {
        return cpp::Console::ShowCursor();
    }

    libexport void Console_HideCursor(void) {
        return cpp::Console::HideCursor();
    }

    libexport void Console_SetCursorSize(uint8_t size) {
        return cpp::Console::SetCursorSize(size);
    }

    libexport void Console_SetTitle(uniconv::unichar* title) {
        return cpp::Console::SetTitle(uniconv::UnicodeToNativeString(title).c_str());
    }

    libexport void Console_ReverseCursorBlink(void) {
        return cpp::Console::ReverseCursorBlink();
    }

    libexport int32_t Console_GetArgC(void) {
        return cpp::Console::GetArgC();
    }

    libexport unichar** Console_GetArgV(void) {
        int _argc = cpp::Console::GetArgC();
        utfcstr* _argv = cpp::Console::GetArgV();
        unichar** out = (unichar**)__dllalloc(sizeof(unichar*)*_argc);
        for (int i = 0; i < _argc; i++)
            out[i] = NativeStringToUnicode(_argv[i]);;
        return out;
    }

    struct popwinretval { bool val; int code; uniconv::unichar* result; };

    libexport popwinretval Console_PopupWindow(int type, int argc, uniconv::unichar* argv[], uniconv::unichar* title) {
        uniconv::utfcstr* args = (uniconv::utfcstr*)System::AllocateMemory(sizeof(uniconv::utfcstr)*argc);
        for (int i = 0; i < argc; i++) {
            uniconv::utfstr arg = UnicodeToNativeString(argv[i]).c_str();
            args[i] = (uniconv::utfcstr)System::AllocateMemory(sizeof(wchar_t)*arg.size());
            char_t* loc = (char_t*)args[i];
            for (size_t j = 0; j < arg.size(); j++) loc[j] = arg[j];
            loc[arg.size()] = L'\0';
        }
        System::FreeMemory(argv);
        auto ret = Console::PopupWindow(type, argc, args, (title ? UnicodeToNativeString(title).c_str() : nullptr));
        popwinretval retval;
        if ((retval.val = ret.has_value())) {
            retval.code = ret.value().first;
            retval.result = NativeStringToUnicode(ret.value().second.c_str());
        }
        System::FreeMemory(args);
        return retval;
    }

#ifdef _WIN32
    __declspec(dllexport)
#else
    __attribute__((visibility("default")))
#endif
    auto Console_PopupWindowAsync(int type, int argc, const char16_t* arg16v[], const char16_t u16title[]) {
        return Console::PopupWindowAsync(type, argc, arg16v, u16title);
    }

    struct popwinasyncretval { bool val; void* promise; };

    int (*Console_sub)(int);

    int sub(int arg1) {
        return Console_sub(arg1);
    }

    libexport void Console_sub$define(int (*arg1)(int)) {
        Console_sub = arg1;
    }
    

    int (*_Main)(void);

    int Main() {
        return _Main();
    }

    libexport void Main$define(int (*arg1)(void)) {
        _Main = arg1;
    }

    // Symbol
        libexport void* Console_Symbol_Construct$smb(cpp::Console::Symbol* src) {
            void* out = (void*) new cpp::Console::Symbol(*src);
        #ifdef _DEBUG
            __save$SYMBOLS(out);
        #endif
            return out;
        }

        libexport void* Console_Symbol_Construct$cfb(unichar character, uint8_t foreground = 7, uint8_t background = 0) {
            void* out = new cpp::Console::Symbol(UnicodeToNative(character), foreground, background);
        #ifdef _DEBUG
            __save$SYMBOLS(out);
        #endif
            return out;
        }

        #ifdef _WIN32
            libexport void* Console_Symbol_Construct$atr(uint8_t attribute) {
                void* out = (void*) new cpp::Console::Symbol(attribute);
            #ifdef _DEBUG
                __save$SYMBOLS(out);
            #endif
                return out;
            }
            
            libexport void Console_Symbol_SetAttribute(cpp::Console::Symbol* smb, uint8_t attribute) {
            #ifdef _DEBUG
                __check$SYMBOLS(smb);
            #endif
                smb->SetAttribute(attribute);
            }

            libexport uint8_t Console_Symbol_GetAttribute(cpp::Console::Symbol* smb) {
            #ifdef _DEBUG
                __check$SYMBOLS(smb);
            #endif
                return smb->GetAttribute();
            }
        #endif

        libexport void Console_Symbol_character$set(cpp::Console::Symbol* smb, unichar character) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            smb->character = UnicodeToNative(character);
        }

        libexport unichar Console_Symbol_character$get(cpp::Console::Symbol* smb) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            return NativeToUnicode(smb->character);
        }

        libexport void Console_Symbol_foreground$set(cpp::Console::Symbol* smb, uint8_t foreground) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            smb->foreground = foreground;
        }

        libexport uint8_t Console_Symbol_foreground$get(cpp::Console::Symbol* smb) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            return smb->foreground;
        }

        libexport void Console_Symbol_background$set(cpp::Console::Symbol* smb, uint8_t background) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            smb->background = background;
        }

        libexport uint8_t Console_Symbol_background$get(cpp::Console::Symbol* smb) {
        #ifdef _DEBUG
            __check$SYMBOLS(smb);
        #endif
            return smb->background;
        }

        libexport void Console_Symbol_Destruct(cpp::Console::Symbol* smb) {
        #ifdef _DEBUG
            __free$SYMBOLS(smb);
        #endif
            delete smb;
        }

        libexport void* Console_Symbol_operator$eq(cpp::Console::Symbol* cp, cpp::Console::Symbol* src) {
        #ifdef _DEBUG
            __check$SYMBOLS(src);
            __check$SYMBOLS(cp);
        #endif
            void* out = (void*) new cpp::Console::Symbol( (*cp) = (*src) );
        #ifdef _DEBUG
            __save$SYMBOLS(out);
        #endif
            return out;
        }

        libexport void Console_Symbol_Inspect(cpp::Console::Symbol* cp) {
            auto sym = *cp;
            return;
        }
    // ~Symbol

// ~Console

// System
    libexport uniconv::unichar* System_GetRootDir(void) {
        return uniconv::NativeStringToUnicode(cpp::System::GetRootDir().c_str());
    }

    libexport uniconv::unichar* System_GetSelfPath(void) {
        return uniconv::NativeStringToUnicode(cpp::System::GetSelfPath().c_str());
    }

    libexport int System_MakeDirectory(uniconv::unichar* arg1) {
        return cpp::System::MakeDirectory(uniconv::UnicodeToNativeString(arg1).c_str());
    }

    libexport int System_ClearDirectory(uniconv::unichar* arg1) {
        return cpp::System::ClearDirectory(uniconv::UnicodeToNativeString(arg1).c_str());
    }

    libexport int System_DeleteDirectory(uniconv::unichar* arg1) {
        return cpp::System::DeleteDirectory(uniconv::UnicodeToNativeString(arg1).c_str());
    }

    libexport int System_RemoveFile(uniconv::unichar* arg1) {
        return cpp::System::RemoveFile(uniconv::UnicodeToNativeString(arg1).c_str());
    }

    libexport bool System_IsFile(uniconv::unichar* arg1) {
        return cpp::System::IsFile(uniconv::UnicodeToNativeString(arg1).c_str());
    }

    libexport bool System_IsDirectory(uniconv::unichar* arg1) {
        return cpp::System::IsDirectory(uniconv::UnicodeToNativeString(arg1).c_str());
    }

    libexport bool System_DoesPathExist(uniconv::unichar* arg1) {
        return cpp::System::DoesPathExist(uniconv::UnicodeToNativeString(arg1).c_str());
    }
    
    libexport uniconv::unichar* System_ToNativePath(uniconv::unichar* arg1) {
        return uniconv::NativeStringToUnicode(cpp::System::ToNativePath(uniconv::UnicodeToNativeString(arg1)).c_str());
    }
    
    libexport nint System_AllocateMemory(size_t arg1) {
        return cpp::System::AllocateMemory(arg1);
    }

    libexport void System_FreeMemory(nint arg1) {
        return cpp::System::FreeMemory(arg1);
    }

    libexport nint System_MovePointer(nint arg1, int32_t arg2) {
        return cpp::System::MovePointer(arg1,arg2);
    }
    // ReadPointer<T>
        libexport int8_t System_ReadPointer$int8(nint arg1) {
            return cpp::System::ReadPointer<int8_t>(arg1);
        }

        libexport int16_t System_ReadPointer$int16(nint arg1) {
            return cpp::System::ReadPointer<int16_t>(arg1);
        }

        libexport int32_t System_ReadPointer$int32(nint arg1) {
            return cpp::System::ReadPointer<int32_t>(arg1);
        }

        libexport int64_t System_ReadPointer$int64(nint arg1) {
            return cpp::System::ReadPointer<int64_t>(arg1);
        }

        libexport uint8_t System_ReadPointer$uint8(nint arg1) {
            return cpp::System::ReadPointer<uint8_t>(arg1);
        }

        libexport uint16_t System_ReadPointer$uint16(nint arg1) {
            return cpp::System::ReadPointer<uint16_t>(arg1);
        }

        libexport uint32_t System_ReadPointer$uint32(nint arg1) {
            return cpp::System::ReadPointer<uint32_t>(arg1);
        }

        libexport uint64_t System_ReadPointer$uint64(nint arg1) {
            return cpp::System::ReadPointer<uint64_t>(arg1);
        }

        libexport bool System_ReadPointer$bool(nint arg1) {
            return cpp::System::ReadPointer<bool>(arg1);
        }

        libexport nint System_ReadPointer$nint(nint arg1) {
            return cpp::System::ReadPointer<nint>(arg1);
        }

        // offset = true

        libexport int8_t System_ReadPointer$int8$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<int8_t>(arg1,arg2);
        }

        libexport int16_t System_ReadPointer$int16$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<int16_t>(arg1,arg2);
        }

        libexport int32_t System_ReadPointer$int32$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<int32_t>(arg1,arg2);
        }

        libexport int64_t System_ReadPointer$int64$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<int64_t>(arg1,arg2);
        }

        libexport uint8_t System_ReadPointer$uint8$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<uint8_t>(arg1,arg2);
        }

        libexport uint16_t System_ReadPointer$uint16$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<uint16_t>(arg1,arg2);
        }

        libexport uint32_t System_ReadPointer$uint32$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<uint32_t>(arg1,arg2);
        }

        libexport uint64_t System_ReadPointer$uint64$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<uint64_t>(arg1,arg2);
        }

        libexport bool System_ReadPointer$bool$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<bool>(arg1,arg2);
        }

        libexport nint System_ReadPointer$nint$ofs(nint arg1, int32_t arg2) {
            return cpp::System::ReadPointer<nint>(arg1,arg2);
        }
    // ~ReadPointer<T>

    
    // WritePointer<T>
        libexport void System_WritePointer$int8(nint arg1, int8_t arg2) {
            return cpp::System::WritePointer<int8_t>(arg1,arg2);
        }

        libexport void System_WritePointer$int16(nint arg1, int16_t arg2) {
            return cpp::System::WritePointer<int16_t>(arg1,arg2);
        }

        libexport void System_WritePointer$int32(nint arg1, int32_t arg2) {
            return cpp::System::WritePointer<int32_t>(arg1,arg2);
        }

        libexport void System_WritePointer$int64(nint arg1, int64_t arg2) {
            return cpp::System::WritePointer<int64_t>(arg1,arg2);
        }

        libexport void System_WritePointer$uint8(nint arg1, uint8_t arg2) {
            return cpp::System::WritePointer<uint8_t>(arg1,arg2);
        }

        libexport void System_WritePointer$uint16(nint arg1, uint16_t arg2) {
            return cpp::System::WritePointer<uint16_t>(arg1,arg2);
        }

        libexport void System_WritePointer$uint32(nint arg1, uint32_t arg2) {
            return cpp::System::WritePointer<uint32_t>(arg1,arg2);
        }

        libexport void System_WritePointer$uint64(nint arg1, uint64_t arg2) {
            return cpp::System::WritePointer<uint64_t>(arg1,arg2);
        }

        libexport void System_WritePointer$bool(nint arg1, bool arg2) {
            return cpp::System::WritePointer<bool>(arg1,arg2);
        }

        libexport void System_WritePointer$nint(nint arg1, nint arg2) {
            return cpp::System::WritePointer<nint>(arg1,arg2);
        }

        // offset = true

        libexport void System_WritePointer$int8$ofs(nint arg1, int32_t arg3, int8_t arg2) {
            return cpp::System::WritePointer<int8_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$int16$ofs(nint arg1, int32_t arg3, int16_t arg2) {
            return cpp::System::WritePointer<int16_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$int32$ofs(nint arg1, int32_t arg3, int32_t arg2) {
            return cpp::System::WritePointer<int32_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$int64$ofs(nint arg1, int32_t arg3, int64_t arg2) {
            return cpp::System::WritePointer<int64_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$uint8$ofs(nint arg1, int32_t arg3, uint8_t arg2) {
            return cpp::System::WritePointer<uint8_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$uint16$ofs(nint arg1, int32_t arg3, uint16_t arg2) {
            return cpp::System::WritePointer<uint16_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$uint32$ofs(nint arg1, int32_t arg3, uint32_t arg2) {
            return cpp::System::WritePointer<uint32_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$uint64$ofs(nint arg1, int32_t arg3, uint64_t arg2) {
            return cpp::System::WritePointer<uint64_t>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$bool$ofs(nint arg1, int32_t arg3, bool arg2) {
            return cpp::System::WritePointer<bool>(arg1,arg3,arg2);
        }

        libexport void System_WritePointer$nint$ofs(nint arg1, int32_t arg3, nint arg2) {
            return cpp::System::WritePointer<nint>(arg1,arg3,arg2);
        }

    // ~WritePointer<T>

// ~System

// Camera
    libexport void* Game_MartixPosition_Construct(int iIndex, int jIndex) {
        cpp::Game::MatrixPosition vpc(iIndex, jIndex);
        auto ret = (cpp::Game::MatrixPosition*)System::AllocateMemory(sizeof(cpp::Game::MatrixPosition));
        *ret = vpc;
        return ret;
    }

    libexport void* Game_Camera_Construct(int height, int width, void* symptr) {
        cpp::Game::Camera cam(height, width, *(cpp::Console::Symbol*)symptr);
        auto ret = (cpp::Game::Camera*)System::AllocateMemory(sizeof(cpp::Game::Camera));
        *ret = cam;
        return ret;
    }

    libexport void* Game_Camera_ViewportCenter(void* cameraptr) {
        auto ret = (cpp::Game::MatrixPosition*)System::AllocateMemory(sizeof(cpp::Game::MatrixPosition));
        *ret = ((cpp::Game::Camera*)cameraptr)->ViewportCenter();
        return ret;
    }

    libexport void Game_Camera_DrawTexture(int x, int y, void* textureptr, void* cameraptr) {
        const auto& texture = cs::PtrToTexture(textureptr, true);
        const auto& real_texture = cs::Convert2dVector<Console::Symbol>(texture);
        ((cpp::Game::Camera*)cameraptr)->DrawTexture(x, y, real_texture);
    }

    libexport void Game_Camera_DrawToScreen(int x, int y, void* screenptr, void* cameraptr) {
        auto& screen = cs::PtrToTexture(screenptr, true);
        auto& real_screen = cs::Convert2dVector<Console::Symbol>(screen);
        ((cpp::Game::Camera*)cameraptr)->DrawTexture(x, y, real_screen);
    }
#ifdef _DEBUG
// control_heap
    libexport void ControlHeap__save$ALLOCATIONS(void* arg1, unsigned long arg2) {
        return __save$ALLOCATIONS(arg1,arg2);
    }

    libexport void ControlHeap__free$ALLOCATIONS(void* arg1) {
        return __free$ALLOCATIONS(arg1);
    }

    libexport void ControlHeap__check$ALLOCATIONS(void* arg1, unsigned long arg2) {
        return __check$ALLOCATIONS(arg1,arg2);
    }

    libexport void ControlHeap__save$SYMBOLS(void* arg1) {
        return __save$SYMBOLS(arg1);
    }

    libexport void ControlHeap__free$SYMBOLS(void* arg1) {
        return __free$SYMBOLS(arg1);
    }

    libexport void ControlHeap__check$SYMBOLS(void* arg1) {
        return __check$SYMBOLS(arg1);
    }
#endif
