#include "stdafx.h"
#pragma once

#ifdef DEBUG
#define DebugPrint(x, ...) DbgPrint("[LE \"%s\":%d] %s -> " x "\n", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__);
#else
#define DebugPrint(...)
#endif

class Utilities {
public:
	static string GetModuleNameFromAddress(DWORD dwAddress);
	static DWORD Resolve(HMODULE Module, int ordinal);

#ifdef DEBUG
	static void LOG(const CHAR* strFormat, ...);
#endif
	static bool GetSectionInfo(DWORD* pdwAddress, DWORD* pdwLength, const char* pchSectionName);

	static void Reverse(BYTE* pbData, DWORD cbData);
	static void FreezeXbox();
	static void SendToDash(PWCHAR Text, ...);
	static void StartThread(LPTHREAD_START_ROUTINE lpStartAddress);
	static void XNotifyUI(PWCHAR Text);
	static void XNotifyUI(CHAR* Text, ...);
	static void PatchInJump(DWORD* Address, DWORD Destination, bool Linked);
	static void SetSignInStrings();
	static void ApplyPatchData(void* buffer);
	static void HookFunctionStart(DWORD* Address, DWORD* SaveStub, DWORD Destination);
	static void SetLiveBlock(bool enabled);

	static bool CompareData(BYTE* target1, BYTE* target2);
	static bool IsNumberBetween(INT Max, INT Min, INT Num);
	static bool IsBufferEmpty(CHAR* Buffer);
	static bool IsBufferEmpty(BYTE* Buffer);
	static bool CReadFile(const char* FilePath, BYTE* pBuffer, DWORD dwSize);
	static bool CWriteFile(const char* FilePath, const void* pData, DWORD dwSize);
	static bool FileExists(LPCSTR FileName);

	static DWORD GetFunctionSize(DWORD* Function);
	static DWORD SetupResources();
	static DWORD MountPath(std::string &device, std::string &link, bool System);
	static DWORD PatchModuleImport(CHAR* Module, CHAR* ImportedModuleName, DWORD Ordinal, DWORD PatchAddress);
	static DWORD PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, CHAR* ImportedModuleName, DWORD Ordinal, DWORD PatchAddress);
	static DWORD HookFunctionStub(CHAR* ModuleName, DWORD Ordinal, void* Destination);
	static DWORD HookFunctionStub(DWORD _Address, void* Function);
	static int RandomInRange(int Min, int Max);
	static BYTE* GetHVCpukey();
	static BYTE* GetFuseCpukey();
	static FARPROC SecureResolveFunction(DWORD dwOrdinal);
	static FARPROC ResolveFunction(HMODULE hHandle, DWORD Ordinal);
	static FARPROC ResolveFunction(CHAR* ModuleName, DWORD Ordinal);
};