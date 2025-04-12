#include <windows.h>

int main() {
    HWND hwndFound;
    wchar_t pszNewWindowTitle[1024];
    wsprintf(pszNewWindowTitle,L"%d/%d",GetTickCount(),GetCurrentProcessId());
    SetConsoleTitle(pszNewWindowTitle);
    Sleep(40);
    hwndFound=FindWindow(NULL, pszNewWindowTitle);
    if (!hwndFound) {
        hwndFound = GetForegroundWindow();
        wchar_t pszWindowTitle[1024];
        int siz = GetWindowText(hwndFound, pszWindowTitle, 1024);
        pszWindowTitle[siz] = L'\0';
    }
    SendMessage(hwndFound, WM_CLOSE, 0, 0);
    return 0;
}
