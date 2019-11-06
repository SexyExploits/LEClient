#pragma once
#include "stdafx.h"

namespace Native {
	namespace Kernel {

		static DWORD XexLoadImageFromMemory(PVOID Image, DWORD ImageSize, LPCSTR ImageName) {
			return Invoker::Invoke<DWORD>(0xf2520dc8, Image, ImageSize, ImageName);
		}

		static PVOID RtlImageXexHeaderField(PVOID XexHeaderBase, DWORD ImageField) {
			return Invoker::Invoke<PVOID>(0x83ae755b, XexHeaderBase, ImageField);
		}

		static BOOL MmIsAddressValid(PVOID Address) {
			return Invoker::Invoke<BOOL>(0xdb1bc5c2, Address);
		}

		static PVOID XeCryptHmacShaFinal(PXECRYPT_HMACSHA_STATE pHmacShaState, PBYTE pbOut, DWORD cbOut) {
			return Invoker::Invoke<PVOID>(0x4c0c7fea, pHmacShaState, pbOut, cbOut);
		}

		static PVOID XeCryptHmacShaUpdate(PXECRYPT_HMACSHA_STATE pHmacShaState, const PBYTE pbInp, DWORD cbInp) {
			return Invoker::Invoke<PVOID>(0x4bf3197d, pHmacShaState, pbInp, cbInp);
		}

		static PVOID XeCryptHmacShaInit(PXECRYPT_HMACSHA_STATE pHmacShaState, const PBYTE pbKey, DWORD cbKey) {
			return Invoker::Invoke<PVOID>(0xd632634b, pHmacShaState, pbKey, cbKey);
		}

		static UCHAR KeGetCurrentProcessType() {
			return Invoker::Invoke<UCHAR>(0x81439a1c);
		}

		static PVOID XeCryptDes3Ecb(CONST PXECRYPT_DES3_STATE pDes3State, CONST PBYTE pbInp, PBYTE pbOut, BOOL fEncrypt) {
			return Invoker::Invoke<PVOID>(0x6ea2ec59, pDes3State, pbInp, pbOut, fEncrypt);
		}

		static PVOID XeCryptDes3Key(PXECRYPT_DES3_STATE pDes3State, CONST PBYTE pbKey) {
			return Invoker::Invoke<PVOID>(0xd837b9a9, pDes3State, pbKey);
		}

		static PVOID XeCryptAesEcb(CONST PXECRYPT_AES_STATE pAesState, CONST PBYTE pbInp, PBYTE pbOut, BOOL fEncrypt) {
			return Invoker::Invoke<PVOID>(0xaf78a41, pAesState, pbInp, pbOut, fEncrypt);
		}

		static PVOID XeCryptAesCbc(PXECRYPT_AES_STATE pAesState, CONST PBYTE pbInp, DWORD cbInp, PBYTE pbOut, PBYTE pbFeed, BOOL fEncrypt) {
			return Invoker::Invoke<PVOID>(0xc64271e5, pAesState, pbInp, cbInp, pbOut, pbFeed, fEncrypt);
		}

		static PVOID XeCryptAesKey(PXECRYPT_AES_STATE pAesState, PBYTE pbKey) {
			return Invoker::Invoke<PVOID>(0xa9c2c539, pAesState, pbKey);
		}

		static PVOID XeCryptHmacSha(CONST PBYTE pbKey, DWORD cbKey, CONST PBYTE pbInp1, DWORD cbInp1, PBYTE pbOut, DWORD cbOut) {
			return Invoker::Invoke<PVOID>(0x8dd778c6, pbKey, cbKey, pbInp1, cbInp1, pbOut, cbOut);
		}

		static PVOID XeCryptRotSumSha(CONST PBYTE pbInp1, DWORD cbInp1, CONST PBYTE pbInp2, DWORD cbInp2, PBYTE pbOut, DWORD cbOut) {
			return Invoker::Invoke<PVOID>(0x5a629980, pbInp1, cbInp1, pbInp2, cbInp2, pbOut, cbOut);
		}

		static PVOID XeCryptMd5(CONST PBYTE pbInp1, DWORD cbInp1, CONST PBYTE pbInp2, DWORD cbInp2, CONST PBYTE pbInp3, DWORD cbInp3, PBYTE pbOut, DWORD cbOut) {
			return Invoker::Invoke<PVOID>(0x69e0a6d1, pbInp1, cbInp1, pbInp2, cbInp2, pbInp3, cbInp3, pbOut, cbOut);
		}

