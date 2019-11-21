#include "stdafx.h"

#ifdef DEBUG
CRITICAL_SECTION dbgLock;
BOOL dbgInit = FALSE;
VOID Utilities::LOG(CONST PCHAR strFormat, ...) {
	if (!Utilities::FileExists(LOGPath)) 
		Utilities::CWriteFile(LOGPath, NULL, NULL);

	if (dbgInit == FALSE) {
		InitializeCriticalSection(&dbgLock);
		dbgInit = TRUE;
	}

	CHAR buffer[0x3E8];

	va_list pArgList;
	va_start(pArgList, strFormat);
	vsprintf_s(buffer, 0x3E8, strFormat, pArgList);
	va_end(pArgList);

	char TimeBuff[20];
	struct tm *sTm;
	time_t now = time(NULL);
	sTm = gmtime(&now);

	strftime(TimeBuff, sizeof(TimeBuff), "%Y-%m-%d %H:%M:%S", sTm);

	EnterCriticalSection(&dbgLock);
	ofstream writeLog;
	writeLog.open(LOGPath, ofstream::app);
	if (writeLog.is_open()) {
		writeLog.write(TimeBuff, strlen(TimeBuff));
		writeLog.write(" ", 0x1);
		writeLog.write(buffer, strlen(buffer));
		writeLog.write("\r\n", 2);
	}
	writeLog.close();
	LeaveCriticalSection(&dbgLock);
}
#endif

