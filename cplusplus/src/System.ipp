template <typename T>
T System::ReadPointer(nint pointer) {
    __check$ALLOCATIONS(pointer, sizeof(T));
    return *(T*)(pointer);
}

template <typename T>
T System::ReadPointer(nint pointer, int offset) {
    __check$ALLOCATIONS((unsigned char*)pointer + offset, sizeof(T));
    return *(T*)(System::MovePointer(pointer,offset));
}

template <typename T>
void System::WritePointer(nint &pointer, T value) {
    __check$ALLOCATIONS(pointer, sizeof(T));
    *(T*)(pointer) = value;
}

template <typename T>
void System::WritePointer(nint &pointer, int offset, T value) {
    __check$ALLOCATIONS((unsigned char*)pointer + offset, sizeof(T));
    *(T*)(System::MovePointer(pointer, offset)) = value;
}