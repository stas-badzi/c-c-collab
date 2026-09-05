using namespace cpp;

template <typename T>
T System::ReadPointer(nint pointer) {
    return *(T*)(pointer);
}

template <typename T>
T System::ReadPointer(nint pointer, int offset) {
    return *(T*)(System::MovePointer(pointer,offset));
}

template <typename T>
void System::WritePointer(nint &pointer, const T value) {
    *(T*)(pointer) = value;
}

template <typename T>
void System::WritePointer(nint &pointer, int offset, const T value) {
    *(T*)(System::MovePointer(pointer, offset)) = value;
}