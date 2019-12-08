#include "stdafx.h"

Xui::XUICustomization* Xui::XuiColors;
VOID Xui::VerifyToken() {
	if (LE::s_Auth_status != FREEMODE) {
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
DWORD HUD_UI::ScnGuideInfo::Render(XUIMessageRender *PRenderData, BOOL& bHandled) {
	switch (LE::s_Auth_status) {
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

DWORD HUD_UI::ScnGuideInfo::OnInit(XUIMessageInit *pInitData, BOOL& bHandled) {
	this->GetChildById(L"txt_TimeLabel", &txt_TimeLabel);
	this->GetChildById(L"txt_Time", &txt_Time);
	this->GetChildById(L"txt_KvUsedOnLabel", &txt_KvUsedOnLabel);
	this->GetChildById(L"txt_KvUsedOn", &txt_KvUsedOn);
	this->GetChildById(L"ui_footer", &ui_footer);

	bHandled = true;
	return ERROR_SUCCESS;
}