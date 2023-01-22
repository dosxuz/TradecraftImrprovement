#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef BOOL (WINAPI *CreateProcessA_t)(
 LPCSTR                lpApplicationName,
 LPSTR                 lpCommandLine,
 LPSECURITY_ATTRIBUTES lpProcessAttributes,
 LPSECURITY_ATTRIBUTES lpThreadAttributes,
 BOOL                  bInheritHandles,
 DWORD                 dwCreationFlags,
 LPVOID                lpEnvironment,
 LPCSTR                lpCurrentDirectory,
 LPSTARTUPINFOA        lpStartupInfo,
 LPPROCESS_INFORMATION lpProcessInformation
);

typedef DWORD (WINAPI *WaitForSingleObject_t)(
 HANDLE hHandle,
 DWORD  dwMilliseconds
);

typedef BOOL (WINAPI *CloseHandle_t)(
 HANDLE hObject
);

int strcmp(const char *X, const char *Y)                                                                                                                                                                   
{                                                                                                                                                                                                          
        while (*X)                                                                                                                                                                                         
        {                                                                                                                                                                                                  
                // if characters differ, or end of the second string is reached                                                                                                                            
                if (*X != *Y) {                                                                                                                                                                            
                        break;                                                                                                                                                                             
                }                                                                                                                                                                                          
                                                                                                                                                                                                           
                // move to the next pair of characters                                                                                                                                                     
                X++;                                                                                                                                                                                       
                Y++;                                                                                                                                                                                       
        }                                                                                                                                                                                                  
                                                                                                                                                                                                           
        // return the ASCII difference after converting `char*` to `unsigned char*`                                                                                                                        
        return *(const unsigned char*)X - *(const unsigned char*)Y;                                                                                                                                        
}