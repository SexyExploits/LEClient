#include "stdafx.h"
#pragma once

#define DebugPrint(x, ...) { printf("[LiveEmulation %s:%d] %s -> "  x  "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__); }
typedef BOOL(*DLAUNCHSETOPTVALBYNAME)(PCHAR optName, PDWORD val);

class Utilities {
public:
	static string GetModuleNameFromAddress(DWORD dwAddress);
	static DWORD Resolve(HMODULE Module, INT ordinal);

#ifdef DEBUG
	static VOID LOG(CONST PCHAR strFormat, ...);
#endif
	static BOOL GetSectionInfo(PDWORD pdwAddress, PDWORD pdwLength, CONST PCHAR pchSectionName);

	static VOID Reverse(PBYTE pbData, DWORD cbData);
	static VOID FreezeXbox();
	static VOID SendToDash(PWCHAR Text, ...);
	static VOID StartThread(LPTHREAD_START_ROUTINE lpStartAddress);
	static VOID XNotifyUI(PWCHAR Text);
	static VOID XNotifyUI(PCHAR Text, ...);
	static VOID PatchInJump(PDWORD Address, DWORD Destination, BOOL Linked);
	static VOID SetSignInStrings();
	static VOID ApplyPatchData(PVOID buffer);

	static DWORD HookFunctionStub(PCHAR ModuleName, DWORD Ordinal, PVOID Destination);
	static DWORD HookFunctionStub(DWORD _Address, PVOID Function);
	static DWORD GetFunctionSize(PDWORD pdwAddress);

	static BOOL CompareData(PBYTE target1, PBYTE target2);
	static BOOL IsNumberBetween(INT Max, INT Min, INT Num);
	static BOOL IsBufferEmpty(PCHAR Buffer);
	static BOOL IsBufferEmpty(PBYTE Buffer);
	static BOOL CReadFile(CONST PCHAR FileName, PBYTE* pBuffer, PDWORD pdwLength);
	static BOOL CWriteFile(CONST PCHAR FilePath, CONST PVOID Data, DWORD Size);
	static BOOL FileExists(LPCSTR FileName);
	static DWORD SetupResources();
	static VOID HookFunctionStart(PDWORD Address, PDWORD SaveStub, DWORD Destination);

	static HRESULT MountPath(std::string &device, std::string &link, BOOL System);
	static DWORD SetLiveBlock(BOOL enabled);
	static DWORD PatchModuleImport(PCHAR Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress);
	static DWORD PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress);

	static INT RandomInRange(INT Min, INT Max);
	static PBYTE GetHVCpukey();
	static PBYTE GetFuseCpukey();
	static FARPROC SecureResolveFunction(DWORD dwOrdinal);
	static FARPROC ResolveFunction(HMODULE mHandle, DWORD Ordinal);
	static FARPROC ResolveFunction(PCHAR ModuleName, DWORD Ordinal);
};