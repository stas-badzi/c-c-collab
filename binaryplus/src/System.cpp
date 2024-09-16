#include "System.hpp"
#include "dllimport.hpp"
#include <exception>

using namespace cpp;
using namespace std;
using namespace uniconv;

wstring System::ToNativePath(wstring path) {
    return NativeToWString(UnicodeToUtf8String(cppimp::System_ToNativePath(Utf8StringToUnicode(WStringToNative(path)))));
}

utfstr System::GetRootPath(void) {
    return NativeToWString(UnicodeToUtf8String(cppimp::System_GetRootPath()));
}

nint System::AllocateMemory(unsigned int bytes) {
    return cppimp::System_AllocateMemory(bytes);
}

void System::FreeMemory(nint pointer) {
    return cppimp::System_FreeMemory(pointer);
}

nint System::MovePointer(nint pointer, signed int bytes) {
    return cppimp::System_MovePointer(pointer, bytes);
}

template <typename T>
T System::ReadPointer(nint pointer) {
    switch (typeid(T))
    {
    case typeid(bool):
        return cppimp::System_ReadPointer$bool(pointer);
    case typeid(nint):
        return cppimp::System_ReadPointer$nint(pointer);
    case typeid(int8_t):
        return cppimp::System_ReadPointer$int8_t(pointer);
    case typeid(int16_t):
        return cppimp::System_ReadPointer$int16_t(pointer);
    case typeid(int32_t):
        return cppimp::System_ReadPointer$int32_t(pointer);
    case typeid(int64_t):
        return cppimp::System_ReadPointer$int64_t(pointer);
    case typeid(uint8_t):
        return cppimp::System_ReadPointer$uint8_t(pointer);
    case typeid(uint16_t):
        return cppimp::System_ReadPointer$uint16_t(pointer);
    case typeid(uint32_t):
        return cppimp::System_ReadPointer$uint32_t(pointer);
    case typeid(uint64_t):
        return cppimp::System_ReadPointer$uint64_t(pointer);
    default:
        break;
    }

    T* ret;
    switch (sizeof(T))
    {
    case sizeof(int8_t):
        ret = (T*)(&cppimp::System_ReadPointer$int8_t(pointer));
        break;
    case sizeof(int16_t):
        ret = (T*)(&cppimp::System_ReadPointer$int16_t(pointer));
        break;
    case sizeof(int32_t):
        ret = (T*)(&cppimp::System_ReadPointer$int32_t(pointer));
        break;
    case sizeof(int64_t):
        ret = (T*)(&cppimp::System_ReadPointer$int64_t(pointer));
        break;
    default:
        throw(runtime_error("Neither type size or type are defined"));
        break;
    }
    return *ret;
}

template <typename T>
T System::ReadPointer(nint pointer, int offset) {
    switch (typeid(T))
    {
    case typeid(bool):
        return cppimp::System_ReadPointer$bool(pointer,offset);
    case typeid(nint):
        return cppimp::System_ReadPointer$nint(pointer,offset);
    case typeid(int8_t):
        return cppimp::System_ReadPointer$int8_t(pointer,offset);
    case typeid(int16_t):
        return cppimp::System_ReadPointer$int16_t(pointer,offset);
    case typeid(int32_t):
        return cppimp::System_ReadPointer$int32_t(pointer,offset);
    case typeid(int64_t):
        return cppimp::System_ReadPointer$int64_t(pointer,offset);
    case typeid(uint8_t):
        return cppimp::System_ReadPointer$uint8_t(pointer,offset);
    case typeid(uint16_t):
        return cppimp::System_ReadPointer$uint16_t(pointer,offset);
    case typeid(uint32_t):
        return cppimp::System_ReadPointer$uint32_t(pointer,offset);
    case typeid(uint64_t):
        return cppimp::System_ReadPointer$uint64_t(pointer,offset);
    default:
        break;
    }

    T* ret;
    switch (sizeof(T))
    {
    case sizeof(int8_t):
        ret = (T*)(&cppimp::System_ReadPointer$int8_t(pointer,offset));
        break;
    case sizeof(int16_t):
        ret = (T*)(&cppimp::System_ReadPointer$int16_t(pointer,offset));
        break;
    case sizeof(int32_t):
        ret = (T*)(&cppimp::System_ReadPointer$int32_t(pointer,offset));
        break;
    case sizeof(int64_t):
        ret = (T*)(&cppimp::System_ReadPointer$int64_t(pointer,offset));
        break;
    default:
        throw(runtime_error("Neither type size or type are defined"));
        break;
    }
    return *ret;
}

