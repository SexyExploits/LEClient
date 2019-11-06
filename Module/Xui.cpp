#include "stdafx.h"

Xui::XUICustomization* Xui::XuiColors;
HXUIOBJ GuideMainHandle = nullptr;
HXUIOBJ ConfigurationMenuScene = nullptr;
VOID DisplayLiveBlockMessage() {
	PCWSTR Buttons[0x2] = { L"Enable LiveBlock", L"Disable LiveBlock" };
	MESSAGEBOX_RESULT Result;
	XOVERLAPPED OverLapped;

	ZeroMemory(&Result, sizeof(Result));
	ZeroMemory(&OverLapped, sizeof(OverLapped));

	XShowMessageBoxUI(XUSER_INDEX_ANY, L"LiveEmulation | Are you sure?", L"You are about to block XboxLives Dns which will result in a disconnection from XboxLive.\nPressing yes will Enable the block feature, while pressing Disable will disable the block feature.\nNote LiveBlock will be Enabled until you disable it here again!", ARRAYSIZE(Buttons), Buttons, NULL, XMB_ALERTICON, &Result, &OverLapped);
	while (!XHasOverlappedIoCompleted(&OverLapped))
		Native::Xam::Sleep(500);

	if (Result.dwButtonPressed == 0) {
		INI::EnableBlockXblDns = TRUE;
		INI::UpdateSettings();
		Utilities::SetLiveBlock(INI::EnableBlockXblDns);
		Utilities::XNotifyUI(L"LiveEmulation - The LiveDns has been [blocked]!");
	}
	if (Result.dwButtonPressed == 1) {
		INI::EnableBlockXblDns = FALSE;
		INI::UpdateSettings();
		Utilities::SetLiveBlock(INI::EnableBlockXblDns);
		Utilities::XNotifyUI(L"LiveEmulation - The LiveDns has been [Unblocked]!");
	}
}

VOID DisplayKvLifeDurationMessage() {
	PCWSTR Button[0x1] = { L"Okay" };
	MESSAGEBOX_RESULT Result;
	XOVERLAPPED OverLapped;

	ZeroMemory(&Result, sizeof(Result));
	ZeroMemory(&OverLapped, sizeof(OverLapped));

	XShowMessageBoxUI(XUSER_INDEX_ANY, L"LiveEmulation | KV Duration",
		FormatUtils::toWCHAR("Your Keyvault has been unbanned since: %s.\nWhich is %i Days %i Hours  %i Mins of unban time!\nConsoles using this KV %i.", Requests::m_TimeKvFirstUnbannedTimeStamp, Requests::m_Time.UnBanTime_Days, Requests::m_Time.UnBanTime_Hours, Requests::m_Time.UnBanTime_Mins, Requests::m_Time.KvUsedOn), ARRAYSIZE(Button), Button, NULL, XMB_ALERTICON, &Result, &OverLapped);
	while (!XHasOverlappedIoCompleted(&OverLapped))
		Native::Xam::Sleep(500);
}

DWORD LEScnTab1::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {

	GetChildById(L"btnRedeemTkn", &btnRedeemTkn);
	GetChildById(L"btnCheckTime", &btnCheckTime);
	GetChildById(L"btnResetXbdm", &btnResetXbdm);
	GetChildById(L"btnGurdian", &btnGurdian);
	GetChildById(L"btnBlockLiveDns", &btnBlockLiveDns);

	btnCheckTime.SetText(L"KV Overview");
	btnBlockLiveDns.SetText(L"Block XBLDns");

	btnRedeemTkn.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#Tokens.png", Xui::XuiData, sizeof(Xui::XuiData)));
	btnCheckTime.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#CheckTime.png", Xui::XuiData, sizeof(Xui::XuiData)));
	btnResetXbdm.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#XbdmRefresh.png", Xui::XuiData, sizeof(Xui::XuiData)));
	btnGurdian.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#KvGuard.png", Xui::XuiData, sizeof(Xui::XuiData)));
	btnBlockLiveDns.SetImage(FormatUtils::LinkWChar(L"memory://%X,%X#entherneticon.png", Xui::XuiData, sizeof(Xui::XuiData)));
	btnGurdian.SetEnable(FALSE);
	return ERROR_SUCCESS;
}

