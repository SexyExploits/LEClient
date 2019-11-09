#include "stdafx.h"

From::Time Requests::m_Time;
BYTE Requests::m_TimeKvFirstUnbannedTimeStamp[0x16];
BYTE Requests::m_rgSessionKey[0x10];
BOOL Requests::m_Initalized = FALSE;
DWORD Requests::TimeRespTick = NULL;

DWORD Requests::Auth() {
	auto Request = static_cast<To::Auth*>(malloc(sizeof(To::Auth)));

	CopyMemory(Request->FuseCpukey, HV::FuseLines, 0x10);
	CopyMemory(Request->KVSerial, &KV::KeyVault.ConsoleSerialNumber, 0xC);
	Request->ConsoleType = CONSOLE_TYPE_FROM_FLAGS;

	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_AUTH, Request, sizeof(To::Auth), NULL, NULL, TRUE, FALSE))) {
		Networking::Recv(m_rgSessionKey, 0x10);
		Networking::Disconnect();
		free(Request);
		return ERROR_SUCCESS;
	}
	Networking::Disconnect();
	free(Request);
#ifdef DEBUG
	DebugPrint("[LiveEmulation] - ath 1 error");
#endif
	return E_FAIL;
}

BOOL bDisplayMsg;
VOID DisplayUpdateMessage() {
	PCWSTR UpdateButtons[2] = { L"Restart me now", L"Restart me later" };
	MESSAGEBOX_RESULT Result;
	XOVERLAPPED OverLapped;

	ZeroMemory(&Result, sizeof(Result));
	ZeroMemory(&OverLapped, sizeof(OverLapped));

	XShowMessageBoxUI(XUSER_INDEX_ANY, L"LiveEmulation Client Update", L"You may choose whether or not to restart to be on the new update now or later.\nIf you wait your client will be automatically updated upon next reboot!", ARRAYSIZE(UpdateButtons), UpdateButtons, 0, XMB_ALERTICON, &Result, &OverLapped);
	while (!XHasOverlappedIoCompleted(&OverLapped)) 
		Native::Xam::Sleep(500);

	if (Result.dwButtonPressed == 0) 
		Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);
	if (Result.dwButtonPressed == 1) 
		Utilities::XNotifyUI(L"LiveEmulation - Your Client will be automatically updated upon next reboot.");
	 bDisplayMsg = TRUE;
}

char DiscordToken;
void DiscordVerification() {
	PCWSTR DiscordButtons[1] = { L"Close" };
	MESSAGEBOX_RESULT Result;
	XOVERLAPPED OverLapped;

	ZeroMemory(&Result, sizeof(Result));
	ZeroMemory(&OverLapped, sizeof(OverLapped));

	CHAR MessageContent[0x100];
	WCHAR WMessageContent[0x100];

	sprintf(MessageContent, "Your discord verification token is: %s", &DiscordToken);
	FormatUtils::toWCHAR(MessageContent, WMessageContent);

	XShowMessageBoxUI(XUSER_INDEX_ANY, L"LiveEmulation | Discord Verification Token", WMessageContent, ARRAYSIZE(DiscordButtons), DiscordButtons, 0, XMB_ALERTICON, &Result, &OverLapped);
	while (!XHasOverlappedIoCompleted(&OverLapped))
		Sleep(500);
}

DWORD Requests::UpdateClient(DWORD DwModuleSize) {
	Utilities::SetLiveBlock(TRUE);

	PBYTE ModuleBuffer = static_cast<PBYTE>(XPhysicalAlloc(DwModuleSize, MAXULONG_PTR, NULL, PAGE_READWRITE));
	if (Networking::Recv(ModuleBuffer, DwModuleSize) != ERROR_SUCCESS) 
		return E_FAIL;

	if (!Utilities::CWriteFile(XEX_Path, ModuleBuffer, DwModuleSize)) 
		return E_FAIL;

	if (Utilities::FileExists(INI_Path)) 
		remove(INI_Path);

	if (!bDisplayMsg) 
		Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(DisplayUpdateMessage));

	Networking::Disconnect();
	XPhysicalFree(ModuleBuffer);
	Utilities::SetLiveBlock(FALSE);
	return ERROR_SUCCESS;
}