template <typename T>
void System::WritePointer(nint &pointer, T value) {
    switch (typeid(T))
    {
    case typeid(bool):
        return cppimp::System_ReadPointer$bool(pointer,value);
    case typeid(nint):
        return cppimp::System_ReadPointer$nint(pointer,value);
    case typeid(int8_t):
        return cppimp::System_ReadPointer$int8_t(pointer,value);
    case typeid(int16_t):
        return cppimp::System_ReadPointer$int16_t(pointer,value);
    case typeid(int32_t):
        return cppimp::System_ReadPointer$int32_t(pointer,value);
    case typeid(int64_t):
        return cppimp::System_ReadPointer$int64_t(pointer,value);
    case typeid(uint8_t):
        return cppimp::System_ReadPointer$uint8_t(pointer,value);
    case typeid(uint16_t):
        return cppimp::System_ReadPointer$uint16_t(pointer,value);
    case typeid(uint32_t):
        return cppimp::System_ReadPointer$uint32_t(pointer,value);
    case typeid(uint64_t):
        return cppimp::System_ReadPointer$uint64_t(pointer,value);
    default:
        break;
    }

    switch (sizeof(T))
    {
    case sizeof(int8_t):
        return cppimp::System_ReadPointer$int8_t(pointer,*(int8_t*)(&value));
    case sizeof(int16_t):
        return cppimp::System_ReadPointer$int16_t(pointer,*(int16_t*)(&value));
    case sizeof(int32_t):
        return cppimp::System_ReadPointer$int32_t(pointer,*(int32_t*)(&value));
    case sizeof(int64_t):
        return cppimp::System_ReadPointer$int64_t(pointer,*(int64_t*)(&value));
    default:
        throw(runtime_error("Neither type size or type are defined"));
    }
}

template <typename T>
void System::WritePointer(nint &pointer, int offset, T value) {
    switch (typeid(T))
    {
    case typeid(bool):
        return cppimp::System_ReadPointer$bool(pointer,offset,value);
    case typeid(nint):
        return cppimp::System_ReadPointer$nint(pointer,offset,value);
    case typeid(int8_t):
        return cppimp::System_ReadPointer$int8_t(pointer,offset,value);
    case typeid(int16_t):
        return cppimp::System_ReadPointer$int16_t(pointer,offset,value);
    case typeid(int32_t):
        return cppimp::System_ReadPointer$int32_t(pointer,offset,value);
    case typeid(int64_t):
        return cppimp::System_ReadPointer$int64_t(pointer,offset,value);
    case typeid(uint8_t):
        return cppimp::System_ReadPointer$uint8_t(pointer,offset,value);
    case typeid(uint16_t):
        return cppimp::System_ReadPointer$uint16_t(pointer,offset,value);
    case typeid(uint32_t):
        return cppimp::System_ReadPointer$uint32_t(pointer,offset,value);
    case typeid(uint64_t):
        return cppimp::System_ReadPointer$uint64_t(pointer,offset,value);
    default:
        break;
    }

    switch (sizeof(T))
    {
    case sizeof(int8_t):
        return cppimp::System_ReadPointer$int8_t(pointer,offset,*(int8_t*)(&value));
    case sizeof(int16_t):
        return cppimp::System_ReadPointer$int16_t(pointer,offset,*(int16_t*)(&value));
    case sizeof(int32_t):
        return cppimp::System_ReadPointer$int32_t(pointer,offset,*(int32_t*)(&value));
    case sizeof(int64_t):
        return cppimp::System_ReadPointer$int64_t(pointer,offset,*(int64_t*)(&value));
    default:
        throw(runtime_error("Neither type size or type are defined"));
    }
}
