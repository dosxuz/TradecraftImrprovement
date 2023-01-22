#include <stdio.h>
#include <windows.h>

#define DEREF_64( name )*(DWORD64 *)(name)
#define DEREF( name )*(UINT_PTR *)(name)

//structs taken from https://github.com/paranoidninja/PIC-Get-Privileges/blob/main/addresshunter.h because I was too lazy. @NinjaParanoid please don't kill me lol

typedef struct _UNICODE_STR
{
  USHORT Length;
  USHORT MaximumLength;
  PWSTR Buffer;
} UNICODE_STR, *PUNICODE_STR;

//redefine PEB_LDR_DATA struct
typedef struct _PEB_LDR_DATA
{
   DWORD dwLength;
   DWORD dwInitialized;
   LPVOID lpSsHandle;
   LIST_ENTRY InLoadOrderModuleList;
   LIST_ENTRY InMemoryOrderModuleList;
   LIST_ENTRY InInitializationOrderModuleList;
   LPVOID lpEntryInProgress;
} PEB_LDR_DATA, * PPEB_LDR_DATA;

//redefine LDR_DATA_TABLE_ENTRY struct
typedef struct _LDR_DATA_TABLE_ENTRY
{
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID DllBase;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STR FullDllName;
	UNICODE_STR BaseDllName;
	ULONG Flags;
	SHORT LoadCount;
	SHORT TlsIndex;
	LIST_ENTRY HashTableEntry;
	ULONG TimeDateStamp;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

//redefine PEB_FREE_BLOCK struct
typedef struct _PEB_FREE_BLOCK
{
   struct _PEB_FREE_BLOCK * pNext;
   DWORD dwSize;
} PEB_FREE_BLOCK, * PPEB_FREE_BLOCK;

//redefine PEB struct
typedef struct __PEB
{
   BYTE bInheritedAddressSpace;
   BYTE bReadImageFileExecOptions;
   BYTE bBeingDebugged;
   BYTE bSpareBool;
   LPVOID lpMutant;
   LPVOID lpImageBaseAddress;
   PPEB_LDR_DATA pLdr;
   LPVOID lpProcessParameters;
   LPVOID lpSubSystemData;
   LPVOID lpProcessHeap;
   PRTL_CRITICAL_SECTION pFastPebLock;
   LPVOID lpFastPebLockRoutine;
   LPVOID lpFastPebUnlockRoutine;
   DWORD dwEnvironmentUpdateCount;
   LPVOID lpKernelCallbackTable;
   DWORD dwSystemReserved;
   DWORD dwAtlThunkSListPtr32;
   PPEB_FREE_BLOCK pFreeList;
   DWORD dwTlsExpansionCounter;
   LPVOID lpTlsBitmap;
   DWORD dwTlsBitmapBits[2];
   LPVOID lpReadOnlySharedMemoryBase;
   LPVOID lpReadOnlySharedMemoryHeap;
   LPVOID lpReadOnlyStaticServerData;
   LPVOID lpAnsiCodePageData;
   LPVOID lpOemCodePageData;
   LPVOID lpUnicodeCaseTableData;
   DWORD dwNumberOfProcessors;
   DWORD dwNtGlobalFlag;
   LARGE_INTEGER liCriticalSectionTimeout;
   DWORD dwHeapSegmentReserve;
   DWORD dwHeapSegmentCommit;
   DWORD dwHeapDeCommitTotalFreeThreshold;
   DWORD dwHeapDeCommitFreeBlockThreshold;
   DWORD dwNumberOfHeaps;
   DWORD dwMaximumNumberOfHeaps;
   LPVOID lpProcessHeaps;
   LPVOID lpGdiSharedHandleTable;
   LPVOID lpProcessStarterHelper;
   DWORD dwGdiDCAttributeList;
   LPVOID lpLoaderLock;
   DWORD dwOSMajorVersion;
   DWORD dwOSMinorVersion;
   WORD wOSBuildNumber;
   WORD wOSCSDVersion;
   DWORD dwOSPlatformId;
   DWORD dwImageSubsystem;
   DWORD dwImageSubsystemMajorVersion;
   DWORD dwImageSubsystemMinorVersion;
   DWORD dwImageProcessAffinityMask;
   DWORD dwGdiHandleBuffer[34];
   LPVOID lpPostProcessInitRoutine;
   LPVOID lpTlsExpansionBitmap;
   DWORD dwTlsExpansionBitmapBits[32];
   DWORD dwSessionId;
   ULARGE_INTEGER liAppCompatFlags;
   ULARGE_INTEGER liAppCompatFlagsUser;
   LPVOID lppShimData;
   LPVOID lpAppCompatInfo;
   UNICODE_STR usCSDVersion;
   LPVOID lpActivationContextData;
   LPVOID lpProcessAssemblyStorageMap;
   LPVOID lpSystemDefaultActivationContextData;
   LPVOID lpSystemAssemblyStorageMap;
   DWORD dwMinimumStackCommit;
} PEB, * _PPEB;

HMODULE WINAPI MyGetModuleHandle(LPCWSTR sModuleName);
FARPROC WINAPI MyGetProcAddress_my(HMODULE hMod, char *sProcName);

HMODULE WINAPI MyGetModuleHandle(LPCWSTR sModuleName)
{
    //Get pointer to PEB
    _PPEB ppeb = (_PPEB)__readgsqword(0x60);
	ULONG_PTR pLdr = (ULONG_PTR)ppeb->pLdr;
	ULONG_PTR val1 = (ULONG_PTR)((PPEB_LDR_DATA)pLdr)->InMemoryOrderModuleList.Flink;
	PVOID dllBase = NULL;

	while (val1)
	{
		PWSTR DllName = ((PLDR_DATA_TABLE_ENTRY)val1)->BaseDllName.Buffer;
		dllBase = (PVOID)((PLDR_DATA_TABLE_ENTRY)val1)->DllBase;
		if (strcmp((const char*)sModuleName, (const char*)DllName) == 0)
		{
			break;
		}
		val1 = DEREF_64(val1);
	}
	return (HMODULE)dllBase;
}

FARPROC WINAPI MyGetProcAddress(HMODULE hMod, char *sProcName)
{
    UINT64 dllAddress = (UINT64)hMod;
    PIMAGE_DOS_HEADER ImageDosHeader = (PIMAGE_DOS_HEADER)dllAddress;
    PIMAGE_NT_HEADERS NTHeaders = (PIMAGE_NT_HEADERS)(dllAddress + ImageDosHeader->e_lfanew);
    PIMAGE_DATA_DIRECTORY ImageDataDirectory = (PIMAGE_DATA_DIRECTORY)&NTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    //PIMAGE_EXPORT_DIRECTORY ppImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((PBYTE)hMod + OptHeader->DataDirectory[0].VirtualAddress);
    PIMAGE_EXPORT_DIRECTORY ppImageExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(dllAddress + ImageDataDirectory->VirtualAddress);

    PDWORD addressOfFunctions = (PDWORD)(dllAddress + ppImageExportDirectory->AddressOfFunctions);
    PDWORD addressOfNames = (PDWORD)(dllAddress + ppImageExportDirectory->AddressOfNames);
    PWORD addressOfNameOrdinals = (PWORD)(dllAddress + ppImageExportDirectory->AddressOfNameOrdinals);
    PVOID funcAddress = 0x00;

    for (WORD cx = 0; cx < ppImageExportDirectory->NumberOfNames; cx++)
    {
        PCHAR functionName = (PCHAR)(dllAddress + addressOfNames[cx]);

        if (strcmp(sProcName, functionName) == 0)
        {
            PVOID funcAddress = (PVOID)(dllAddress + addressOfFunctions[addressOfNameOrdinals[cx]]);
            return (FARPROC)funcAddress;
        }
    }

    return (FARPROC)funcAddress;
}