DWORD Requests::Status() {
	auto Request = static_cast<To::Status*>(malloc(sizeof(To::Status)));
	auto Response = static_cast<From::Status*>(malloc(sizeof(From::Status)));

	DWORD XexBufferSize = NULL;
	PBYTE XexBuffer = nullptr;
	for (auto i = 0; i < 3; i++) {
		if (FAILED(Utilities::CReadFile(XEX_Path, &XexBuffer, &XexBufferSize))) {
			Native::Xam::Sleep(500);
			if (i >= 3) {
#ifdef DEBUG
				DebugPrint("Failed to read xex file!");
#endif
				return E_FAIL;
			}
			continue;
		}
		break;
	}

	 if (XexBuffer == nullptr) {
#ifdef DEBUG
		 DebugPrint("xex buffer was null!");
#endif
		return E_FAIL;
	 }

	CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);
	Native::Kernel::XeCryptHmacSha(m_rgSessionKey, 0x10, XexBuffer, XexBufferSize, Request->ExecutableHash, 0x10);
	free(XexBuffer);

	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_STATUS, Request, sizeof(To::Status), Response, sizeof(From::Status), TRUE, TRUE))) {
		if (Response->DwStatus == UPDATE) 
				UpdateClient(Response->DwXexSize);
		else if (Response->DwStatus != SUCCESS) {
#ifdef DEBUG
			DebugPrint("Status resp Failure!");
#endif
			return E_FAIL;
		}

		Networking::Disconnect();
		free(Request);
		free(Response);
		return ERROR_SUCCESS;
	}
	else {
#ifdef DEBUG
#endif
	}
	Networking::Disconnect();
	free(Request);
	free(Response);
#ifdef DEBUG
	DebugPrint("Status Failure!");
#endif
	return E_FAIL;
}

VOID Requests::PresenseThread() {
	if (!SUCCEEDED(Status())) {
#ifdef DEBUG
		DebugPrint("ServerCheckStatus Failed!");
#endif
		Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);
	}

	if (!SUCCEEDED(Security())) {
#ifdef DEBUG
		DebugPrint("ServerCheckSecurity Failed!");
#endif
		Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);
	}

	if (!SUCCEEDED(UpdateTime())) {
#ifdef DEBUG
		DebugPrint("ServerCheckTime Failed!");
#endif
		Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);
	}

	auto Request = static_cast<To::Presence*>(malloc(sizeof(To::Presence)));
	auto Response = static_cast<From::Presence*>(malloc(sizeof(From::Presence)));
	CopyMemory(Request->SessionKey, m_rgSessionKey, 0x10);
	Request->TitleId = Native::Xam::XamGetCurrentTitleId();

	BYTE Unknown[0x10] = { 0x55, 0x6E, 0x6B, 0x6E, 0x6F, 0x77, 0x6E, 0x00, 0x00, 0x00 };
	XUSER_SIGNIN_INFO userInfo;
	ZeroMemory(&userInfo, sizeof(XUSER_SIGNIN_INFO));
	(XUserGetSigninInfo(NULL, XUSER_GET_SIGNIN_INFO_ONLINE_XUID_ONLY, &userInfo) == ERROR_SUCCESS) ? memcpy(Request->GamerTag, userInfo.szUserName, 0x10) : memcpy(Request->GamerTag, Unknown, 0x10);

	DWORD XnetStatus = NULL;
	Native::Xam::XNetLogonGetExtendedStatus(&XnetStatus, &Request->LiveStatus);
	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_PRES, Request, sizeof(To::Presence), Response, sizeof(From::Presence), FALSE, TRUE))) {
		
		if (Response->DiscordPopup == 1) {
			strcpy(&DiscordToken, Response->DiscordToken);
			Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(DiscordVerification));
		}

		if (Response->DwStatus != SUCCESS) {
#ifdef DEBUG
			DebugPrint("Pres Status Error!");
#endif
		}

		if (Response->ConsoleAction == REBOOT) {
#ifdef DEBUG
			DebugPrint("ConsoleAction reboot");
#endif
			Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);
		}
		else if (Response->ConsoleAction == RROD) {
#ifdef DEBUG
			DebugPrint("ConsoleAction RROD");
#endif
			Native::Kernel::VdDisplayFatalError();
		}
		else if (Response->ConsoleAction == SENDTODASH)
			Utilities::SendToDash(L"LiveEmulation - You've been sent to dash by an Admin!");
	}
	else 
		Native::Kernel::HalReturnToFirmware(HalFatalErrorRebootRoutine);
	free(Request);
	free(Response);
	Native::Xam::Sleep(Utilities::RandomInRange(35000, 65000));
}


