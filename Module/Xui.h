#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <xuiapp.h>

typedef VOID(*XAMBUILDRESOURCELOCATOR)(HANDLE LEhandle, PWCHAR wModuleName, PWCHAR CONST cdModule, PWCHAR pBuffer, ULONG ddSize);

class Xui {
public:
	static struct XUICustomization {
		DWORD Primary;
		DWORD Secondary;
	};
	
	static XUICustomization* XuiColors;

	static BYTE XuiData[208062];

	static INT TimeTicky;
	static VOID VerifyToken();
};