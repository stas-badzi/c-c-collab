#include <windows.h>

int main() {
    if (MessageBeep(0xFFFFFFFF)) return 0;
    return GetLastError();
}