DWORD Requests::VerifyToken(PCHAR Token) {
	auto Request = static_cast<To::Token*>(malloc(sizeof(To::Token)));
	auto Response = static_cast<From::TokenCheck*>(malloc(sizeof(From::TokenCheck)));

	CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);
	strncpy(Request->token, Token, 0xC);

	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_TOKEN_CHECK, Request, sizeof(To::Token), Response, sizeof(From::TokenCheck), FALSE, TRUE))) {
		if (Response->DwStatus != SUCCESS) 
			return E_FAIL;

		PCWSTR RedeemButtons[2] = { L"Confirm", L"Cancel" };
		PCWSTR ErrorsButtons[2] = { L"Try Again", L"Cancel" };
		MESSAGEBOX_RESULT Result;
		XOVERLAPPED OverLapped;

		ZeroMemory(&Result, sizeof(Result));
		ZeroMemory(&OverLapped, sizeof(OverLapped));

		CHAR MessageContent[0x100];
		WCHAR WMessageContent[0x100];
		if (Response->TokenStatus == NULL) {
			sprintf(MessageContent, "Press Confirm to redeem %i Day(s) or press cancel to redeem at a later time!", Response->TokenValue);
			FormatUtils::toWCHAR(MessageContent, WMessageContent);

			XShowMessageBoxUI(XUSER_INDEX_ANY, L"LiveEmulation | Confirm Redeem", WMessageContent, ARRAYSIZE(RedeemButtons), RedeemButtons, 0, XMB_ALERTICON, &Result, &OverLapped);
			while (!XHasOverlappedIoCompleted(&OverLapped)) 
				Sleep(500);

			if (Result.dwButtonPressed == NULL)
				RedeemToken(Token);

		} else {
			sprintf(MessageContent, "Error, the token was either already redeemed or doesn't exist.\nYou can try again or cancel.");
			FormatUtils::toWCHAR(MessageContent, WMessageContent);

			XShowMessageBoxUI(XUSER_INDEX_ANY, L"LiveEmulation | Redeem Error", WMessageContent, ARRAYSIZE(ErrorsButtons), ErrorsButtons, 0, XMB_ERRORICON, &Result, &OverLapped);
			while (!XHasOverlappedIoCompleted(&OverLapped)) 
				Sleep(500);
			if (Result.dwButtonPressed == NULL)
				Xui::VerifyToken();
		}
		free(Request);
		free(Response);
		return ERROR_SUCCESS;
	}
	free(Request);
	free(Response);
	return E_FAIL;
}

DWORD Requests::RedeemToken(PCHAR Token) {
	auto Request = static_cast<To::Token*>(malloc(sizeof(To::Token)));
	auto Response = static_cast<From::TokenRedeem*>(malloc(sizeof(From::TokenRedeem)));

	CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);
	strncpy(Request->token, Token, 0xC);

	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_TOKEN_REDEEM, Request, sizeof(To::Token), Response, sizeof(From::TokenRedeem), FALSE, TRUE))) {
		if (Response->DwStatus != SUCCESS) 
			return E_FAIL;

		Utilities::XNotifyUI(Response->TokenMessage);
	    UpdateTime();
		free(Request);
		free(Response);
		return ERROR_SUCCESS;
	}
	free(Request);
	free(Response);
	return E_FAIL;
}

DWORD Requests::Security() {
	auto Request = static_cast<To::Security*>(malloc(sizeof(To::Security)));
	auto Response = static_cast<From::Security*>(malloc(sizeof(From::Security)));

	CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);
	CopyMemory(Request->KV_CpuKey, KV::CpuKey, 0x10);
	CopyMemory(Request->HV_CpuKey, Utilities::GetHVCpukey(), 0x10);
	CopyMemory(Request->Fuse_CpuKey, HV::FuseLines, 0x10);

	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_SECURITY, Request, sizeof(To::Security), Response, sizeof(From::Security), FALSE, TRUE))) {
		if (Response->DwStatus != SUCCESS) 
			return E_FAIL;

		free(Request);
		free(Response);
		return ERROR_SUCCESS;
	}
	free(Request);
	free(Response);
