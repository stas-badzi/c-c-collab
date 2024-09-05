#include "System.hpp"

using namespace cpp;
using namespace std;

string System::root = System::GetRoot();

#ifdef _WIN32
    string System::GetRoot(void) {
        char buf[255];
        DWORD size = GetModuleFileNameA(NULL, buf, 255);
        char* edit = buf;
        while (edit[--size] != '\\') buf[size] = '\0';
        edit[size + 1] = '.';
        edit[size + 2] = '.';
        edit[size + 3] = '\0';
        string out = (string)(edit);
        return out;
    }

    string cpp::System::ToNativePath(string path) {
        char* edit = (char*)path;
        for (int i = 0; edit[i] != '\0'; i++) if (edit[i] == '/') edit[i] = '\\';
        string out = (string)(edit);
        return out;
    }
#else
    string System::GetRoot(void) {
        char buf[255] = { '\0' };
        ssize_t size = readlink("/proc/self/exe",buf,255);
        char* edit = buf;
        while (edit[--size] != '/') buf[size] = '\0';
        edit[size + 1] = '.';
        edit[size + 2] = '.';
        edit[size + 3] = '\0';
        string out = (string)(edit);
        return out;
    }

    string cpp::System::ToNativePath(string path) {
        return path;
    }
#endif

string System::GetRootPath()
{
    return System::root;
}



void *System::AllocateMemory(unsigned int bytes)
{
    return malloc(bytes);
}

void System::FreeMemory(void* pointer) {
    free(pointer);
}

void* System::MovePointer(void* pointer, signed int bytes) {
    return (int8_t*)(pointer) + bytes;
}

template <typename T>
T System::ReadPointer(void *pointer) {
    return *(T*)(pointer);
}

template <typename T>
T System::ReadPointer(void *pointer, int offset) {
    return *(T*)(System::MovePointer(pointer,offset));
}

template <typename T>
void System::WritePointer(void *&pointer, T value) {
    *(T*)(pointer) = value;
}

template <typename T>
void System::WritePointer(void *&pointer, int offset, T value) {
    *(T*)(System::MovePointer(pointer, offset)) = value;
}
