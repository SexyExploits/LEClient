#include "stdafx.h"
#pragma once

class FormatUtils {
public:

	static DWORD HexStringToByteArray(PBYTE pbArray, PCHAR pchHexString, DWORD dwLength);
	static INT toWCHAR(PCHAR input, PWCHAR output);
	static PWCHAR toWCHAR(PCHAR Text, ...);
	static PCHAR toCHAR(PWCHAR Text);
	static PWCHAR LinkWChar(PWCHAR Text, ...);
	static PCHAR LinkChar(CONST PCHAR Text, ...);
	static VOID ReplaceString(CONST PCHAR newtext, DWORD addr, INT length);
};