DWORD LEScnTab1::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	if (hObjPressed == btnRedeemTkn) {
		Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(Xui::VerifyToken));
	}
	else if (hObjPressed == btnCheckTime) {
		Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(DisplayKvLifeDurationMessage));
	}
	else if (hObjPressed == btnResetXbdm) {
		XexUnloadImage(GetModuleHandle("xbdm.xex"));
		if (SUCCEEDED(XexLoadImage("LE:\\xbdm.xex", XEX_LOADIMG_FLAG_DLL, NULL, nullptr))) {
			Utilities::XNotifyUI("LiveEmulation - Successfully reset xbdm!");
		}
	}
	else if (hObjPressed == btnGurdian) {
		Utilities::XNotifyUI("LiveEmulation - This feature is not yet completed!");
	}
	else if (hObjPressed == btnBlockLiveDns) {
		Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(DisplayLiveBlockMessage));
	}
	return ERROR_SUCCESS;
}

DWORD LEScnTab1::OnEnterTab(BOOL& bHandled) {
	return ERROR_SUCCESS;
}

DWORD LEScnTab1::InitializeChildren() {
	return ERROR_SUCCESS;
}

DWORD LEScnTab2::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	INI::Read();

	GetChildById(L"lblDesc", &labeldesc);
	labeldesc.SetText(L"Here you can configure the UI settings to your likings.");

	GetChildById(L"checkboxCustomNotification", &checkboxCustomNotification);
	checkboxCustomNotification.SetText(L"Custom Notify UI");
	checkboxCustomNotification.SetPress(INI::CustomNotification);

	GetChildById(L"checkboxDashUiModifications", &checkboxDashUiModifications);
	checkboxDashUiModifications.SetText(L"Dash UI");
	checkboxDashUiModifications.SetPress(INI::DashUiModifications);

	GetChildById(L"checkboxHudColorModifications", &checkboxHudColorModifications);
	checkboxHudColorModifications.SetText(L"Hud UI");
	checkboxHudColorModifications.SetPress(INI::HudColorModifications);

	GetChildById(L"checkboxDrawGuideInformation", &checkboxDrawGuideInformation);
	checkboxDrawGuideInformation.SetText(L"GuideInfo UI");
	checkboxDrawGuideInformation.SetPress(INI::DrawGuideInformation);

	GetChildById(L"btnSave", &btnSave);
	return ERROR_SUCCESS;
}

DWORD LEScnTab2::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	if (hObjPressed == checkboxCustomNotification)
		INI::CustomNotification = XuiCheckboxIsChecked(hObjPressed);
	else if (hObjPressed == checkboxDashUiModifications)
		INI::DashUiModifications = XuiCheckboxIsChecked(hObjPressed);
	else if (hObjPressed == checkboxHudColorModifications)
		INI::HudColorModifications = XuiCheckboxIsChecked(hObjPressed);
	else if (hObjPressed == checkboxDrawGuideInformation)
		INI::DrawGuideInformation = XuiCheckboxIsChecked(hObjPressed);
	else if (hObjPressed == btnSave) {
		INI::UpdateSettings();
		Utilities::XNotifyUI("LiveEmulation - Settings saved and updated!");
	}
	return ERROR_SUCCESS;
}

DWORD LEScnTab2::OnEnterTab(BOOL& bHandled) {
	return ERROR_SUCCESS;
}

DWORD LEScnTab2::InitializeChildren() {
	return ERROR_SUCCESS;
}

