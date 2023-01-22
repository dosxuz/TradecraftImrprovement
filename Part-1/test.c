#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(NULL, "C:\\Windows\\System32\\cmd.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        return -1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}