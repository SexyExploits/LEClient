#include "stdafx.h"

NativeRegistration Invoker::g_natives[256];
DWORD Invoker::m_invokingAddress;
NativeManager Invoker::m_context;

inline INT ProtectOrdinal(INT Ordinal, INT Key) {
	INT ConfuserX = 0x963 ^ 0x555;
	INT ConfuserY = 0x143; ConfuserY ^= ConfuserX;
	INT ConfuserZ = 0x237; ConfuserZ ^= ConfuserY;
	INT ConfuserD = 0x980; ConfuserD ^= ConfuserZ;

	Ordinal ^= Key + 0x90 * (0x4 - 0x2);
	Ordinal ^= Key + ConfuserY * (0x2 + 0x4);

	Ordinal ^= Key;
	Ordinal ^= ConfuserD;
	Ordinal ^= ConfuserZ;
	Ordinal ^= ConfuserY;
	Ordinal ^= ConfuserX;
	Ordinal ^= Key;

	for (INT i = 0; i < 0x5; i++) {
		Ordinal ^= i;
		Ordinal ^= (i * 0x5 + 50) > 35 ? 0x80 : 0x90;
	}

	for (INT i = 0; i < 0xC; i++) {
		ConfuserX ^= i;
		ConfuserX ^= (Key * 0x5 + 29) > 100 ? 0xD : 0x1;
	}

	Ordinal ^= ConfuserX;
	Ordinal ^= Key;
	return Ordinal;
}

VOID Invoker::RegisterNative(DWORD NativeAddress, NativeHandler Handler) {
	NativeRegistration* Registration = &g_natives[NativeAddress & 0xFF];
	if (Registration != nullptr) {
		while (Registration->m_next != nullptr) {
			Registration = Registration->m_next;
		}

		if (Registration->GetCount() == 9) {
			Registration->m_next = static_cast<NativeRegistration*>(malloc(sizeof(NativeRegistration)));
			Registration = Registration->m_next;
		}

		Registration->SetAddress(Registration->GetCount(), NativeAddress);
		Registration->SetFunction(Registration->GetCount(), Handler);
		Registration->SetCount(Registration->GetCount() + 1);
		GetIntegrityManager()->Add(static_cast<PVOID>(Handler), 0x4);

#ifdef DEBUG
		DebugPrint("[RegisterFunction] Native 0x%X registered, handler => %p\n", NativeAddress, (DWORD)Handler);
#endif
	}
}

inline NativeHandler Invoker::GetNativeHandler(DWORD NativeAddress){
	NativeHandler Handler = nullptr;
	NativeRegistration* Registration = &g_natives[NativeAddress & 0xFF];

	if (Registration != nullptr) {
		for (; Registration; Registration = Registration->m_next) {
			for (auto i = 0; i < Registration->GetCount(); i++) {
				if (NativeAddress == Registration->GetAddress(i)) {
					Handler = Registration->GetFunction(i);
					break;
				}
			}
		}
	}
	return Handler;
}

namespace NativeHandlers {