		static PVOID XeCryptSha(CONST PBYTE pbInp1, DWORD cbInp1, CONST PBYTE pbInp2, DWORD cbInp2, CONST PBYTE pbInp3, DWORD cbInp3, PBYTE pbOut, DWORD cbOut) {
			return Invoker::Invoke<PVOID>(0xb815b8e4, pbInp1, cbInp1, pbInp2, cbInp2, pbInp3, cbInp3, pbOut, cbOut);
		}

		static PVOID XeCryptRc4(CONST PBYTE pbKey, DWORD cbKey, PBYTE pbInpOut, DWORD cbInpOut) {
			return Invoker::Invoke<PVOID>(0xd1f3bce2, pbKey, cbKey, pbInpOut, cbInpOut);
		}

		static PVOID XexUnloadImage(HANDLE ModuleHandle) {
			return Invoker::Invoke<PVOID>(0xdfe009f8, ModuleHandle);
		}

		static NTSTATUS XexLoadExecutable(PCHAR szXexName, PHANDLE pHandle, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion) {
			return Invoker::Invoke<NTSTATUS>(0xbd329ed7, szXexName, pHandle, dwModuleTypeFlags, dwMinimumVersion);
		}

		static NTSTATUS XexLoadImage(LPCSTR szXexName, DWORD dwModuleTypeFlags, DWORD dwMinimumVersion, PHANDLE pHandle) {
			return Invoker::Invoke<NTSTATUS>(0x43664b40, szXexName, dwModuleTypeFlags, dwMinimumVersion, pHandle);
		}

		static BOOL XexGetModuleSection(HANDLE ModuleHandle, LPCSTR szSectionName, PVOID* pSectionData, ULONG* pSectionSize) {
			return Invoker::Invoke<BOOL>(0x5e48a4f8, ModuleHandle, szSectionName, pSectionData, pSectionSize);
		}

		static PVOID XexPcToFileHeader(PVOID Address, PLDR_DATA_TABLE_ENTRY* ldatOut) {
			return Invoker::Invoke<PVOID>(0x77a9b548, Address, ldatOut);
		}

		static DWORD ExCreateThread(PHANDLE pHandle, DWORD dwStackSize, LPDWORD lpThreadId, PVOID apiThreadStartup, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlagsMod) {
			return Invoker::Invoke<DWORD>(0x8546aae1, pHandle, dwStackSize, lpThreadId, apiThreadStartup, lpStartAddress, lpParameter, dwCreationFlagsMod);
		}

		static HRESULT ObCreateSymbolicLink(PSTRING SymbolicLinkName, PSTRING DeviceName, BOOL System) {
			return Invoker::Invoke<HRESULT>(0xfe805bca, SymbolicLinkName, DeviceName, System);
		}

		static PVOID RtlInitAnsiString(PANSI_STRING DestinationString, PCSZ DeviceName) {
			return Invoker::Invoke<PVOID>(0xd8f258c5, DestinationString, DeviceName);
		}

		static NTSTATUS ExSetXConfigSetting(WORD Category, WORD Setting, PVOID Buffer, WORD szSetting) {
			return Invoker::Invoke<NTSTATUS>(0xa2ec093b, Category, Setting, Buffer, szSetting);
		}

		static NTSTATUS ExGetXConfigSetting(WORD Category, WORD Setting, PVOID Buffer, WORD Size, PWORD szSetting) {
			return Invoker::Invoke<NTSTATUS>(0x3c2abdb8, Category, Setting, Buffer, Size, szSetting);
		}

		static PVOID MmGetPhysicalAddress(PVOID Address) {
			return Invoker::Invoke<PVOID>(0x267c460a, Address);
		}

		static PVOID VdDisplayFatalError() {
			return Invoker::Invoke<PVOID>(0x393f3210);
		}

		static PVOID HalReturnToFirmware(FIRMWARE_REENTRY PowerDownMode) {
			return Invoker::Invoke<PVOID>(0xbcd16d0b, PowerDownMode);
		}

		static PVOID XeCryptRandom(PBYTE Bytes, DWORD AmountOfBytes) {
			return Invoker::Invoke<PVOID>(0x81f34970, Bytes, AmountOfBytes);
		}
	}
	namespace Xam {

