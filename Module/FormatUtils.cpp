#include "StdAfx.h"

DWORD FormatUtils::HexStringToByteArray(PBYTE pbArray, PCHAR pchHexString, DWORD dwLength) {
	PCHAR pos = pchHexString; 
	PCHAR endptr = nullptr; 
	size_t count = NULL;

	if ((pchHexString[0] == '\0') || (strlen(pchHexString) % 2)) 
		return -1;

	PBYTE Data = static_cast<PBYTE>(malloc(dwLength + 1));
	ZeroMemory(Data, dwLength + 1);
	for (count = 0; count < dwLength; count++) {
		CHAR buf[5] = { '0', 'x', pos[0], pos[1], 0 };
		Data[count] = static_cast<BYTE>(strtol(buf, &endptr, 0)); 
		pos += 2;
		if (endptr[0] != '\0') {
			free(Data);
			return -1;
		}
	} 
	memcpy(pbArray, Data, dwLength);
	free(Data);
	return ERROR_SUCCESS;
}

INT FormatUtils::toWCHAR(PCHAR input, PWCHAR output) {
	if (!input || !output) 
		return ERROR_SUCCESS;

	INT Len = strlen(input);
	memset(output, NULL, (Len * 2) + 2);
	for (auto i = 1, b = 0; b < Len; i += 2) {
		reinterpret_cast<PCHAR>(output)[i] = input[b];
		b++;
	}
	return Len;
}

PWCHAR FormatUtils::toWCHAR(PCHAR Text, ...) {
	static CHAR Buffer[0x1000];
	static CHAR MessageBuffer[0x100];
	static WCHAR Message[0x100];

	va_list pArgList;
	va_start(pArgList, Text);
	vsprintf(Buffer, Text, pArgList);
	va_end(pArgList);

	sprintf(MessageBuffer, Buffer);
	mbstowcs(Message, MessageBuffer, strlen(MessageBuffer) + 1);

	ZeroMemory(Buffer, sizeof(Buffer));
	ZeroMemory(MessageBuffer, sizeof(MessageBuffer));
	return Message;
}

PCHAR FormatUtils::toCHAR(PWCHAR Text) {
	static CHAR Buffer[0x1000];
	wcstombs(Buffer, Text, wcslen(Text) + 1);
	return Buffer;
}

PWCHAR FormatUtils::LinkWChar(PWCHAR Text, ...) {
	static WCHAR Buffer[0x1000], MessageBuffer[0x100];;

	va_list pArgList;
	va_start(pArgList, Text);
	vswprintf(Buffer, Text, pArgList);
	va_end(pArgList);

	swprintf(MessageBuffer, Buffer);
	return MessageBuffer;
}

PCHAR FormatUtils::LinkChar(CONST PCHAR Text, ...) {
	static CHAR Buffer[0x1000];

	va_list pArgList;
	va_start(pArgList, Text);
	vsprintf(Buffer, Text, pArgList);
	va_end(pArgList);
	return Buffer;
}

VOID FormatUtils::ReplaceString(CONST PCHAR newtext, DWORD addr, INT length) {
	PCHAR temp = new CHAR[length + 1];
	for (auto i = 0; i < length; i++)
		temp[i] = ' ';

	temp[length] = '\0';

	for (auto i = 0; i < strlen(newtext); i++) 
		temp[i] = newtext[i];

	memcpy(reinterpret_cast<LPVOID>(addr), temp, length + 1);
	delete temp;
}

