#include "System.hpp"
#include <vector>

using namespace cpp;
using namespace std;
using namespace uniconv;

wstring System::ToNativePath(wstring path) {
    return NativeToWString(UnicodeToUtf8String(cppimp::System_ToNativePath(Utf8StringToUnicode(WStringToNative(path)))));
}

utfstr System::GetRootPath(void) {
    return NativeToWString(UnicodeToUtf8String(cppimp::System_GetRootPath()));
}

nint System::AllocateMemory(size_t bytes) {
    return cppimp::System_AllocateMemory(bytes);
}

void System::FreeMemory(nint pointer) {
    return cppimp::System_FreeMemory(pointer);
}

nint System::MovePointer(nint pointer, signed int bytes) {
    return cppimp::System_MovePointer(pointer, bytes);
}

template vector<T> System::MakeList(size_t count, T elem)
{
    vector<T> ret;
    for (size_t i = 0; i < count; i++)
        ret.push_back(elem);
    return ret;
}

template <> bool System::ReadPointer<bool>(nint pointer) { return cppimp::System_ReadPointer$bool(pointer);}
template <> nint System::ReadPointer<nint>(nint pointer) { return cppimp::System_ReadPointer$nint(pointer);}
template <> int8_t System::ReadPointer<int8_t>(nint pointer) { return cppimp::System_ReadPointer$int8(pointer);}
template <> int16_t System::ReadPointer<int16_t>(nint pointer) { return cppimp::System_ReadPointer$int16(pointer);}
template <> int32_t System::ReadPointer<int32_t>(nint pointer) { return cppimp::System_ReadPointer$int32(pointer);}
template <> int64_t System::ReadPointer<int64_t>(nint pointer) { return cppimp::System_ReadPointer$int64(pointer);}
template <> uint8_t System::ReadPointer<uint8_t>(nint pointer) { return cppimp::System_ReadPointer$uint8(pointer);}
template <> uint16_t System::ReadPointer<uint16_t>(nint pointer) { return cppimp::System_ReadPointer$uint16(pointer);}
template <> uint32_t System::ReadPointer<uint32_t>(nint pointer) { return cppimp::System_ReadPointer$uint32(pointer);}
template <> uint64_t System::ReadPointer<uint64_t>(nint pointer) { return cppimp::System_ReadPointer$uint64(pointer);}

template <> bool System::ReadPointer<bool>(nint pointer, int offset) { return cppimp::System_ReadPointer$bool$ofs(pointer,offset);}
template <> nint System::ReadPointer<nint>(nint pointer, int offset) { return cppimp::System_ReadPointer$nint$ofs(pointer,offset);}
template <> int8_t System::ReadPointer<int8_t>(nint pointer, int offset) { return cppimp::System_ReadPointer$int8$ofs(pointer,offset);}
template <> int16_t System::ReadPointer<int16_t>(nint pointer, int offset) { return cppimp::System_ReadPointer$int16$ofs(pointer,offset);}
template <> int32_t System::ReadPointer<int32_t>(nint pointer, int offset) { return cppimp::System_ReadPointer$int32$ofs(pointer,offset);}
template <> int64_t System::ReadPointer<int64_t>(nint pointer, int offset) { return cppimp::System_ReadPointer$int64$ofs(pointer,offset);}
template <> uint8_t System::ReadPointer<uint8_t>(nint pointer, int offset) { return cppimp::System_ReadPointer$uint8$ofs(pointer,offset);}
template <> uint16_t System::ReadPointer<uint16_t>(nint pointer, int offset) { return cppimp::System_ReadPointer$uint16$ofs(pointer,offset);}
template <> uint32_t System::ReadPointer<uint32_t>(nint pointer, int offset) { return cppimp::System_ReadPointer$uint32$ofs(pointer,offset);}
template <> uint64_t System::ReadPointer<uint64_t>(nint pointer, int offset) { return cppimp::System_ReadPointer$uint64$ofs(pointer,offset);}

template <> void System::WritePointer<bool>(nint& pointer, bool value) { return cppimp::System_WritePointer$bool(pointer,value);}
template <> void System::WritePointer<nint>(nint& pointer, nint value) { return cppimp::System_WritePointer$nint(pointer,value);}
template <> void System::WritePointer<int8_t>(nint& pointer, int8_t value) { return cppimp::System_WritePointer$int8(pointer,value);}
template <> void System::WritePointer<int16_t>(nint& pointer, int16_t value) { return cppimp::System_WritePointer$int16(pointer,value);}
template <> void System::WritePointer<int32_t>(nint& pointer, int32_t value) { return cppimp::System_WritePointer$int32(pointer,value);}
template <> void System::WritePointer<int64_t>(nint& pointer, int64_t value) { return cppimp::System_WritePointer$int64(pointer,value);}
template <> void System::WritePointer<uint8_t>(nint& pointer, uint8_t value) { return cppimp::System_WritePointer$uint8(pointer,value);}
template <> void System::WritePointer<uint16_t>(nint& pointer, uint16_t value) { return cppimp::System_WritePointer$uint16(pointer,value);}
template <> void System::WritePointer<uint32_t>(nint& pointer, uint32_t value) { return cppimp::System_WritePointer$uint32(pointer,value);}
template <> void System::WritePointer<uint64_t>(nint& pointer, uint64_t value) { return cppimp::System_WritePointer$uint64(pointer,value);}

template <> void System::WritePointer<bool>(nint& pointer, int offset, bool value) { return cppimp::System_WritePointer$bool$ofs(pointer,offset,value);}
template <> void System::WritePointer<nint>(nint& pointer, int offset, nint value) { return cppimp::System_WritePointer$nint$ofs(pointer,offset,value);}
template <> void System::WritePointer<int8_t>(nint& pointer, int offset, int8_t value) { return cppimp::System_WritePointer$int8$ofs(pointer,offset,value);}
template <> void System::WritePointer<int16_t>(nint& pointer, int offset, int16_t value) { return cppimp::System_WritePointer$int16$ofs(pointer,offset,value);}
template <> void System::WritePointer<int32_t>(nint& pointer, int offset, int32_t value) { return cppimp::System_WritePointer$int32$ofs(pointer,offset,value);}
template <> void System::WritePointer<int64_t>(nint& pointer, int offset, int64_t value) { return cppimp::System_WritePointer$int64$ofs(pointer,offset,value);}
template <> void System::WritePointer<uint8_t>(nint& pointer, int offset, uint8_t value) { return cppimp::System_WritePointer$uint8$ofs(pointer,offset,value);}
template <> void System::WritePointer<uint16_t>(nint& pointer, int offset, uint16_t value) { return cppimp::System_WritePointer$uint16$ofs(pointer,offset,value);}
template <> void System::WritePointer<uint32_t>(nint& pointer, int offset, uint32_t value) { return cppimp::System_WritePointer$uint32$ofs(pointer,offset,value);}
template <> void System::WritePointer<uint64_t>(nint& pointer, int offset, uint64_t value) { return cppimp::System_WritePointer$uint64$ofs(pointer,offset,value);}