string Utilities::GetModuleNameFromAddress(DWORD dwAddress) {
	auto ldr = reinterpret_cast<PLDR_DATA_TABLE_ENTRY>(GetModuleHandleA("xboxkrnl.exe"));
	auto CurrentEntry = ldr->InLoadOrderLinks.Flink;
	PLDR_DATA_TABLE_ENTRY Current = nullptr;

	char buffer[100];
	while (CurrentEntry != &ldr->InLoadOrderLinks && CurrentEntry != nullptr) {
		Current = CONTAINING_RECORD(CurrentEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
		if (!Current)
			break;

		if (dwAddress >= reinterpret_cast<DWORD>(Current->ImageBase)) {
			if (dwAddress <= reinterpret_cast<DWORD>(Current->ImageBase) + Current->SizeOfFullImage) {
				wcstombs(buffer, Current->BaseDllName.Buffer, sizeof(buffer));
				return std::string(buffer);
			}
		}
		CurrentEntry = CurrentEntry->Flink;
	}
	return nullptr;
}

CHAR m_hookSection[0x500];
INT m_hookCount;
DWORD Utilities::Resolve(HMODULE Module, INT ordinal) {
	auto ldr = reinterpret_cast<PLDR_DATA_TABLE_ENTRY>(Module);
	auto header = static_cast<_IMAGE_XEX_HEADER*>(ldr->XexHeaderBase);
	if (header) {
		auto sec = reinterpret_cast<PXEX_SECURITY_INFO>(header->SecurityInfo);
		if (sec) {
			PIMAGE_EXPORT_ADDRESS_TABLE exports = sec->ExportTableAddress;
			if (exports)
				return exports->ordOffset[ordinal - 1] + (exports->ImageBaseAddress << 16);
		}
	}
	return ERROR_SUCCESS;
}

VOID Utilities::Reverse(PBYTE pbData, DWORD cbData) {
	DWORD dwMidpoint = cbData >> 1;
	DWORD dwReverseCounter = 0;
	PBYTE pbPtr = pbData;

	if (dwMidpoint) {
		while (dwReverseCounter < dwMidpoint) {
			DWORD i = (cbData - dwReverseCounter) - 1 & 0xFFFFFFFF;

			BYTE b1 = *pbPtr;
			BYTE b2 = pbData[i];
			*pbPtr = b2;
			pbData[i] = b1;

			dwReverseCounter++;
			pbPtr++;
		}
	}
}

#pragma code_seg(push, r1, ".ptext")
DWORD Utilities::GetFunctionSize(PDWORD pdwAddress) {
	DWORD dwAddress = (DWORD)pdwAddress;
	DWORD dwSize = 0;

	while (*(PDWORD)dwAddress != 0x0000000 && *(PDWORD)dwAddress != 0x4E800020) {
		dwAddress += 4;
		dwSize += 4;
	}
	return dwSize > 0 ? (dwSize - 4) : 0;
} 
#pragma code_seg(pop, r1) 

BOOL Utilities::CompareData(PBYTE target1, PBYTE target2) {
	if (sizeof(target1) != sizeof(target2)) 
		return FALSE;

	for (int i = 0; i < sizeof(target1); i++) {
		if (target1[i] != target2[i]) 
			return FALSE;
	}
	return TRUE;
}

BOOL Utilities::IsNumberBetween(INT Max, INT Min, INT Num) {
	return ((Num < Max && !(Num <= Min)) || (Num > Max && !(Num >= Min)));
}

BOOL Utilities::IsBufferEmpty(PCHAR Buffer) {
	for (DWORD i = 0; i < sizeof(Buffer); i++)
	if (Buffer[i] != NULL)
		return FALSE;
	return TRUE;
}

BOOL Utilities::IsBufferEmpty(PBYTE Buffer) {
	for (DWORD i = 0; i < sizeof(Buffer); i++)
	if (Buffer[i] != NULL) 
		return FALSE;
	return TRUE;
}

DWORD Utilities::SetupResources() {

	/* Masterkey: 0x9B, 0xBC, 0x90, 0xA6, 0xAE, 0x90, 0xA1, 0x3D, 0xAC, 0x7F, 0x36, 0xC6, 0xE8, 0x0A, 0x8C, 0x02 */

	BYTE MasterKey[0x10];
	BYTE DesKey[0x10] = { 0x8B, 0xD3, 0xCC, 0x2D, 0x37, 0x5D, 0xD9, 0x75, 0xB0, 0x8C, 0xB2, 0x70, 0x05, 0xC6, 0xAA, 0x47 };
	BYTE AesKey[0x10] = { 0xCF, 0x65, 0x30, 0x9F, 0x07, 0xE4, 0x2F, 0xA6, 0xB7, 0x0C, 0x0A, 0x8C, 0x9B, 0x7D, 0x50, 0xF8 };
	BYTE AesIV[0x10] = { 0x94, 0xDA, 0xA3, 0xF5, 0xBB, 0x58, 0x6B, 0x58, 0x14, 0xAD, 0xFD, 0x68, 0x8C, 0x78, 0x2D, 0x7F };
	
	DWORD Aes_SboxKey = 0x90BC;
	DWORD Aes_SboxSz = 0x9;
	CryptoTools::CreateAesSbox(Aes_SboxKey, Aes_SboxSz);
	for (DWORD i = 0; i < sizeof(AesKey); i++) {
		AesKey[i] ^= Aes_SboxKey;
	}

	DWORD Des_SboxKey = 0x653;
	DWORD Des_SboxSz = 0x4;
	CryptoTools::CreateAesSbox(Des_SboxKey, Des_SboxSz);
	for (DWORD i = 0; i < sizeof(DesKey); i++) {
		DesKey[i] ^= Des_SboxKey;
	}

	CryptoTools::AesCbcEncrypt(AesKey, AesIV, AesKey, TRUE, sizeof(AesKey));
	CryptoTools::Des3Ecb(DesKey, DesKey, TRUE);
	CryptoTools::EncryptedMemcpy(AesKey, AesKey, 0x10);
	Native::Kernel::XeCryptSha((PBYTE)DesKey, sizeof(DesKey), AesKey, sizeof(AesKey), AesIV, sizeof(AesIV), MasterKey, 0x10);

	Native::Kernel::XeCryptRc4(MasterKey, sizeof(MasterKey), Xui::XuiData, sizeof(Xui::XuiData));
	//Utilities::CWriteFile(XEX_Path "MasterKey.bin", MasterKey, sizeof(MasterKey));
	return ERROR_SUCCESS;
}

VOID Utilities::SetSignInStrings() {
	BYTE SignInOrOutEdit[0xA] = { 0xD4, 0xE6, 0xEF, 0xE0, 0xEC, 0xEE, 0xE6, 0xA3, 0xD7, 0xEC };
	BYTE ChooseProfileEdit[0x13] = { 0xD9, 0xE3, 0xEE, 0xF9, 0xCE, 0xEF, 0xED, 0xE8, 0xEF, 0xE4, 0xD2, 0xE4, 0xF3, 0xF7, 0xE4, 0xF3, 0xA1, 0xA0, 0xA1 };

	for (DWORD x = 0; x < sizeof(SignInOrOutEdit); x++) 
		SignInOrOutEdit[x] ^= 0x83;
	for (DWORD x = 0; x < sizeof(ChooseProfileEdit); x++) 
		ChooseProfileEdit[x] ^= 0x81;

	strcpy((PCHAR)0xC248E010, (PCHAR)SignInOrOutEdit);
	strcpy((PCHAR)0xC248DFFC, (PCHAR)ChooseProfileEdit);
}

VOID Utilities::SendToDash(PWCHAR Text, ...) {
	XLaunchNewImage(nullptr, NULL);
	Utilities::XNotifyUI(Text);
}

PBYTE Utilities::GetHVCpukey() {
	static BYTE cpukey[0x10];

	HV::HvPeekBytes(0x20, cpukey, 0x10);
	return cpukey;
}

VOID Utilities::FreezeXbox() {
	HV::HvxGetVersions(0x72627472, 5, 0x12345678, 0x8000, 0x12345678, 0x12345678);
}

PBYTE Utilities::GetFuseCpukey() {
	BYTE cpukey[0x10];

	QWORD f1p1 = HV::HvGetFuseLine(3) | HV::HvGetFuseLine(4);
	QWORD f1p2 = HV::HvGetFuseLine(5) | HV::HvGetFuseLine(6);
	memcpy(cpukey, &f1p1, 0x08);
	memcpy(cpukey + 0x08, &f1p2, 0x08);
	return cpukey;
}

VOID Utilities::StartThread(LPTHREAD_START_ROUTINE lpStartAddress) {
	HANDLE handle = nullptr;
	DWORD lpThreadId = NULL;

	Native::Kernel::ExCreateThread(&handle, NULL, &lpThreadId, static_cast<PVOID>(XapiThreadStartup), lpStartAddress, nullptr, 0x2 | CREATE_SUSPENDED);
	XSetThreadProcessor(handle, 0x4);
	SetThreadPriority(handle, THREAD_PRIORITY_ABOVE_NORMAL);
	ResumeThread(handle);
}

VOID __declspec(naked) GLPR(VOID) {
	__asm {
		std     r14, -0x98(sp)
		std     r15, -0x90(sp)
		std     r16, -0x88(sp)
		std     r17, -0x80(sp)
		std     r18, -0x78(sp)
		std     r19, -0x70(sp)
		std     r20, -0x68(sp)
		std     r21, -0x60(sp)
		std     r22, -0x58(sp)
		std     r23, -0x50(sp)
		std     r24, -0x48(sp)
		std     r25, -0x40(sp)
		std     r26, -0x38(sp)
		std     r27, -0x30(sp)
		std     r28, -0x28(sp)
		std     r29, -0x20(sp)
		std     r30, -0x18(sp)
		std     r31, -0x10(sp)
		stw     r12, -0x8(sp)
		blr
	}
}

DWORD RelinkGPLR(DWORD SFSOffset, PDWORD SaveStubAddress, PDWORD OriginalAddress) {
	DWORD Instruction = NULL, Replacing;
	PDWORD Saver = (PDWORD)GLPR;

	if (SFSOffset & 0x2000000) 
		SFSOffset |= 0xFC000000;

	Replacing = OriginalAddress[SFSOffset / 0x04];
	for (DWORD i = NULL; i < 0x14; i++) {
		if (Replacing == Saver[i]) {
			DWORD NewAddress = (DWORD)&Saver[i] - (DWORD)SaveStubAddress;
			Instruction = (0x48000001 | (NewAddress & 0x03FFFFFC));
		}
	} 
	return Instruction;
}

VOID Utilities::HookFunctionStart(PDWORD Address, PDWORD SaveStub, DWORD Destination) {
	if ((Address != NULL) && (SaveStub != NULL)) {
		DWORD Relocation = (DWORD)(&Address[0x04]);
		SaveStub[0x00] = (0x3D600000 + ((Relocation >> 0x10) & 0xFFFF));
		SaveStub[0x01] = (0x616B0000 + (Relocation & 0xFFFF));
		SaveStub[0x02] = 0x7D6903A6;

		for (DWORD i = 0; i < 0x04; i++) {
			if ((Address[i] & 0x48000003) == 0x48000001) 
				SaveStub[i + 0x03] = RelinkGPLR((Address[i] & ~0x48000003), &SaveStub[i + 0x03], &Address[i]);
			else SaveStub[i + 0x03] = Address[i];
		}

		SaveStub[0x07] = 0x4E800420;
		__dcbst(NULL, SaveStub);
		__sync();
		__isync();
		Utilities::PatchInJump(Address, Destination, FALSE);
	}
}

DWORD Utilities::HookFunctionStub(DWORD _Address, PVOID Function) {
	PDWORD startStub = (PDWORD)&m_hookSection[m_hookCount * 32];
	m_hookCount++;

	for (INT i = 0; i < 7; i++)
		startStub[i] = 0x60000000;
	startStub[7] = 0x4E800020;

	HookFunctionStart((PDWORD)_Address, startStub, (DWORD)Function);
	return (DWORD)startStub;
}

DWORD Utilities::HookFunctionStub(PCHAR ModuleName, DWORD Ordinal, PVOID Destination) {
	return Utilities::HookFunctionStub((DWORD)Utilities::ResolveFunction(ModuleName, Ordinal), Destination);
}

VOID Utilities::ApplyPatchData(PVOID buffer) {

	DWORD PatchCount = NULL;
	PDWORD PatchData = (PDWORD)buffer;

	while (*PatchData != 0xFFFFFFFF) {
		memcpy((PVOID)PatchData[0], &PatchData[2], PatchData[1] * sizeof(DWORD));
		PatchData += (PatchData[1] + 2);
		PatchCount++;
	}
}

VOID Utilities::PatchInJump(PDWORD Address, DWORD Destination, BOOL Linked) {
	Address[0] = 0x3D600000 + ((Destination >> 16) & 0xFFFF);
	if (Destination & 0x8000) 
		Address[0] += 1;
	Address[1] = 0x396B0000 + (Destination & 0xFFFF);
	Address[2] = 0x7D6903A6;
	Address[3] = Linked ? 0x4E800421 : 0x4E800420;
}

DWORD Utilities::PatchModuleImport(PCHAR Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress) {
	LDR_DATA_TABLE_ENTRY* moduleHandle = (LDR_DATA_TABLE_ENTRY*)GetModuleHandle(Module);
	return (moduleHandle == NULL) ? S_FALSE : PatchModuleImport(moduleHandle, ImportedModuleName, Ordinal, PatchAddress);
}

DWORD Utilities::PatchModuleImport(PLDR_DATA_TABLE_ENTRY Module, PCHAR ImportedModuleName, DWORD Ordinal, DWORD PatchAddress) {
	DWORD Address = (DWORD)Utilities::ResolveFunction(ImportedModuleName, Ordinal);
	if (Address == NULL)
		return S_FALSE;

	PXEX_IMPORT_DESCRIPTOR importDesc = (PXEX_IMPORT_DESCRIPTOR)RtlImageXexHeaderField(Module->XexHeaderBase, 0x000103FF);
	if (importDesc == NULL) 
		return S_FALSE;

	DWORD Result = 2;
	PCHAR stringTable = (PCHAR)(importDesc + 1);
	XEX_IMPORT_TABLE_ORG* importTable = (XEX_IMPORT_TABLE_ORG*)(stringTable + importDesc->NameTableSize);

	for (DWORD x = 0; x < importDesc->ModuleCount; x++) {
		PDWORD importAdd = (PDWORD)(importTable + 1);
		for (DWORD y = 0; y < importTable->ImportTable.ImportCount; y++) {

			if (*(PDWORD)importAdd[y] == Address) {
				CopyMemory((PDWORD)importAdd[y], &PatchAddress, 0x4);
				DWORD newCode[0x4];
				PatchInJump(newCode, PatchAddress, FALSE);
				CopyMemory((PDWORD)importAdd[y + 1], newCode, 0x10);
				Result = S_OK;
			}
		} 
		importTable = (XEX_IMPORT_TABLE_ORG*)(((PBYTE)importTable) + importTable->TableSize);
	}
	return Result;
}

HRESULT Utilities::MountPath(std::string &device, std::string &link, BOOL System) {
	STRING deviceName;
	STRING linkName;

	Native::Kernel::RtlInitAnsiString(&deviceName, device.c_str());
	Native::Kernel::RtlInitAnsiString(&linkName, link.c_str());
	return (SUCCEEDED(Native::Kernel::ObCreateSymbolicLink(&linkName, &deviceName, System))) ? ERROR_SUCCESS : E_FAIL;
}

BOOL Utilities::CReadFile(CONST PCHAR FileName, PBYTE* pBuffer, PDWORD pdwLength) {
	HANDLE hFile; DWORD dwNumberOfBytesRead;
	hFile = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return FALSE;

	*pdwLength = GetFileSize(hFile, NULL);
	*pBuffer = (PBYTE)malloc(*pdwLength);
	if (*pBuffer == NULL) {
		CloseHandle(hFile);
		return FALSE;
	}

	if (!(ReadFile(hFile, *pBuffer, *pdwLength, &dwNumberOfBytesRead, NULL)) || (dwNumberOfBytesRead != *pdwLength)) {
		free(pBuffer);
		CloseHandle(hFile);
		return FALSE;
	}

	CloseHandle(hFile);
	return TRUE;
}

BOOL Utilities::CWriteFile(CONST PCHAR FilePath, CONST PVOID Data, DWORD Size) {
	HANDLE fHandle = CreateFile(FilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fHandle == INVALID_HANDLE_VALUE)
		return FALSE;

	if (WriteFile(fHandle, Data, Size, &Size, NULL) != TRUE)
		return FALSE;

	CloseHandle(fHandle);
	return TRUE;
}

BOOL Utilities::FileExists(LPCSTR FileName) {
	if (GetFileAttributes(FileName) == -1) {
		DWORD lastError = GetLastError();
		if (lastError == ERROR_FILE_NOT_FOUND || lastError == ERROR_PATH_NOT_FOUND)
			return FALSE;
	}
	return TRUE;
}

BOOL pfShow = (BOOL)0xDEADBEEF;
BOOL pfShowMovie;
BOOL pfPlaySound;
BOOL pfShowIPTV;
VOID ToggleNotify(BOOL Status) {

	if ((INT)pfShow == 0xDEADBEEF)  Native::Xam::XNotifyUIGetOptions(&pfShow, &pfShowMovie, &pfPlaySound, &pfShowIPTV);
	if (!Status) { Native::Xam::XNotifyUISetOptions(pfShow, pfShowMovie, pfPlaySound, pfShowIPTV); }
	else { Native::Xam::XNotifyUISetOptions(TRUE, TRUE, TRUE, TRUE); }
	Native::Xam::Sleep(500);
}

VOID NotifyDoQueueUI(PWCHAR pwszStringParam) {
	ToggleNotify(TRUE);
	Native::Xam::XNotifyQueueUI(pwszStringParam);
}

VOID Utilities::XNotifyUI(PWCHAR Text) {
	if ((UCHAR)Native::Kernel::KeGetCurrentProcessType() != PROC_USER) {
		HANDLE th = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)NotifyDoQueueUI, (LPVOID)Text, CREATE_SUSPENDED, NULL);
		(th != NULL) ? ResumeThread(th) : NULL;
	}
	else 
		NotifyDoQueueUI(Text);
}

