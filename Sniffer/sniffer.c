#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

HHOOK keyboardHook;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
    //a key was pressed
    if (wParam == WM_KEYDOWN && nCode == HC_ACTION)
    {
        printf("%s", key);
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

int main() {
    //keyboardHook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardProc, NULL, NULL);
    int pid = GetWindowThreadProcessId(hwnd, 0);
    printf("%d\n", pid);
    //MSG msg = { 0 };
    ////our application loop
    //while (GetMessage(&msg, NULL, 0, 0) != 0);
    //UnhookWindowsHookEx(keyboardHook);
    return 0;
}