	VOID XeCryptHmacShaFinal(NativeCallContext* Context) {

		PXECRYPT_HMACSHA_STATE pHmacShaState = Context->GetArgument <PXECRYPT_HMACSHA_STATE>(0);
		CONST PBYTE pbOut = Context->GetArgument <CONST PBYTE>(1);
		DWORD cbOut = Context->GetArgument <DWORD>(2);

		INT Ordinal = ProtectOrdinal(0x5C06, 0x818); // 0x181

		PVOID Return = ((PVOID(*)(PXECRYPT_HMACSHA_STATE, CONST PBYTE, DWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pHmacShaState, pbOut, cbOut);
		Context->SetResult(0, Return);
	}

	VOID XeCryptHmacShaUpdate(NativeCallContext* Context) {

		PXECRYPT_HMACSHA_STATE pHmacShaState = Context->GetArgument <PXECRYPT_HMACSHA_STATE>(0);
		CONST PBYTE pbInp = Context->GetArgument <CONST PBYTE>(1);
		DWORD cbInp = Context->GetArgument <DWORD>(2);

		INT Ordinal = ProtectOrdinal(0x5494, 0x187); // 0x180

		PVOID Return = ((PVOID(*)(PXECRYPT_HMACSHA_STATE, CONST PBYTE, DWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pHmacShaState, pbInp, cbInp);
		Context->SetResult(0, Return);
	}

	VOID XeCryptHmacShaInit(NativeCallContext* Context) {

		PXECRYPT_HMACSHA_STATE pHmacShaState = Context->GetArgument <PXECRYPT_HMACSHA_STATE>(0);
		CONST PBYTE pbKey = Context->GetArgument <CONST PBYTE>(1);
		DWORD cbKey = Context->GetArgument <DWORD>(2);

		INT Ordinal = ProtectOrdinal(0x7AC1, 0xF71); // 0x17F

		PVOID Return = ((PVOID(*)(PXECRYPT_HMACSHA_STATE, CONST PBYTE, DWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pHmacShaState, pbKey, cbKey);
		Context->SetResult(0, Return);
	}

	VOID KeGetCurrentProcessType(NativeCallContext* Context) {

		INT Ordinal = ProtectOrdinal(0xB2E8, 0x6661); // 0x66

		UCHAR Return = ((UCHAR(*)())Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))();
		Context->SetResult(0, Return);
	}

	// crypto
	VOID XeCryptDes3Ecb(NativeCallContext* Context) {

		PXECRYPT_DES3_STATE pDes3State = Context->GetArgument <PXECRYPT_DES3_STATE>(0);
		CONST PBYTE pbInp = Context->GetArgument <CONST PBYTE>(1);
		PBYTE pbOut = Context->GetArgument <PBYTE>(2);
		BOOL fEncrypt = Context->GetArgument <BOOL>(3); 

		INT Ordinal = ProtectOrdinal(0xEDF0, 0x971A); // 0x179

		PVOID Return = ((PVOID(*)(CONST PXECRYPT_DES3_STATE, CONST PBYTE, PBYTE, BOOL))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pDes3State, pbInp, pbOut, fEncrypt);
		Context->SetResult(0, Return);
	}

	VOID XeCryptDes3Key(NativeCallContext* Context) {

		PXECRYPT_DES3_STATE pDes3State = Context->GetArgument <PXECRYPT_DES3_STATE>(0);
		CONST PBYTE pbKey = Context->GetArgument <CONST PBYTE>(1);

		INT Ordinal = ProtectOrdinal(0xDDF2, 0x871D); // 0x178

		PVOID Return = ((PVOID(*)(PXECRYPT_DES3_STATE, CONST PBYTE))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pDes3State, pbKey);
		Context->SetResult(0, Return);
	}

	VOID XeCryptAesEcb(NativeCallContext* Context) {

		CONST PXECRYPT_AES_STATE pAesState = Context->GetArgument <CONST PXECRYPT_AES_STATE>(0);
		CONST PBYTE pbInp = Context->GetArgument <CONST PBYTE>(1);
		PBYTE pbOut = Context->GetArgument <PBYTE>(2);
		BOOL fEncrypt = Context->GetArgument <BOOL>(3);

		INT Ordinal = ProtectOrdinal(0xF3DB, 0xA512); // 0x15A

		PVOID Return = ((PVOID(*)(CONST PXECRYPT_AES_STATE, CONST PBYTE, PBYTE, BOOL))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pAesState, pbInp, pbOut, fEncrypt);
		Context->SetResult(0, Return);
	}

	VOID XeCryptAesCbc(NativeCallContext* Context) {

		PXECRYPT_AES_STATE pAesState = Context->GetArgument <PXECRYPT_AES_STATE>(0);
		CONST PBYTE pbInp = Context->GetArgument <CONST PBYTE>(1);
		DWORD cbInp = Context->GetArgument <DWORD>(2);
		PBYTE pbOut = Context->GetArgument <PBYTE>(3);
		PBYTE pbFeed = Context->GetArgument <PBYTE>(4);
		BOOL fEncrypt = Context->GetArgument <BOOL>(5);

		INT Ordinal = ProtectOrdinal(0x10395, 0xB521); // 0x15B

		PVOID Return = ((PVOID(*)(PXECRYPT_AES_STATE, CONST PBYTE, DWORD, PBYTE, PBYTE, BOOL))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pAesState, pbInp, cbInp, pbOut, pbFeed, fEncrypt);
		Context->SetResult(0, Return);
	}

	VOID XeCryptAesKey(NativeCallContext* Context) {

		PXECRYPT_AES_STATE pAesState = Context->GetArgument <PXECRYPT_AES_STATE>(0);
		PBYTE pbKey = Context->GetArgument <PBYTE>(1);

		INT Ordinal = ProtectOrdinal(0xE807, 0x9D51); // 0x159

		PVOID Return = ((PVOID(*)(PXECRYPT_AES_STATE, PBYTE))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pAesState, pbKey);
		Context->SetResult(0, Return);
	}

	VOID XeCryptHmacSha(NativeCallContext* Context) {

		CONST PBYTE pbKey = Context->GetArgument <CONST PBYTE>(0);
		DWORD cbKey = Context->GetArgument <DWORD>(1);
		CONST PBYTE pbInp1 = Context->GetArgument <CONST PBYTE>(2);
		DWORD cbInp1 = Context->GetArgument <DWORD>(3);
		PBYTE pbOut = Context->GetArgument <PBYTE>(4);
		DWORD cbOut = Context->GetArgument <DWORD>(5);

		INT Ordinal = ProtectOrdinal(0x7C0B, 0x281A); // 0x182

		PVOID Return = ((PVOID(*)(CONST PBYTE, DWORD, CONST PBYTE, DWORD, CONST PBYTE, DWORD, CONST PBYTE, DWORD, PBYTE, DWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pbKey, cbKey, pbInp1, cbInp1, NULL, NULL, NULL, NULL, pbOut, cbOut);
		Context->SetResult(0, Return);
	}

	VOID XeCryptRotSumSha(NativeCallContext* Context) {

		CONST PBYTE pbInp1 = Context->GetArgument <CONST PBYTE>(0);
		DWORD cbInp1 = Context->GetArgument <DWORD>(1);
		CONST PBYTE pbInp2 = Context->GetArgument <CONST PBYTE>(2);
		DWORD cbInp2 = Context->GetArgument <DWORD>(3);
		PBYTE pbOut = Context->GetArgument <PBYTE>(4);
		DWORD cbOut = Context->GetArgument <DWORD>(5);

		INT Ordinal = ProtectOrdinal(0x13E02, 0xEA1F); // 0x18E

		PVOID Return = ((PVOID(*)(CONST PBYTE, DWORD, CONST PBYTE, DWORD, PBYTE, DWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pbInp1, cbInp1, pbInp2, cbInp2, pbOut, cbOut);
		Context->SetResult(0, Return);
	}

	VOID XeCryptMd5(NativeCallContext* Context) {

		CONST PBYTE pbInp1 = Context->GetArgument <CONST PBYTE>(0);
		DWORD cbInp1 = Context->GetArgument <DWORD>(1);
		CONST PBYTE pbInp2 = Context->GetArgument <CONST PBYTE>(2);
		DWORD cbInp2 = Context->GetArgument <DWORD>(3);
		CONST PBYTE pbInp3 = Context->GetArgument <CONST PBYTE>(4);
		DWORD cbInp3 = Context->GetArgument <DWORD>(5);
		PBYTE pbOut = Context->GetArgument <PBYTE>(6);
		DWORD cbOut = Context->GetArgument <DWORD>(7);

		INT Ordinal = ProtectOrdinal(0xCC18, 0x7810); // 0x187

		PVOID Return = ((PVOID(*)(CONST PBYTE, DWORD, CONST PBYTE, DWORD, CONST PBYTE, DWORD, PBYTE, DWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pbInp1, cbInp1, pbInp2, cbInp2, pbInp3, cbInp3, pbOut, cbOut);
		Context->SetResult(0, Return);
	}

	VOID XeCryptSha(NativeCallContext* Context) {
		
		CONST PBYTE pbInp1 = Context->GetArgument <CONST PBYTE>(0);
		DWORD cbInp1 = Context->GetArgument <DWORD>(1);
		CONST PBYTE pbInp2 = Context->GetArgument <CONST PBYTE>(2);
		DWORD cbInp2 = Context->GetArgument <DWORD>(3);
		CONST PBYTE pbInp3 = Context->GetArgument <CONST PBYTE>(4);
		DWORD cbInp3 = Context->GetArgument <DWORD>(5);
		PBYTE pbOut = Context->GetArgument <PBYTE>(6);
		DWORD cbOut = Context->GetArgument <DWORD>(7);

		INT Ordinal = ProtectOrdinal(0x578C, 0x291); // 0x192

		PVOID Return = ((PVOID(*)(CONST PBYTE, DWORD, CONST PBYTE, DWORD, CONST PBYTE, DWORD, PBYTE, DWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pbInp1, cbInp1, pbInp2, cbInp2, pbInp3, cbInp3, pbOut, cbOut);
		Context->SetResult(0, Return);
	}

	VOID XeCryptRc4(NativeCallContext* Context) {

		CONST PBYTE pbKey = Context->GetArgument<CONST PBYTE>(0);
		DWORD cbKey = Context->GetArgument<DWORD>(1);
		PBYTE pbInpOut = Context->GetArgument<PBYTE>(2);
		DWORD cbInpOut = Context->GetArgument<DWORD>(3);

		INT Ordinal = ProtectOrdinal(0x589D, 0xD83); // 0x18D

		PVOID Return = ((PVOID(*)(CONST PBYTE, DWORD, PBYTE, DWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pbKey, cbKey, pbInpOut, cbInpOut);
		Context->SetResult(0, Return);
	}
	////////////////

	VOID XexLoadImageFromMemory(NativeCallContext* Context) {

		PVOID Image = Context->GetArgument<PVOID>(0);
		DWORD ImageSize = Context->GetArgument<DWORD>(1);
		LPCSTR ImageName = Context->GetArgument<LPCSTR>(2);

		INT Ordinal = ProtectOrdinal(0x5fc3, 0xA4A); // 0x19a

		DWORD Return = ((DWORD(*)(PVOID, DWORD, LPCSTR, DWORD, DWORD, PHANDLE))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(Image, ImageSize, ImageName, 0x00000008, NULL, NULL);
		Context->SetResult(0, Return);
	}

	VOID RtlImageXexHeaderField(NativeCallContext* Context) {

		PVOID XexHeaderBase = Context->GetArgument<PVOID>(0);
		DWORD ImageField = Context->GetArgument<DWORD>(1);

		INT Ordinal = ProtectOrdinal(0x100CE, 0xB436); // 0x12B

		PVOID Return = ((PVOID(*)(PVOID, DWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(XexHeaderBase, ImageField);
		Context->SetResult(0, Return);
	}

	VOID MmIsAddressValid(NativeCallContext* Context) {

		PVOID Address = Context->GetArgument<PVOID>(0);

		INT Ordinal = ProtectOrdinal(0x7723, 0x220F); // 0xBF

		BOOL Return = ((BOOL(*)(PVOID))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(Address);
		Context->SetResult(0, Return);
	}

	VOID XexUnloadImage(NativeCallContext* Context) {

		HANDLE moduleHandle = Context->GetArgument<HANDLE>(0);

		INT Ordinal = ProtectOrdinal(0x6121, 0x1313); // 0x1A1

		PVOID Return = ((PVOID(*)(HANDLE))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(moduleHandle);
		Context->SetResult(0, Return);
	}

	VOID XexLoadExecutable(NativeCallContext* Context) {

		PCHAR szXexName = Context->GetArgument<PCHAR>(0);
		PHANDLE pHandle = Context->GetArgument<PHANDLE>(1);
		DWORD dwModuleTypeFlags = Context->GetArgument<DWORD>(2);
		DWORD dwMinimumVersion = Context->GetArgument<DWORD>(3);

		INT Ordinal = ProtectOrdinal(0xEC19, 0x9812); // 0x198 

		NTSTATUS Return = ((NTSTATUS(*)(PCHAR, PHANDLE, DWORD, DWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(szXexName, pHandle, dwModuleTypeFlags, dwMinimumVersion);
		Context->SetResult(0, Return);
	}

	VOID XexLoadImage(NativeCallContext* Context) {

		LPCSTR szXexName = Context->GetArgument<LPCSTR>(0);
		DWORD dwModuleTypeFlags = Context->GetArgument<DWORD>(1);
		DWORD dwMinimumVersion = Context->GetArgument<DWORD>(2);
		PHANDLE pHandle = Context->GetArgument<PHANDLE>(3);

		INT Ordinal = ProtectOrdinal(0xEF18, 0x9912); // 0x199 

		NTSTATUS Return = ((NTSTATUS(*)(LPCSTR, DWORD, DWORD, PHANDLE))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(szXexName, dwModuleTypeFlags, dwMinimumVersion, pHandle);
		Context->SetResult(0, Return);
	}

	VOID XexGetModuleSection(NativeCallContext* Context) {

		HANDLE ModuleHandle = Context->GetArgument<HANDLE>(0);
		LPCSTR  szSectionName = Context->GetArgument<LPCSTR>(1);
		PVOID*  pSectionData = Context->GetArgument<PVOID*>(2);
		ULONG*  pSectionSize = Context->GetArgument<ULONG*>(3);

		INT Ordinal = ProtectOrdinal(0xBF1A, 0x691F); // 0x196 

		BOOL Return = ((BOOL(*)(HANDLE, LPCSTR, PVOID*, ULONG*))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(ModuleHandle, szSectionName, pSectionData, pSectionSize);
		Context->SetResult(0, Return);
	}

	VOID XexPcToFileHeader(NativeCallContext* Context) {

		PVOID Address = Context->GetArgument<PVOID>(0);
		PLDR_DATA_TABLE_ENTRY*  ldatOut = Context->GetArgument<PLDR_DATA_TABLE_ENTRY*>(1);

		INT Ordinal = ProtectOrdinal(0x599D, 0xC92); // 0x19C 

		PVOID Return = ((PVOID(*)(PVOID, PLDR_DATA_TABLE_ENTRY*))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(Address, ldatOut);
		Context->SetResult(0, Return);
	}

	VOID ExCreateThread(NativeCallContext* Context) {

		PHANDLE pHandle = Context->GetArgument<PHANDLE>(0);
		DWORD dwStackSize = Context->GetArgument<DWORD>(1);
		LPDWORD lpThreadId = Context->GetArgument<LPDWORD>(2);
		PVOID apiThreadStartup = Context->GetArgument<PVOID>(3);
		LPTHREAD_START_ROUTINE lpStartAddress = Context->GetArgument<LPTHREAD_START_ROUTINE>(4);
		LPVOID lpParameter = Context->GetArgument<LPVOID>(5);
		DWORD dwCreationFlagsMod = Context->GetArgument<DWORD>(6);

		INT Ordinal = ProtectOrdinal(0x6A9E, 0x1D04); // 0x0D 

		DWORD Return = ((DWORD(*)(PHANDLE, DWORD, LPDWORD, PVOID, LPTHREAD_START_ROUTINE, LPVOID, DWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(pHandle, dwStackSize, lpThreadId, apiThreadStartup, lpStartAddress, lpParameter, dwCreationFlagsMod);
		Context->SetResult(0, Return);
	}

	VOID ObCreateSymbolicLink(NativeCallContext* Context) {

		PSTRING SymbolicLinkName = Context->GetArgument<PSTRING>(0);
		PSTRING DeviceName = Context->GetArgument<PSTRING>(1);
		BOOL System = Context->GetArgument<BOOL>(2);

		INT Ordinal = ProtectOrdinal(0x57F0, 0x124); // 0x103 

		HRESULT Return = ((HRESULT(*)(PSTRING, PSTRING, BOOL))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(SymbolicLinkName, DeviceName, System);
		Context->SetResult(0, Return);
	}

	VOID RtlInitAnsiString(NativeCallContext* Context) {

		PANSI_STRING DestinationString = Context->GetArgument<PANSI_STRING>(0);
		PCSZ DeviceName = Context->GetArgument<PCSZ>(1);

		INT Ordinal = ProtectOrdinal(0x57E2, 0x121); // 0x12C 

		PVOID Return = ((PVOID(*)(PANSI_STRING, PCSZ))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(DestinationString, DeviceName);
		Context->SetResult(0, Return);
	}

	VOID ExSetXConfigSetting(NativeCallContext* Context) {

		WORD Category = Context->GetArgument<WORD>(0);
		WORD Setting = Context->GetArgument<WORD>(1);
		PVOID Buffer = Context->GetArgument<PVOID>(2);
		WORD szSetting = Context->GetArgument<WORD>(3);

		INT Ordinal = ProtectOrdinal(0x6146, 0x1551); // 0x18 

		NTSTATUS Return = ((NTSTATUS(*)(WORD, WORD, PVOID, WORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(Category, Setting, Buffer, szSetting);
		Context->SetResult(0, Return);
	}

	VOID ExGetXConfigSetting(NativeCallContext* Context) {

		WORD Category = Context->GetArgument<WORD>(0);
		WORD Setting = Context->GetArgument<WORD>(1);
		PVOID Buffer = Context->GetArgument<PVOID>(2);
		WORD Size = Context->GetArgument<WORD>(3);
		PWORD szSetting = Context->GetArgument<PWORD>(4);

		INT Ordinal = ProtectOrdinal(0x62F2, 0x1535); // 0x10 

		NTSTATUS Return = ((NTSTATUS(*)(WORD, WORD, PVOID, WORD, PWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(Category, Setting, Buffer, Size, szSetting);
		Context->SetResult(0, Return);
	}

	VOID MmGetPhysicalAddress(NativeCallContext* Context) {
		PVOID Address = Context->GetArgument<PVOID>(0);

		INT Ordinal = ProtectOrdinal(0x5EB0, 0xBE1); // 0xBE 

		PVOID Return = ((PVOID(*)(PVOID))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(Address);
		Context->SetResult(0, Return);
	}

	VOID VdDisplayFatalError(NativeCallContext* Context) {

		INT Ordinal = ProtectOrdinal(0x5CE0, 0x945); // 0x1B2 

		PVOID Return = ((PVOID(*)(DWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(0x4F);
		Context->SetResult(0, Return);
	}

	VOID HalReturnToFirmware(NativeCallContext* Context) {

		FIRMWARE_REENTRY PowerDownMode = Context->GetArgument<FIRMWARE_REENTRY>(0);
		INT Ordinal = ProtectOrdinal(0x122ED, 0xD6D6); // 0x28 

		PVOID Return = ((PVOID(*)(FIRMWARE_REENTRY))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(PowerDownMode);

		Context->SetResult(0, Return);
	}

	VOID XeCryptRandom(NativeCallContext* Context) {

		PBYTE Bytes = Context->GetArgument<PBYTE>(0);
		DWORD AmountOfBytes = Context->GetArgument<DWORD>(1);
		INT Ordinal = ProtectOrdinal(0x5191, 0x48C); // 0x18A

		PVOID Return = ((PVOID(*)(PBYTE, DWORD))Utilities::ResolveFunction((HMODULE)LE::Kernelhandle, Ordinal))(Bytes, AmountOfBytes);
		Context->SetResult(0, Return);
	}

	/////////////////////////////////////// XAM /////////////////////////////////////////////////

	VOID XNetLogonGetExtendedStatus(NativeCallContext* Context) {

		PDWORD Input = Context->GetArgument<PDWORD>(0);
		PDWORD Output = Context->GetArgument<PDWORD>(1);

		INT Ordinal = ProtectOrdinal(0xB314, 0x101B8); // 0x13B

		DWORD Return = ((DWORD(*)(PDWORD, PDWORD))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, 0x13B))(Input, Output);
		Context->SetResult(0, Return);
	}

	VOID XamCacheReset(NativeCallContext* Context) {

		XAM_CACHE_FILE_TYPE FileType = Context->GetArgument<XAM_CACHE_FILE_TYPE>(0);
		 
		INT Ordinal = ProtectOrdinal(0x51A7, 0x783); // 0x2B7

		BOOL Return = ((BOOL(*)(XAM_CACHE_FILE_TYPE))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(FileType);
		Context->SetResult(0, Return);
	}

	VOID NetDll_XNetXnAddrToMachineId(NativeCallContext* Context) {

		XNCALLER_TYPE xnc = Context->GetArgument<XNCALLER_TYPE>(0);
		CONST XNADDR pxnaddr = Context->GetArgument<CONST XNADDR>(1);
		ULONGLONG pqwMachineId = Context->GetArgument<ULONGLONG>(2);

		INT Ordinal = ProtectOrdinal(0x5410, 0x043); // 0X40

		INT Return = ((INT(*)(XNCALLER_TYPE, CONST XNADDR, ULONGLONG))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(xnc, pxnaddr, pqwMachineId);
		Context->SetResult(0, Return);
	}

	VOID XNotifyQueueUI(NativeCallContext* Context) {

		PWCHAR pwszStringParam = Context->GetArgument<PWCHAR>(0);

		INT Ordinal = ProtectOrdinal(0x5C66, 0x0929); // 0x290

		PVOID Return = ((PVOID(*)(...))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(XNOTIFYUI_TYPE_PREFERRED_REVIEW, XUSER_INDEX_ANY, XNOTIFYUI_PRIORITY_HIGH, pwszStringParam, NULL);
		Context->SetResult(0, Return);
	}

	VOID XNotifyUIGetOptions(NativeCallContext* Context) {

		PBOOL pfShow = Context->GetArgument<PBOOL>(0);
		PBOOL pfShowMovie = Context->GetArgument<PBOOL>(1);
		PBOOL pfPlaySound = Context->GetArgument<PBOOL>(2);
		PBOOL pfShowIPTV = Context->GetArgument<PBOOL>(3);

		INT Ordinal = ProtectOrdinal(0x8C1F, 0x391F); // 0x293

		PVOID Return = ((PVOID(*)(PBOOL, PBOOL, PBOOL, PBOOL))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(pfShow, pfShowMovie, pfPlaySound, pfShowIPTV);
		Context->SetResult(0, Return);
	}

	VOID XNotifyUISetOptions(NativeCallContext* Context) {

		BOOL pfShow = Context->GetArgument<BOOL>(0);
		BOOL pfShowMovie = Context->GetArgument<BOOL>(1);
		BOOL pfPlaySound = Context->GetArgument<BOOL>(2);
		BOOL pfShowIPTV = Context->GetArgument<BOOL>(3);

		INT Ordinal = ProtectOrdinal(0x5C63, 0x922); // 0x292

		PVOID Return = ((PVOID(*)(BOOL, BOOL, BOOL, BOOL))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(pfShow, pfShowMovie, pfPlaySound, pfShowIPTV);
		Context->SetResult(0, Return);
	}

	VOID XamGetCurrentTitleId(NativeCallContext* Context) {

		INT Ordinal = ProtectOrdinal(0x14836, 0xFC2A); // 0x1CF

		DWORD Return = ((DWORD(*)())Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))();
		Context->SetResult(0, Return);
	}

	VOID Sleep(NativeCallContext* Context) {

		DWORD Milliseconds = Context->GetArgument<DWORD>(0);
		INT Ordinal = ProtectOrdinal(0x57D5, 0x634); // 0x436

		PVOID Return = ((PVOID(*)(DWORD))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(Milliseconds);
		Context->SetResult(0, Return);
	}

	//  Networking
	VOID NetDll_socket(NativeCallContext* Context) {

		XNCALLER_TYPE xnCaller = Context->GetArgument<XNCALLER_TYPE>(0);
		DWORD af = Context->GetArgument<DWORD>(1);
		DWORD type = Context->GetArgument<DWORD>(2);
		DWORD protocol = Context->GetArgument<DWORD>(3);
		INT Ordinal = ProtectOrdinal(0x502A, 0x5FA); // 0x03

		SOCKET Return = ((SOCKET(*)(XNCALLER_TYPE, INT, INT, INT))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(xnCaller, af, type, protocol);
		Context->SetResult(0, Return);
	}

	VOID NetDll_closesocket(NativeCallContext* Context) {

		XNCALLER_TYPE xnCaller = Context->GetArgument<XNCALLER_TYPE>(0);
		SOCKET socket = Context->GetArgument<SOCKET>(1);
		DWORD Ordinal = ProtectOrdinal(0x5D2F, 0x6FC); // 0x04

		DWORD Return = ((DWORD(*)(XNCALLER_TYPE, SOCKET))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(xnCaller, socket);
		Context->SetResult(0, Return);
	}

	VOID NetDll_shutdown(NativeCallContext* Context) {

		XNCALLER_TYPE xnCaller = Context->GetArgument<XNCALLER_TYPE>(0);
		SOCKET socket = Context->GetArgument<SOCKET>(1);
		DWORD method = Context->GetArgument<DWORD>(2);
		DWORD Ordinal = ProtectOrdinal(0xC9B7, 0x7D65); // 0x05

		DWORD Return = ((DWORD(*)(XNCALLER_TYPE, SOCKET, DWORD))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(xnCaller, socket, method);
		Context->SetResult(0, Return);
	}

	VOID NetDll_setsockopt(NativeCallContext* Context) {

		XNCALLER_TYPE xnCaller = Context->GetArgument<XNCALLER_TYPE>(0);
		SOCKET socket = Context->GetArgument<SOCKET>(1);
		DWORD level = Context->GetArgument<DWORD>(2);
		DWORD option = Context->GetArgument<DWORD>(3);
		const char* value = Context->GetArgument<const char*>(4);
		DWORD length = Context->GetArgument<DWORD>(5);
		DWORD Ordinal = ProtectOrdinal(0xDF85, 0x8A15); // 0x07

		DWORD Return = ((DWORD(*)(XNCALLER_TYPE, SOCKET, DWORD, DWORD, const char*, DWORD))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(xnCaller, socket, level, option, value, length);
		Context->SetResult(0, Return);
	}

	VOID NetDll_connect(NativeCallContext* Context) {

		XNCALLER_TYPE xnCaller = Context->GetArgument<XNCALLER_TYPE>(0);
		SOCKET socket = Context->GetArgument<SOCKET>(1);
		sockaddr* name = Context->GetArgument<sockaddr*>(2);
		DWORD length = Context->GetArgument<DWORD>(3);
		DWORD Ordinal = ProtectOrdinal(0x5DC7, 0x9DC); // 0x0C

		DWORD Return = ((DWORD(*)(XNCALLER_TYPE, SOCKET, sockaddr*, DWORD))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(xnCaller, socket, name, length);
		Context->SetResult(0, Return);
	}

	VOID NetDll_recv(NativeCallContext* Context) {

		XNCALLER_TYPE xnCaller = Context->GetArgument<XNCALLER_TYPE>(0);
		SOCKET socket = Context->GetArgument<SOCKET>(1);
		PCHAR buffer = Context->GetArgument<PCHAR>(2);
		INT length = Context->GetArgument<INT>(3);
		INT flags = Context->GetArgument<INT>(4);
		DWORD Ordinal = ProtectOrdinal(0xA44D, 0x5050); // 0x12

		INT Return = ((INT(*)(XNCALLER_TYPE, SOCKET, PCHAR, INT, INT))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(xnCaller, socket, buffer, length, flags);
		Context->SetResult(0, Return);
	}

	VOID NetDll_send(NativeCallContext* Context) {

		XNCALLER_TYPE xnCaller = Context->GetArgument<XNCALLER_TYPE>(0);
		SOCKET socket = Context->GetArgument<SOCKET>(1);
		PCHAR buffer = Context->GetArgument<PCHAR>(2);
		INT length = Context->GetArgument<INT>(3);
		INT flags = Context->GetArgument<INT>(4);
		DWORD Ordinal = ProtectOrdinal(0xA448, 0x5051); // 0x16

		INT Return = ((INT(*)(XNCALLER_TYPE, SOCKET, PCHAR, INT, INT))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(xnCaller, socket, buffer, length, flags);
		Context->SetResult(0, Return);
	}

	VOID NetDll_WSAStartupEx(NativeCallContext* Context) {

		XNCALLER_TYPE xnCaller = Context->GetArgument<XNCALLER_TYPE>(0);
		WORD versionA = Context->GetArgument<WORD>(1);
		WSADATA* wsad = Context->GetArgument<WSADATA*>(2);
		DWORD versionB = Context->GetArgument<DWORD>(3);
		DWORD Ordinal = ProtectOrdinal(0xA465, 0x5052); // 0x24

		DWORD Return = ((DWORD(*)(XNCALLER_TYPE, WORD, WSADATA*, DWORD))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(xnCaller, versionA, wsad, versionB);
		Context->SetResult(0, Return);
	}

	VOID NetDll_XNetStartup(NativeCallContext* Context) {

		XNCALLER_TYPE xnCaller = Context->GetArgument<XNCALLER_TYPE>(0);
		XNetStartupParams* xnsp = Context->GetArgument<XNetStartupParams*>(1);
		DWORD Ordinal = ProtectOrdinal(0xA473, 0x5053); // 0x33

		DWORD Return = ((DWORD(*)(XNCALLER_TYPE, XNetStartupParams*))Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))(xnCaller, xnsp);
		Context->SetResult(0, Return);
	}

	VOID XamLoaderGetDvdTrayState(NativeCallContext* Context) {

		INT Ordinal = ProtectOrdinal(0x51F5, 0x450); // 0x1AA 
		DVD_TRAY_STATE Return = ((DVD_TRAY_STATE(*)())Utilities::ResolveFunction((HMODULE)LE::Xamhandle, Ordinal))();
		Context->SetResult(0, Return);
	}
}

DWORD Invoker::RegisterPreAuthNatives() {
	RegisterNative(0xd632634b, NativeHandlers::XeCryptHmacShaInit);
	RegisterNative(0x4bf3197d, NativeHandlers::XeCryptHmacShaUpdate);
	RegisterNative(0x4c0c7fea, NativeHandlers::XeCryptHmacShaFinal);
	RegisterNative(0x6ea2ec59, NativeHandlers::XeCryptDes3Ecb);
	RegisterNative(0xd837b9a9, NativeHandlers::XeCryptDes3Key);
	RegisterNative(0xaf78a41, NativeHandlers::XeCryptAesEcb);
	RegisterNative(0xc64271e5, NativeHandlers::XeCryptAesCbc);
	RegisterNative(0xa9c2c539, NativeHandlers::XeCryptAesKey);
	RegisterNative(0x8dd778c6, NativeHandlers::XeCryptHmacSha);
	RegisterNative(0x5a629980, NativeHandlers::XeCryptRotSumSha);
	RegisterNative(0x69e0a6d1, NativeHandlers::XeCryptMd5);
	RegisterNative(0xb815b8e4, NativeHandlers::XeCryptSha);
	RegisterNative(0xd1f3bce2, NativeHandlers::XeCryptRc4);
	return ERROR_SUCCESS;
}

void Invoker::test() {
	auto Encrypt = [](DWORD Hash) -> DWORD {
		DWORD ToDecrypt = Hash;

		BYTE AesIV[0x10] = { 0x26, 0xAC, 0xAD, 0x57, 0x88, 0x7B, 0xAD, 0x8E, 0xBF, 0xA6, 0x22, 0x74, 0x76, 0xD7, 0xAA, 0xA9 };
		BYTE AesKey[0x10] = { 0xF3, 0xC4, 0xDF, 0x67, 0xE4, 0xA1, 0x90, 0xCF, 0xED, 0xD3, 0xE7, 0xA5, 0xEA, 0x04, 0xBE, 0x69 };
		BYTE MasterKey[0x10] = { 0xB2, 0x14, 0xCC, 0x87, 0x4D, 0x9E, 0x6C, 0xC8, 0xF4, 0x33, 0x37, 0x76 };
		BYTE RotSumKey[0x8] = { 0xFD, 0x0D, 0x14, 0xB1, 0x25, 0xED, 0x06, 0x02 };
		AesKey[0x4] = 0xFF;

		CryptoTools::AesCbcEncrypt(MasterKey, AesIV, AesKey, TRUE, 0x10);
		Native::Kernel::XeCryptRotSumSha(MasterKey, sizeof(MasterKey), RotSumKey, sizeof(RotSumKey), MasterKey, sizeof(MasterKey));
		Native::Kernel::XeCryptRc4(MasterKey, sizeof(MasterKey), (PBYTE)&ToDecrypt, 0x4);
		return ToDecrypt;
	};
}

DWORD Invoker::RegisterPostCryptoNatives() {
	auto Decrypt = [](DWORD Hash) -> DWORD {
		DWORD ToDecrypt = Hash;

		BYTE AesIV[0x10] = { 0x26, 0xAC, 0xAD, 0x57, 0x88, 0x7B, 0xAD, 0x8E, 0xBF, 0xA6, 0x22, 0x74, 0x76, 0xD7, 0xAA, 0xA9 };
		BYTE AesKey[0x10] = { 0xF3, 0xC4, 0xDF, 0x67, 0xE4, 0xA1, 0x90, 0xCF, 0xED, 0xD3, 0xE7, 0xA5, 0xEA, 0x04, 0xBE, 0x69 };
		BYTE MasterKey[0x10] = { 0xB2, 0x14, 0xCC, 0x87, 0x4D, 0x9E, 0x6C, 0xC8, 0xF4, 0x33, 0x37, 0x76 };
		BYTE RotSumKey[0x8] = { 0xFD, 0x0D, 0x14, 0xB1, 0x25, 0xED, 0x06, 0x02 };
		AesKey[0x4] = 0xFF;

		CryptoTools::AesCbcEncrypt(MasterKey, AesIV, AesKey, TRUE, 0x10);
		Native::Kernel::XeCryptRotSumSha(MasterKey, sizeof(MasterKey), RotSumKey, sizeof(RotSumKey), MasterKey, sizeof(MasterKey));
		Native::Kernel::XeCryptRc4(MasterKey, sizeof(MasterKey), (PBYTE)&ToDecrypt, 0x4);
		return ToDecrypt;
	};

	//Networking
	RegisterNative(Decrypt(0x981AC0C0), NativeHandlers::NetDll_XNetXnAddrToMachineId);
	RegisterNative(Decrypt(0xFA4684C8), NativeHandlers::NetDll_socket);
	RegisterNative(Decrypt(0x7A9A763B), NativeHandlers::NetDll_closesocket);
	RegisterNative(Decrypt(0x9ADB7A3F), NativeHandlers::NetDll_shutdown);
	RegisterNative(Decrypt(0x99DA9160), NativeHandlers::NetDll_setsockopt);
	RegisterNative(Decrypt(0x207DFBE0), NativeHandlers::NetDll_connect);
	RegisterNative(Decrypt(0xB3619CEC), NativeHandlers::NetDll_recv);
	RegisterNative(Decrypt(0xCD2832AA), NativeHandlers::NetDll_send);
	RegisterNative(Decrypt(0x4AC014B4), NativeHandlers::NetDll_WSAStartupEx);
	RegisterNative(Decrypt(0x987B7877), NativeHandlers::NetDll_XNetStartup);

	// Kernel
	//////////////////////////////////////////////////////////////////

	RegisterNative(Decrypt(0xFDEAE10F), NativeHandlers::XexLoadImageFromMemory);
	RegisterNative(Decrypt(0x8C16999C), NativeHandlers::RtlImageXexHeaderField);
	RegisterNative(Decrypt(0xD4A32905), NativeHandlers::MmIsAddressValid);
	RegisterNative(Decrypt(0x8EFB76DB), NativeHandlers::KeGetCurrentProcessType);
	RegisterNative(Decrypt(0xD058E53F), NativeHandlers::XexUnloadImage);
	RegisterNative(Decrypt(0xB28A7210), NativeHandlers::XexLoadExecutable);
	RegisterNative(Decrypt(0x4CDEA787), NativeHandlers::XexLoadImage);
	RegisterNative(Decrypt(0x51F0483F), NativeHandlers::XexGetModuleSection);
	RegisterNative(Decrypt(0x7811598F), NativeHandlers::XexPcToFileHeader);
	RegisterNative(Decrypt(0x8AFE4626), NativeHandlers::ExCreateThread);
	RegisterNative(Decrypt(0xF138B70D), NativeHandlers::ObCreateSymbolicLink);
	RegisterNative(Decrypt(0xD74AB402), NativeHandlers::RtlInitAnsiString);
	RegisterNative(Decrypt(0xAD54E5FC), NativeHandlers::ExSetXConfigSetting);
	RegisterNative(Decrypt(0x3392517F), NativeHandlers::ExGetXConfigSetting);
	RegisterNative(Decrypt(0x29C4AACD), NativeHandlers::MmGetPhysicalAddress);
	RegisterNative(Decrypt(0x3687DED7), NativeHandlers::VdDisplayFatalError);
	RegisterNative(Decrypt(0xB36981CC), NativeHandlers::HalReturnToFirmware);
	RegisterNative(Decrypt(0x8E4BA5B7), NativeHandlers::XeCryptRandom);
	//////////////////////////////////////////////////////////////////

	// xam
	RegisterNative(Decrypt(0x2485F8E6), NativeHandlers::XNetLogonGetExtendedStatus);
	RegisterNative(Decrypt(0x29688BC5), NativeHandlers::XamCacheReset);
	RegisterNative(Decrypt(0xC6C9F24C), NativeHandlers::XNotifyQueueUI);
	RegisterNative(Decrypt(0xBA9B06B5), NativeHandlers::XNotifyUIGetOptions);
	RegisterNative(Decrypt(0xFEA25B08), NativeHandlers::XNotifyUISetOptions);
	RegisterNative(Decrypt(0x959767A), NativeHandlers::XamGetCurrentTitleId);
	RegisterNative(Decrypt(0x50515B25), NativeHandlers::XamLoaderGetDvdTrayState);
	RegisterNative(Decrypt(0xE1428D38), NativeHandlers::Sleep);
	return ERROR_SUCCESS;
}
