#include "System.hpp"

using namespace cpp;
using namespace std;
using namespace uniconv;

utfstr System::root = System::GetRoot();

#ifdef _WIN32
    utfstr System::GetRoot(void) {
        wchar_t buf[255];
        DWORD size = GetModuleFileName(NULL, buf, 255);
        wchar_t* edit = buf;
        while (edit[--size] != '\\') buf[size] = '\0';
        edit[size + 1] = '.';
        edit[size + 2] = '.';
        edit[size + 3] = '\0';
        utfstr out = (utfstr)(edit);
        return out;
    }

    utfstr cpp::System::ToNativePath(utfstr path) {
        for (int i = 0; path[i] != L'\0'; i++) if (path[i] == L'/') path[i] = L'\\';
        return path;
    }
#else
    utfstr System::GetRoot(void) {
        char buf[255] = { '\0' };
        ssize_t size = readlink("/proc/self/exe",buf,255);
        char* edit = buf;
        while (edit[--size] != '/') buf[size] = '\0';
        edit[size + 1] = '.';
        edit[size + 2] = '.';
        edit[size + 3] = '\0';
        utfstr out = (utfstr)(edit);
        return out;
    }

    utfstr System::ToNativePath(utfstr path) {
        return path;
    }
#endif

utfstr System::GetRootPath(void) {
    return System::root;
}



nint System::AllocateMemory(size_t bytes) {
    nint out = malloc(bytes);
    __save$ALLOCATIONS(out,bytes);
    return out;
}

void System::FreeMemory(nint pointer) {
    __free$ALLOCATIONS(pointer);
    free(pointer);
}

nint System::MovePointer(nint pointer, signed int bytes) {
    return (int8_t*)(pointer) + bytes;
}