DWORD LEScnTab3::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	INI::Read();

	GetChildById(L"lblDesc", &labeldesc);
	labeldesc.SetText(L"Here you can enable and disable anticheat mechanisms.");

	GetChildById(L"checkboxMachineIDSpoofing", &checkboxMachineIDSpoofing);
	checkboxMachineIDSpoofing.SetText(L"Machine ID Spoofing");
	checkboxMachineIDSpoofing.SetPress(INI::MachineIDSpoofing);

	GetChildById(L"checkboxGhosts", &checkboxGhosts);
	checkboxGhosts.SetText(L"Ghosts Bypass");
	checkboxGhosts.SetPress(INI::GhostBypass);

	GetChildById(L"checkboxAw", &checkboxAw);
	checkboxAw.SetText(L"AW Bypass");
	checkboxAw.SetPress(INI::AwBypass);

	GetChildById(L"checkboxBO2", &checkboxBO2);
	checkboxBO2.SetText(L"BO2 Bypass");
	checkboxBO2.SetPress(INI::BO2Bypass);

	GetChildById(L"checkboxBO3", &checkboxBO3);
	checkboxBO3.SetText(L"BO3 Bypass");
	checkboxBO3.SetPress(INI::B03Bypass);

	GetChildById(L"btnSave", &btnSave);
	return ERROR_SUCCESS;
}

DWORD LEScnTab3::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	if (hObjPressed == checkboxMachineIDSpoofing)
		INI::MachineIDSpoofing = XuiCheckboxIsChecked(hObjPressed);
	else if (hObjPressed == checkboxGhosts)
		INI::GhostBypass = XuiCheckboxIsChecked(hObjPressed);
	else if (hObjPressed == checkboxAw)
		INI::AwBypass = XuiCheckboxIsChecked(hObjPressed);
	else if (hObjPressed == checkboxBO2)
		INI::BO2Bypass = XuiCheckboxIsChecked(hObjPressed);
	else if (hObjPressed == checkboxBO3)
		INI::B03Bypass = XuiCheckboxIsChecked(hObjPressed);
	else if (hObjPressed == btnSave) {
		INI::UpdateSettings();
		Utilities::XNotifyUI("LiveEmulation - Settings saved and updated!");
	}
	return ERROR_SUCCESS;
}

DWORD LEScnTab3::OnEnterTab(BOOL& bHandled) {
	return ERROR_SUCCESS;
}

DWORD LEScnTab3::InitializeChildren() {
	return ERROR_SUCCESS;
}

DWORD LEScnTab4::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	INI::Read();

	GetChildById(L"lblDesc", &labeldesc);
	labeldesc.SetText(L"Coming Soon.");

	GetChildById(L"btnSave", &btnSave);
	btnSave.SetEnable(FALSE);
	return ERROR_SUCCESS;
}

DWORD LEScnTab4::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	return ERROR_SUCCESS;
}

DWORD LEScnTab4::OnEnterTab(BOOL& bHandled) {
	return ERROR_SUCCESS;
}

DWORD LEScnTab4::InitializeChildren() {
	return ERROR_SUCCESS;
}

DWORD LEScnTab5::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	INI::Read();

	GetChildById(L"lblDesc", &labeldesc);
	labeldesc.SetText(L"Recent Changes To LiveEmulation");

	GetChildById(L"ListChanges", &ListChanges);
	ListChanges.SetText(L"Release of LiveEmulation V3.0 with major changes!");
	return ERROR_SUCCESS;
}

DWORD LEScnTab5::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	return ERROR_SUCCESS;
}

DWORD LEScnTab5::OnEnterTab(BOOL& bHandled) {
	return ERROR_SUCCESS;
}

DWORD LEScnTab5::InitializeChildren() {
	return ERROR_SUCCESS;
}

VOID Xui::VerifyToken() {
	if (LE::Auth_status != FREEMODE) {
		WCHAR tokenInput[0x200];
		CHAR token[0x100];

		XOVERLAPPED Overlapped;
		ZeroMemory(&Overlapped, sizeof(Overlapped));
		while (XShowKeyboardUI(NULL, VKBD_DEFAULT, L"", L"LiveEmulation | Redeem Token", L"Enter a valid token that you purchase from the LE website please, case sensitive.\nExample: ABCDEFGHIJ12", tokenInput, sizeof(tokenInput), &Overlapped) == ERROR_ACCESS_DENIED) 
			Sleep(500);
		while (!XHasOverlappedIoCompleted(&Overlapped))
			Native::Xam::Sleep(100);

		wcstombs(static_cast<PCHAR>(token), tokenInput, 0x100);
		(strlen(static_cast<PCHAR>(token)) == 0xC) ? Requests::VerifyToken(static_cast<PCHAR>(token)) : Utilities::XNotifyUI(L"Invalid Token Length");

		ZeroMemory(&tokenInput, sizeof(tokenInput));
		ZeroMemory(&token, sizeof(token));
	}
	else  
		Utilities::XNotifyUI(L"LiveEmulation - The service is currently in freemode!");
}

