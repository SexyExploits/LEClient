#pragma once
#include "stdafx.h"


class CryptoTools {
public:

	static PVOID EncryptedMemcpy(PBYTE Dest, PBYTE Src, INT Length);
	static VOID CreateAesSbox(DWORD r3, DWORD r4);
	static VOID AesCbcEncrypt(PBYTE pbAes_input, PBYTE pbIV, PBYTE pbAes_Key, BOOL bProtect, DWORD dwSize);
	static VOID Des3Ecb(PBYTE pbDes3_input, PBYTE pbDes3_Key, BOOL bProtect);
};
