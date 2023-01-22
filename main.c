#include <windows.h>
#include "functions.h"
#include "helper.h"


int WinMain(
 HINSTANCE hInstance,
 HINSTANCE hPrevInstance,
 LPSTR     lpCmdLine,
 int       nShowCmd
) 
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    //CreateProcessA_t CreateProcessA_p = (CreateProcessA_t)GetProcAddress(GetModuleHandle("kernel32.dll"), "CreateProcessA");
    //WaitForSingleObject_t WaitForSingleObject_p = (WaitForSingleObject_t)GetProcAddress(GetModuleHandle("kernel32.dll"), "WaitForSingleObject");
    //CloseHandle_t CloseHandle_p = (CloseHandle_t)GetProcAddress(GetModuleHandle("kernel32.dll"), "CloseHandle");

    CreateProcessA_t CreateProcessA_p = (CreateProcessA_t)MyGetProcAddress(MyGetModuleHandle(L"KERNEL32.DLL"), "CreateProcessA");
    WaitForSingleObject_t WaitForSingleObject_p = (WaitForSingleObject_t)MyGetProcAddress(MyGetModuleHandle(L"KERNEL32.DLL"), "WaitForSingleObject");
    CloseHandle_t CloseHandle_p = (CloseHandle_t)MyGetProcAddress(MyGetModuleHandle(L"KERNEL32.DLL"), "CloseHandle");

    if (!CreateProcessA_p(NULL, "C:\\Windows\\System32\\cmd.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        return -1;
    }

    WaitForSingleObject_p(pi.hProcess, INFINITE);

    CloseHandle_p(pi.hProcess);
    CloseHandle_p(pi.hThread);
    return 0;
}