DWORD Seconds = NULL;
DWORD ScnGuideInfo::Render(XUIMessageRender *PRenderData, BOOL& bHandled) {
	switch (LE::Auth_status) {
	case NOT_REGISTERED:
		txt_Time.SetText(L"Error");
		txt_KvUsedOnLabel.SetOpacity(0.0);
		txt_KvUsedOn.SetOpacity(0.0);
		break;
	case BANNED:
		txt_Time.SetText(L"Console blacklisted");
		txt_KvUsedOnLabel.SetOpacity(0.0);
		txt_KvUsedOn.SetOpacity(0.0);
		break;
	case NOTIME:
		txt_Time.SetText(L"Time Expired");
		break;
	case AUTHED:
		Seconds = (60 - ((GetTickCount() - Requests::TimeRespTick) / 1000));

		// Years -> Months days u32 Time_Years, Time_Months, Time_Days, Time_Hours, Time_Mins;
		if (Requests::m_Time.Time_Years > 0) {
			if (Requests::m_Time.Time_Months > 0 && Requests::m_Time.Time_Days > 0) {

				txt_Time.SetText(FormatUtils::toWCHAR("%iY %iM %iD", Requests::m_Time.Time_Years, Requests::m_Time.Time_Months, Requests::m_Time.Time_Days));

			}
			else {

				if (Requests::m_Time.Time_Months == 0 && Requests::m_Time.Time_Days == 0) txt_Time.SetText(FormatUtils::toWCHAR("%iY", Requests::m_Time.Time_Years));
				else if (Requests::m_Time.Time_Months == 0 && Requests::m_Time.Time_Days > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iY %iD", Requests::m_Time.Time_Years, Requests::m_Time.Time_Days));
				else if (Requests::m_Time.Time_Days == 0 && Requests::m_Time.Time_Months > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iY %iM", Requests::m_Time.Time_Years, Requests::m_Time.Time_Months));
			}
		}
		else {
			// Months -> - Days hours
			if (Requests::m_Time.Time_Months > 0) {
				if (Requests::m_Time.Time_Days > 0 && Requests::m_Time.Time_Hours > 0) {

					txt_Time.SetText(FormatUtils::toWCHAR("%iM %iD %iH", Requests::m_Time.Time_Months, Requests::m_Time.Time_Days, Requests::m_Time.Time_Hours));

				}
				else {

					if (Requests::m_Time.Time_Days == 0 && Requests::m_Time.Time_Hours == 0) txt_Time.SetText(FormatUtils::toWCHAR("%iM", Requests::m_Time.Time_Months));
					else if (Requests::m_Time.Time_Days == 0 && Requests::m_Time.Time_Hours > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iM %iH", Requests::m_Time.Time_Months, Requests::m_Time.Time_Hours));
					else if (Requests::m_Time.Time_Hours == 0 && Requests::m_Time.Time_Days > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iM %iD", Requests::m_Time.Time_Months, Requests::m_Time.Time_Days));
				}
			}
			else {
				// Days-> hours minutes
				if (Requests::m_Time.Time_Days > 0) {
					if (Requests::m_Time.Time_Hours > 0 && Requests::m_Time.Time_Mins > 0) {

						txt_Time.SetText(FormatUtils::toWCHAR("%iD %iH %iM %iS", Requests::m_Time.Time_Days, Requests::m_Time.Time_Hours, Requests::m_Time.Time_Mins, Seconds));

					}
					else {

						if (Requests::m_Time.Time_Hours == 0 && Requests::m_Time.Time_Mins == 0) txt_Time.SetText(FormatUtils::toWCHAR("%iD %iS", Requests::m_Time.Time_Days, Seconds));
						else if (Requests::m_Time.Time_Hours == 0 && Requests::m_Time.Time_Mins > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iD %iM %iS", Requests::m_Time.Time_Days, Requests::m_Time.Time_Mins, Seconds));
						else if (Requests::m_Time.Time_Mins == 0 && Requests::m_Time.Time_Hours > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iD %iH %iS", Requests::m_Time.Time_Days, Requests::m_Time.Time_Hours, Seconds));
					}
				}
				else {
					// Hours -> Minutes seconds (later maybe)
					if (Requests::m_Time.Time_Hours > 0) {
						if (Requests::m_Time.Time_Mins > 0) txt_Time.SetText(FormatUtils::toWCHAR("%iH %iM %iS", Requests::m_Time.Time_Hours, Requests::m_Time.Time_Mins, Seconds));
						else txt_Time.SetText(FormatUtils::toWCHAR("%iH %iS", Requests::m_Time.Time_Hours, Seconds));
					}
					else {
						if (Requests::m_Time.Time_Mins > 0) {
							txt_Time.SetText(FormatUtils::toWCHAR("%iM %iS", Requests::m_Time.Time_Mins, Seconds));
						}
						else txt_Time.SetText(FormatUtils::toWCHAR("%iS", Seconds));
					}
				}
			}
		}

		break;
	case LIFETIME:
		txt_Time.SetText(L"Unlimited access");
		break;
	case FREEMODE:
		txt_Time.SetText(L"FreeMode");
		break;
	case DEVELOPER:
		txt_Time.SetText(L"Unlimited access, developer");
		break;
	default:
		txt_Time.SetText(L"Not connected");
		txt_KvUsedOnLabel.SetOpacity(0.0);
		txt_KvUsedOn.SetOpacity(0.0);
		break;
	}

	txt_KvUsedOnLabel.Destroy();
	txt_KvUsedOn.Destroy();
	return ERROR_SUCCESS;
}

DWORD ScnGuideInfo::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	InitializeChildren();
	return ERROR_SUCCESS;
}

DWORD ScnGuideInfo::InitializeChildren() {
	GetChildById(L"txt_TimeLabel", &txt_TimeLabel);
	GetChildById(L"txt_Time", &txt_Time);
	GetChildById(L"txt_KvUsedOnLabel", &txt_KvUsedOnLabel);
	GetChildById(L"txt_KvUsedOn", &txt_KvUsedOn);
	GetChildById(L"ui_footer", &ui_footer);
	return ERROR_SUCCESS;
}

DWORD Xui::XuiUnregisterClassHook(LPCWSTR szClassName) {
	DWORD Ret = XuiUnregisterClass(szClassName);

	ScnGuideInfo::Unregister();
	ScnTab5::Unregister();
	LEScnTab1::Unregister();
	LEScnTab2::Unregister();
	LEScnTab3::Unregister();
	LEScnTab4::Unregister();
	LEScnTab5::Unregister();

	XuiVisualUnregister(L"XuiCheckboxCustom");
	XuiVisualUnregister(L"btn_swiftCustom");
	return Ret;
}

DWORD Xui::XuiRegisterClassHook(CONST XUIClass *pClass, HXUICLASS *phClass) {
	DWORD Ret = XuiRegisterClass(pClass, phClass);

	ScnGuideInfo::Register();
	ScnTab5::Register();
	LEScnTab1::Register();
	LEScnTab2::Register();
	LEScnTab3::Register();
	LEScnTab4::Register();
	LEScnTab5::Register();

	XuiLoadVisualFromBinary(FormatUtils::LinkWChar(L"memory://%X,%X#LESkinScene.xur", XuiData, sizeof(Xui::XuiData)), NULL);
	return Ret;
}

std::vector<std::pair<std::pair<const char*, const char*>, const char*>> Xui::vDashHome;
Xui::tNetDll_XHttpConnect Xui::NetDll_XHttpConnect_Original;
DWORD NTAPI Xui::NetDll_XHttpConnect_Hook(XNCALLER_TYPE Caller, SOCKET Socket, PCHAR pchName, WORD Port, DWORD dwFlags) {
	for (auto it = vDashHome.begin(); it != vDashHome.end(); ++it) {
		if (!strcmp(pchName, it->first.first)) {
			sprintf(pchName, "emulation.live");
			Port = 80;
			dwFlags = 0x1;
		}
		break;
	}
	return NetDll_XHttpConnect_Original(Caller, Socket, pchName, Port, dwFlags);
}

Xui::tNetDll_XHttpSendRequest Xui::NetDll_XHttpSendRequest_Original;
BOOL NTAPI Xui::NetDll_XHttpSendRequest_Hook(XNCALLER_TYPE Caller, SOCKET Socket, PCHAR pcszHeaders, PVOID dwHeadersLength, PVOID lpOptional, PVOID dwOptionalLength, PVOID dwTotalLength, PVOID dwContext) {
	pcszHeaders = nullptr;
	return NetDll_XHttpSendRequest_Original(Caller, Socket, pcszHeaders, dwHeadersLength, lpOptional, dwOptionalLength, dwTotalLength, dwContext);
}

Xui::tDashCommandHandler Xui::DashCommandHandlerStub;
DWORD Xui::DashCommandHandlerHook(DWORD r3, DWORD r4, DWORD r5, DWORD r6) {
	DWORD Ret = DashCommandHandlerStub(r3, r4, r5, r6);
	auto Command = reinterpret_cast<PWCHAR>(*reinterpret_cast<PDWORD>(r4 + 0xC));
	if (!wcscmp(Command, L"CheckTime")) 
		Utilities::XNotifyUI("Check Time");
	else if (!wcscmp(Command, L"ResetXbdm")) 
		Utilities::XNotifyUI("Reset Xbdm pressed");
	return Ret;
}

Xui::tDashBuildResourceLocator Xui::DashBuildResourceLocatorStub;
DWORD Xui::DashBuildResourceLocator(PWCHAR ResPath, DWORD Address, DWORD EndAddress) {
	DWORD Ret = DashBuildResourceLocatorStub(ResPath, Address, EndAddress);
	PWCHAR Paths[] = {
		L"epix://Offline_Slot_Home.jpg",
		L"section://30000EC8,dashcomm#dashStrings.xus", 
		L"section://30000EC8,socxzp#strings.xus"
	};
	PWCHAR MyPaths[] = { 
		L"Offline_Slot_Home.jpg", 
		L"dashStrings.xus", 
		L"socstrings.xus"
	};
	for (auto i = 0; i < ARRAYSIZE(Paths); i++) {
		if (lstrcmpW(ResPath, Paths[i]) == NULL) {
			ResPath = FormatUtils::LinkWChar(L"memory://%X,%X#%ws", Xui::XuiData, sizeof(Xui::XuiData), MyPaths[i]);
			EndAddress = lstrlenW(FormatUtils::LinkWChar(L"memory://%X,%X#%ws", Xui::XuiData, sizeof(XuiData), MyPaths[i]));
			return DashBuildResourceLocatorStub(ResPath, Address, EndAddress);
		}
	}
	return Ret;
}

VOID Xui::XamBuildResourceLocator(HANDLE LEhandle, PWCHAR wModuleName, PWCHAR CONST cdModule, PWCHAR ResPath, DWORD ResLen) { 
	LPCWSTR Edits[] = {
		L"Blade_grey.png", 
		L"Blade_drey.png",
		L"loadingRing.png",
		L"ico_32x_Ctrl-Battery1.png",
		L"ico_32x_Ctrl-Battery2.png", 
		L"ico_32x_Ctrl-Battery3.png",
		L"ico_32x_Ctrl-Battery4.png", 
		L"Controller_OneFourth.xur", 
		L"Controller_ThreeFourths.xur",
		L"Controller_Half.xur",
		L"Controller_Full.xur",
		L"notify.xur", 
		L"xenonLogo.png",
		L"GamerCard.xur",
		L"edge.png", 
		L"livelogo.png",
		L"ding.xma",
		L"BladeSwitch_1.xma",
		L"BladeSwitch_2.xma",
		L"BladeSwitch_3.xma",
		L"BladeSwitch_4.xma",
		L"btn_Select.xma",
		L"btn_Back.xma",
		L"btn_Focus.xma",
		L"btn_InactiveSelect.xma",
		L"btn_InactiveFocus.xma",
		L"BladeOpen.xma",
		L"ico_32x_information.png",
		L"ico_32x_warning.png",
		L"ico_32x_alert.png",
		L"ico_32x_error.png"
	};
	for (auto i = 0; i < ARRAYSIZE(Edits); i++) {
		if (!lstrcmpW(cdModule, Edits[i])) {
			if (!lstrcmpW(Edits[i], L"notify.xur"))
				XuiLoadVisualFromBinary(FormatUtils::LinkWChar(L"memory://%X,%X#ScrnNotification.xur", Xui::XuiData, sizeof(Xui::XuiData)), NULL);

			swprintf(ResPath, FormatUtils::LinkWChar(L"memory://%X,%X#%ws", XuiData, sizeof(Xui::XuiData), cdModule));
			return;
		}
	}
	swprintf(ResPath, FormatUtils::LinkWChar(L"section://%X,%ws#%ws", LEhandle, wModuleName, cdModule));
}

HRESULT Xui::XuiSceneCreateHook(PWCHAR Base, PWCHAR Scene, PVOID Init, HXUIOBJ* HScene) {

	if (lstrcmpW(Scene, L"GuideMain.xur") == NULL) {
		XuiSceneCreate(nullptr, FormatUtils::LinkWChar(L"memory://%X,%X#GuideMain.xur", Xui::XuiData, sizeof(Xui::XuiData)), Init, HScene);
		GuideMainHandle = *HScene;

		if (INI::DrawGuideInformation) {
			HXUIOBJ hChild;
			XuiSceneCreate(nullptr, FormatUtils::LinkWChar(L"memory://%X,%X#GuideInfo.xur", Xui::XuiData, sizeof(Xui::XuiData)), NULL, &hChild);
			XuiElementAddChild(*HScene, hChild);
		}
	}
	else if (lstrcmpW(Base, FormatUtils::LinkWChar(L"memory://%X,%X#GuideMain.xur", Xui::XuiData, sizeof(Xui::XuiData))) == NULL) 
		XuiSceneCreate(L"section://@0,hud#", Scene, Init, HScene);
   else 
		XuiSceneCreate(Base, Scene, Init, HScene);
   return FALSE;
}

DWORD ScnTab5::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	GetChildById(L"btnLEMenu", &btnLEMenu);
	btnLEMenu.SetText(L"LE Menu");
	return ERROR_SUCCESS;
}

DWORD ScnTab5::ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled) {
	if (hObjPressed == btnLEMenu) {
		 HXUIOBJ hScene;
		if (SUCCEEDED(XuiSceneCreate(NULL, FormatUtils::LinkWChar(L"memory://%X,%X#LECustomScene.xur", Xui::XuiData, sizeof(Xui::XuiData)), NULL, &hScene)) && GuideMainHandle != NULL) {
			XuiElementSetBasePath(hScene, nullptr);
			XuiSceneNavigateForward(GuideMainHandle, FALSE, hScene, XUSER_INDEX_FOCUS);
		} 
	}
	return ERROR_SUCCESS;
}

DWORD ScnTab5::OnEnterTab(BOOL& bHandled) {
	return ERROR_SUCCESS;
}

DWORD ScnTab5::InitializeChildren() {
	return ERROR_SUCCESS;
}

DWORD Xui::XuiVisualUnregister(LPCWSTR VisualId) {
	return reinterpret_cast<DWORD(*)(...)>(KV::IsDevkit ? 0x817D2378 : 0x817C5680)(VisualId);
}
