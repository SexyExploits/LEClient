#include "stdafx.h"

#pragma warning(push)
#pragma warning(disable:4826) 
DWORD HV::HvPeekPokeExpID = 0xD50F5D5A; // real value : 0x48565050
BYTE HV::FuseLines[0x10];

DWORD __declspec(naked) HV::HvxGetVersions(DWORD magic, DWORD mode, QWORD dest, QWORD src, DWORD len, QWORD arg_r8) {
	__asm {
		li      r0, 0 // HvxGetVersion
		sc
		blr
	}
}

static QWORD HvxExpansionInstall(DWORD PhysicalAddress, DWORD CodeSize) {
	__asm {
		li			r0, 0x72
		sc
		blr
	}
}

static QWORD HvxExpansionCall(DWORD ExpansionId, QWORD Param1 = 0, QWORD Param2 = 0, QWORD Param3 = 0, QWORD Param4 = 0) {
	__asm {
		li			r0, 0x73
		sc
		blr
	}
}

DWORD HV::SetupHvExpansion() {
	HvPeekPokeExpID ^= 0x9D590D0A;
	PVOID pPhysExp = XPhysicalAlloc(0x1000, MAXULONG_PTR, NULL, PAGE_READWRITE);
	DWORD physExpAdd = reinterpret_cast<DWORD>(Native::Kernel::MmGetPhysicalAddress(pPhysExp));

	ZeroMemory(pPhysExp, 0x1000);

	for (DWORD x = 0; x < sizeof(HvPeekPokeExp); x++)
		HvPeekPokeExp[x] ^= 0x95;
	memcpy(pPhysExp, HvPeekPokeExp, sizeof(HvPeekPokeExp));

	DWORD Result = static_cast<HRESULT>(HvxExpansionInstall(physExpAdd, 0x1000));
	XPhysicalFree(pPhysExp);
	return (SUCCEEDED(Result) ? ERROR_SUCCESS : E_FAIL);
}

BYTE HV::HvPeekBYTE(QWORD Address) { return static_cast<BYTE>(HvxExpansionCall(HvPeekPokeExpID, PEEK_BYTE, Address)); }

WORD HV::HvPeekWORD(QWORD Address) { return static_cast<WORD>(HvxExpansionCall(HvPeekPokeExpID, PEEK_WORD, Address)); }

DWORD HV::HvPeekDWORD(QWORD Address) { return static_cast<DWORD>(HvxExpansionCall(HvPeekPokeExpID, PEEK_DWORD, Address)); }

QWORD HV::HvPeekQWORD(QWORD Address) { return HvxExpansionCall(HvPeekPokeExpID, PEEK_QWORD, Address); }

DWORD HV::HvPeekBytes(QWORD Address, PVOID Buffer, DWORD Size) {
	PVOID Data = XPhysicalAlloc(Size, MAXULONG_PTR, NULL, PAGE_READWRITE);
	ZeroMemory(Data, Size);

	DWORD Result = static_cast<DWORD>(HvxExpansionCall(HvPeekPokeExpID, PEEK_BYTES, Address,reinterpret_cast<QWORD>(Native::Kernel::MmGetPhysicalAddress(Data)), Size));
	if (SUCCEEDED(Result))
		CopyMemory(Buffer, Data, Size);

	XPhysicalFree(Data);
	return Result;
}

DWORD HV::HvPokeBYTE(QWORD Address, BYTE Value) {	return static_cast<HRESULT>(HvxExpansionCall(HvPeekPokeExpID, POKE_BYTE, Address, Value)); }

DWORD HV::HvPokeWORD(QWORD Address, WORD Value) { return static_cast<HRESULT>(HvxExpansionCall(HvPeekPokeExpID, POKE_WORD, Address, Value)); }

DWORD HV::HvPokeDWORD(QWORD Address, DWORD Value) { return static_cast<HRESULT>(HvxExpansionCall(HvPeekPokeExpID, POKE_DWORD, Address, Value)); }

DWORD HV::HvPokeQWORD(QWORD Address, QWORD Value) { return static_cast<HRESULT>(HvxExpansionCall(HvPeekPokeExpID, POKE_QWORD, Address, Value)); }

DWORD HV::HvPokeBytes(QWORD Address, CONST PVOID Buffer, DWORD Size) {
	PVOID Data = XPhysicalAlloc(Size, MAXULONG_PTR, NULL, PAGE_READWRITE);
	memcpy(Data, Buffer, Size);

	DWORD Result = static_cast<HRESULT>(HvxExpansionCall(HvPeekPokeExpID, POKE_BYTES, Address,reinterpret_cast<QWORD>(Native::Kernel::MmGetPhysicalAddress(Data)), Size));
	XPhysicalFree(Data);
	return Result;
}

QWORD HV::HvGetFuseLine(BYTE FuseIndex) { return (FuseIndex > 11) ? NULL : HvPeekQWORD(0x8000020000020000 + (FuseIndex * 0x200)); }