#ifdef DEBUG
	DebugPrint("sec Failure!");
#endif
	return E_FAIL;
}

DWORD Requests::UpdateTime() {
	auto Request = static_cast<To::Time*>(malloc(sizeof(To::Time)));
	CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);

	if (SUCCEEDED(Networking::Send(PACKET_COMMAND_TIME, Request, sizeof(To::Time), &m_Time, sizeof(From::Time), TRUE, TRUE))) {
		if (m_Time.DwStatus != SUCCESS) 
			return E_FAIL;

		DWORD TimeStampSize = NULL;
		Networking::Recv(&TimeStampSize, 0x4);
		Networking::Recv(m_TimeKvFirstUnbannedTimeStamp, TimeStampSize);

		TimeRespTick = GetTickCount();
		LE::Auth_status = m_Time.Status;
		Utilities::SetLiveBlock(LE::Auth_status < AUTHED);
		free(Request);
		Networking::Disconnect();
		return ERROR_SUCCESS;
	}
	free(Request);
#ifdef DEBUG
	DebugPrint("Updt Error!");
#endif
	return E_FAIL;
}

DWORD Requests::Patches(DWORD DwTitleId) {
	if (DwTitleId == 0x81000000 && !KV::IsDevkit) {
		*reinterpret_cast<PDWORD>(0x81A3BEC0) = 0x38600001; // Gold Spoofing
		*reinterpret_cast<PDWORD>(0x816825F4) = 0x60000000; // EvaulateContent
		*reinterpret_cast<PDWORD>(0x8167FA28) = 0x38600000; // ContentEvaluateLicense
		*reinterpret_cast<PDWORD>(0x8167999C) = 0x60000000; // MmGetPhysicalAddress
		*reinterpret_cast<PDWORD>(0x8167C564) = 0x38600000; // XeKeysVerifyRSASignature
		return ERROR_SUCCESS;
	}

	auto Request = static_cast<To::Patches*>(malloc(sizeof(To::Patches)));
	auto Response = static_cast<From::Patches*>(malloc(sizeof(From::Patches)));

	CopyMemory(Request->SessionToken, m_rgSessionKey, 0x10);
	Request->TitleId = DwTitleId;

	if (m_Time.Status >= AUTHED) {
		if (SUCCEEDED(Networking::Send(PACKET_COMMAND_PATCHES, Request, sizeof(To::Patches), Response, sizeof(From::Patches), TRUE, TRUE))) {
			if (Response->DwStatus != SUCCESS) 
				return E_FAIL;

			PBYTE Patches = static_cast<PBYTE>(XPhysicalAlloc(Response->DwPatchSize, MAXULONG_PTR, NULL, PAGE_READWRITE));
			if (Patches == nullptr || FAILED(Networking::Recv(Patches, Response->DwPatchSize)))
				return E_FAIL;
			Networking::Disconnect();

			if (DwTitleId == 0x46B) {
				Xui::XuiColors->Primary = *reinterpret_cast<PDWORD>(Patches);
				Xui::XuiColors->Secondary = *reinterpret_cast<PDWORD>(Patches + 0x4);
				DebugPrint("Primary: 0x%x", Xui::XuiColors->Primary);
			} else 
				Utilities::ApplyPatchData(Patches);
			
			free(Request);
			free(Response);
			XPhysicalFree(Patches);
			return ERROR_SUCCESS;
		}
	} else {
		free(Request);
		free(Response);
		return ERROR_SUCCESS;
	}
	free(Request);
	free(Response);
#ifdef DEBUG
	DebugPrint("Patches Error!");
#endif
	return E_FAIL;
}

DWORD Requests::Setup() {
	if (FAILED(Requests::Auth())) {
#ifdef DEBUG
		DebugPrint("Auth Failed!");
#endif 
		return E_FAIL;
	}

	if (FAILED(Requests::UpdateTime())) {
#ifdef DEBUG
		DebugPrint("updt Failed!");
#endif 
		return E_FAIL;
	}
	Patches(0x81000000);
	m_Initalized = TRUE;
	return ERROR_SUCCESS;
}
