using namespace cpp;

template <typename T>
T System::ReadPointer(nint pointer) {
#ifdef _DEBUG
    __check$ALLOCATIONS(pointer, sizeof(T));
#endif
    return *(T*)(pointer);
}

template <typename T>
T System::ReadPointer(nint pointer, int offset) {
#ifdef _DEBUG
    __check$ALLOCATIONS(System::MovePointer(pointer,offset), sizeof(T));
#endif
    return *(T*)(System::MovePointer(pointer,offset));
}

template <typename T>
void System::WritePointer(nint &pointer, const T value) {
#ifdef _DEBUG
    __check$ALLOCATIONS(pointer, sizeof(T));
#endif
    *(T*)(pointer) = value;
}

template <typename T>
void System::WritePointer(nint &pointer, int offset, const T value) {
#ifdef _DEBUG
    __check$ALLOCATIONS(System::MovePointer(pointer,offset), sizeof(T));
#endif
    *(T*)(System::MovePointer(pointer, offset)) = value;
}