		static DWORD XNetLogonGetExtendedStatus(PDWORD In, PDWORD Out) {
			return Invoker::Invoke<DWORD>(0x2b3d1421, In, Out);
		}

		static BOOL XamCacheReset(XAM_CACHE_FILE_TYPE FileType) {
			return Invoker::Invoke<BOOL>(0x26d06702, FileType);
		}

		static INT NetDll_XNetXnAddrToMachineId(XNCALLER_TYPE xnc, CONST XNADDR pxnaddr, ULONGLONG pqwMachineId) {
			return Invoker::Invoke<INT>(0x97a22c07, xnc, pxnaddr, pqwMachineId);
		}

		static PVOID XNotifyQueueUI(PWCHAR pwszStringParam) {
			return Invoker::Invoke<PVOID>(0xc9711e8b, pwszStringParam);
		}

		static PVOID XNotifyUIGetOptions(PBOOL pfShow, PBOOL pfShowMovie, PBOOL pfPlaySound, PBOOL pfShowIPTV) {
			return Invoker::Invoke<PVOID>(0xb523ea72, pfShow, pfShowMovie, pfPlaySound, pfShowIPTV);
		}

		static PVOID XNotifyUISetOptions(BOOL pfShow, BOOL pfShowMovie, BOOL pfPlaySound, BOOL pfShowIPTV) {
			return Invoker::Invoke<PVOID>(0xf11ab7cf, pfShow, pfShowMovie, pfPlaySound, pfShowIPTV);
		}

		static DWORD XamGetCurrentTitleId() {
			return Invoker::Invoke<DWORD>(0x6e19abd);
		}

		static PVOID Sleep(DWORD Milliseconds) {
			return Invoker::Invoke<PVOID>(0xeefa61ff, Milliseconds);
		}

		static DVD_TRAY_STATE XamLoaderGetDvdTrayState() {
			return Invoker::Invoke<DVD_TRAY_STATE>(0x5fe9b7e2);
		}

		static SOCKET NetDll_socket(XNCALLER_TYPE xnCaller, DWORD af, DWORD type, DWORD protocol) {
			return Invoker::Invoke<SOCKET>(0xf5fe680f, xnCaller, af, type, protocol);
		}

		static DWORD NetDll_closesocket(XNCALLER_TYPE xnCaller, SOCKET socket) {
			return Invoker::Invoke<DWORD>(0x75229afc, xnCaller, socket);
		}

		static DWORD NetDll_shutdown(XNCALLER_TYPE xnCaller, SOCKET socket, DWORD method) {
			return Invoker::Invoke<DWORD>(0x956396f8, xnCaller, socket, method);
		}

		static DWORD NetDll_setsockopt(XNCALLER_TYPE xnCaller, SOCKET socket, DWORD level, DWORD option, const char* value, DWORD length) {
			return Invoker::Invoke<DWORD>(0x96627da7, xnCaller, socket, level, option, value, length);
		}

		static DWORD NetDll_connect(XNCALLER_TYPE xnCaller, SOCKET socket, const sockaddr* name, DWORD length) {
			return Invoker::Invoke<DWORD>(0x2fc51727, xnCaller, socket, name, length);
		}

		static DWORD NetDll_recv(XNCALLER_TYPE xnCaller, SOCKET socket, const char* buffer, DWORD length, DWORD flags) {
			return Invoker::Invoke<DWORD>(0xbcd9702b, xnCaller, socket, buffer, length, flags);
		}

		static DWORD NetDll_send(XNCALLER_TYPE xnCaller, SOCKET socket, const char* buffer, DWORD length, DWORD flags) {
			return Invoker::Invoke<DWORD>(0xc290de6d, xnCaller, socket, buffer, length, flags);
		}

		static DWORD NetDll_WSAStartupEx(XNCALLER_TYPE xnCaller, WORD versionA, WSADATA* wsad, DWORD versionB) {
			return Invoker::Invoke<DWORD>(0x4578f873, xnCaller, versionA, wsad, versionB);
		}

		static DWORD NetDll_XNetStartup(XNCALLER_TYPE xnCaller, XNetStartupParams* xnsp) {
			return Invoker::Invoke<DWORD>(0x97c394b0, xnCaller, xnsp);
		}
	}
}
