/*

    cl-icp-p2p dll client injector
    made by ChatGPT & @lemontoe_ on discord
    https://github.com/lemontoe

    i hate injectors
*/

#include <windows.h>
#include <iostream>
#include <tlhelp32.h>
#include <string>

// Function to convert wide string (WCHAR) to narrow string (char)
std::string WideCharToString(const WCHAR* wideStr) {
    int bufferSize = WideCharToMultiByte(CP_ACP, 0, wideStr, -1, NULL, 0, NULL, NULL);
    std::string result(bufferSize, '\0');
    WideCharToMultiByte(CP_ACP, 0, wideStr, -1, &result[0], bufferSize, NULL, NULL);
    return result;
}

DWORD GetExplorerPID() {
    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
        std::cerr << " [error] failed to take process snapshot.\n";
        return 0;
    }

    Process32First(hProcessSnapshot, &processEntry);
    do {
        std::string processName = WideCharToString(processEntry.szExeFile);
        if (_stricmp(processName.c_str(), "explorer.exe") == 0) {
            CloseHandle(hProcessSnapshot);
            return processEntry.th32ProcessID;
        }
    } while (Process32Next(hProcessSnapshot, &processEntry));

    CloseHandle(hProcessSnapshot);
    std::cerr << " [error] explorer.exe not found.\n";
    return 0;
}

bool InjectDLL(DWORD pid, const std::string& dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        std::cerr << " [error] failed to open process.\n";
        return false;
    }

    LPVOID allocMem = VirtualAllocEx(hProcess, NULL, dllPath.size(), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!allocMem) {
        std::cerr << " [error] failed to allocate memory in target process.\n";
        CloseHandle(hProcess);
        return false;
    }

    if (!WriteProcessMemory(hProcess, allocMem, dllPath.c_str(), dllPath.size(), NULL)) {
        std::cerr << " [error] failed to write DLL path into target process memory.\n";
        VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    LPVOID loadLibraryAddr = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!loadLibraryAddr) {
        std::cerr << " [error] failed to find LoadLibraryA function.\n";
        VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddr, allocMem, 0, NULL);
    if (!hThread) {
        std::cerr << " [error] failed to create remote thread in target process.\n";
        VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);
    VirtualFreeEx(hProcess, allocMem, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    std::cout << " [success] dll injected successfully.\n";
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << " [info] drag and drop a DLL onto the injector.\n";
        return 1;
    }

    std::string dllPath = argv[1];
    DWORD explorerPID = GetExplorerPID();
    if (!explorerPID) {
        std::cerr << " [error] could not find explorer.exe\n";
        return 1;
    }

    if (!InjectDLL(explorerPID, dllPath)) {
        std::cerr << " [error] dll injection failed.\n";
        return 1;
    }

    return 0;
}