VOID Utilities::XNotifyUI(PCHAR Text, ...) {
	CHAR Buffer[0x100];
	PWCHAR Buffer2 = new WCHAR[0x100];

	va_list pArgList;
	va_start(pArgList, Text);
	vsprintf(Buffer, Text, pArgList);
	va_end(pArgList);

	mbstowcs(Buffer2, Buffer, strlen(Buffer) + 1);
	XNotifyUI(Buffer2);
}

INT Utilities::RandomInRange(INT Min, INT Max) {
	INT n = Min - Max + 1;
	INT remainder = RAND_MAX % n;
	INT x;
	do { 
		x = rand(); 
	} while (x >= RAND_MAX - remainder);
	return Min + x % n;
}

FARPROC Utilities::ResolveFunction(HMODULE mHandle, DWORD Ordinal) {
	return (mHandle == NULL) ? NULL : GetProcAddress(mHandle, (LPCSTR)Ordinal);
}

FARPROC Utilities::ResolveFunction(PCHAR ModuleName, DWORD Ordinal) {
	HMODULE mHandle = GetModuleHandle(ModuleName);
	return (mHandle == NULL) ? NULL : GetProcAddress(mHandle, (LPCSTR)Ordinal);
}

BOOL Utilities::GetSectionInfo(PDWORD pdwAddress, PDWORD pdwLength, CONST PCHAR pchSectionName) {
	DWORD SectionInfoOffset = BASEADDY;
	while (!strcmp(FormatUtils::toCHAR(L".rdata"), (PCHAR)SectionInfoOffset) == FALSE) 
		SectionInfoOffset += 0x4;

	PIMAGE_SECTION_HEADER DefaultSections = (PIMAGE_SECTION_HEADER)SectionInfoOffset;
	for (DWORD i = 0; strlen((PCHAR)DefaultSections[i].Name); i++) {
		if (!strcmp(pchSectionName, (PCHAR)DefaultSections[i].Name) == TRUE) {
			*pdwAddress = BASEADDY + _byteswap_ulong(DefaultSections[i].VirtualAddress);
			*pdwLength = _byteswap_ulong(DefaultSections[i].Misc.VirtualSize);
			return TRUE;
		}
	}
	return FALSE;
}

DWORD Utilities::SetLiveBlock(BOOL Enabled) {
	if (Enabled) {
	    PWCHAR nullStr = L"NO.%sNO.NO\0";
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x816021C4 : 0x815FF1F4), FormatUtils::toCHAR(nullStr), 0x14); //siflc
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x816021DC : 0x815FF20C), FormatUtils::toCHAR(nullStr), 0x14); //piflc
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x816021F4 : 0x815FF224), FormatUtils::toCHAR(nullStr), 0x11); //notice
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x81602208 : 0x815FF238), FormatUtils::toCHAR(nullStr), 0x14); //xexds
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x81602238 : 0x815FF250), FormatUtils::toCHAR(nullStr), 0x13); //xeas
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x81602220 : 0x815FF268), FormatUtils::toCHAR(nullStr), 0x14); //xetgs
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x8160224C : 0x815FF27C), FormatUtils::toCHAR(nullStr), 0x15); //xemacs
	}
	else {
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x816021C4 : 0x815FF1F4), FormatUtils::toCHAR(L"SIFLC.%sXBOXLIVE.COM"), 0x14); //siflc
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x816021DC : 0x815FF20C), FormatUtils::toCHAR(L"PIFLC.%sXBOXLIVE.COM"), 0x14); //piflc
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x816021F4 : 0x815FF224), FormatUtils::toCHAR(L"NOTICE.%sXBOX.COM"), 0x11); //notice
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x81602208 : 0x815FF238), FormatUtils::toCHAR(L"XEXDS.%sXBOXLIVE.COM"), 0x14); //xexds
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x81602220 : 0x815FF250), FormatUtils::toCHAR(L"XETGS.%sXBOXLIVE.COM"), 0x14); //xetgs
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x81602238 : 0x815FF268), FormatUtils::toCHAR(L"XEAS.%sXBOXLIVE.COM"), 0x13); //xeas
		CopyMemory(reinterpret_cast<PBYTE>(KV::IsDevkit ? 0x8160224C : 0x815FF27C), FormatUtils::toCHAR(L"XEMACS.%sXBOXLIVE.COM"), 0x15); //xemacs
	} 
	return ERROR_SUCCESS;
}
