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
	static DWORD XuiUnregisterClassHook(LPCWSTR szClassName);
	static DWORD XuiRegisterClassHook(CONST XUIClass *pClass, HXUICLASS *phClass);

	static std::vector<std::pair<std::pair<const char*, const char*>, const char*>> vDashHome;
	typedef DWORD(NTAPI* tNetDll_XHttpConnect)(XNCALLER_TYPE, SOCKET, PCHAR, WORD, DWORD);
	static tNetDll_XHttpConnect NetDll_XHttpConnect_Original;

	typedef BOOL(NTAPI* tNetDll_XHttpSendRequest)(XNCALLER_TYPE, SOCKET, PCHAR, PVOID, PVOID, PVOID, PVOID, PVOID);
	static tNetDll_XHttpSendRequest NetDll_XHttpSendRequest_Original;

	typedef DWORD(*tDashCommandHandler)(DWORD, DWORD, DWORD, DWORD);
	static tDashCommandHandler DashCommandHandlerStub;

	typedef DWORD(*tDashBuildResourceLocator)(PWCHAR ResPath, DWORD Address, DWORD EndAddress);
	static tDashBuildResourceLocator DashBuildResourceLocatorStub;

	static DWORD NTAPI NetDll_XHttpConnect_Hook(XNCALLER_TYPE Caller, SOCKET Socket, PCHAR pchName, WORD Port, DWORD dwFlags);
	static BOOL NTAPI NetDll_XHttpSendRequest_Hook(XNCALLER_TYPE Caller, SOCKET Socket, PCHAR pcszHeaders, PVOID dwHeadersLength, PVOID lpOptional, PVOID dwOptionalLength, PVOID dwTotalLength, PVOID dwContext);
	static DWORD XuiVisualUnregister(LPCWSTR VisualId);
	static BYTE XuiData[208062];
	static HRESULT XuiSceneCreateHook(PWCHAR Base, PWCHAR Scene, PVOID Init, HXUIOBJ* HScene);

	static INT TimeTicky;
	static DWORD DashCommandHandlerHook(DWORD r3, DWORD r4, DWORD r5, DWORD r6);
	static DWORD DashBuildResourceLocator(PWCHAR ResPath, DWORD Unk1, DWORD ResLen);
	static VOID XamBuildResourceLocator(HANDLE LEhandle, PWCHAR wModuleName, PWCHAR CONST cdModule, PWCHAR ResPath, DWORD ResLen);
	static VOID VerifyToken();
};

class LEScnTab1 : CXuiSceneImpl {
public:
	LEScnTab1() {}
	~LEScnTab1() {}

	static LEScnTab1& getInstance() {
		static LEScnTab1 singleton;
		return singleton;
	}

	CXuiControl btnRedeemTkn, btnCheckTime, btnResetXbdm, btnGurdian, btnBlockLiveDns;

	XUI_IMPLEMENT_CLASS(LEScnTab1, L"iniTab1", XUI_CLASS_SCENE)
	XUI_BEGIN_MSG_MAP()
	XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
	XUI_ON_XM_INIT(OnInit)
	XUI_ON_XM_ENTER_TAB(OnEnterTab)
	XUI_END_MSG_MAP()

	DWORD OnEnterTab(BOOL& bHandled);
	DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
	DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	DWORD InitializeChildren();
};

class LEScnTab2 : CXuiSceneImpl {
public:
	LEScnTab2() {}
	~LEScnTab2() {}

	static LEScnTab2& getInstance() {
		static LEScnTab2 singleton;
		return singleton;
	}

	CXuiTextElement labeldesc;
	CXuiControl checkboxCustomNotification, checkboxDashUiModifications, checkboxHudColorModifications,
		checkboxDrawGuideInformation, btnSave;

	XUI_IMPLEMENT_CLASS(LEScnTab2, L"iniTab2", XUI_CLASS_SCENE)
	XUI_BEGIN_MSG_MAP()
	XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
	XUI_ON_XM_INIT(OnInit)
	XUI_ON_XM_ENTER_TAB(OnEnterTab)
	XUI_END_MSG_MAP()

	DWORD OnEnterTab(BOOL& bHandled);
	DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
	DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	DWORD InitializeChildren();
};

class LEScnTab3 : CXuiSceneImpl {
public:
	LEScnTab3() {}
	~LEScnTab3() {}

	static LEScnTab3& getInstance() {
		static LEScnTab3 singleton;
		return singleton;
	}

	CXuiTextElement labeldesc;
	CXuiControl checkboxMachineIDSpoofing, checkboxGhosts, checkboxAw, checkboxBO2, checkboxBO3, btnSave;

	XUI_IMPLEMENT_CLASS(LEScnTab3, L"iniTab3", XUI_CLASS_SCENE)
	XUI_BEGIN_MSG_MAP()
	XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
	XUI_ON_XM_INIT(OnInit)
	XUI_ON_XM_ENTER_TAB(OnEnterTab)
	XUI_END_MSG_MAP()

	DWORD OnEnterTab(BOOL& bHandled);
	DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
	DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	DWORD InitializeChildren();
};

class LEScnTab4 : CXuiSceneImpl {
public:
	LEScnTab4() {}
	~LEScnTab4() {}

	static LEScnTab4& getInstance() {
		static LEScnTab4 singleton;
		return singleton;
	}

	CXuiTextElement labeldesc;
	CXuiControl btnSave;

	XUI_IMPLEMENT_CLASS(LEScnTab4, L"iniTab4", XUI_CLASS_SCENE)
	XUI_BEGIN_MSG_MAP()
	XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
	XUI_ON_XM_INIT(OnInit)
	XUI_ON_XM_ENTER_TAB(OnEnterTab)
	XUI_END_MSG_MAP()

	DWORD OnEnterTab(BOOL& bHandled);
	DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
	DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	DWORD InitializeChildren();
};

class LEScnTab5 : CXuiSceneImpl {
public:
	LEScnTab5() {}
	~LEScnTab5() {}

	static LEScnTab5& getInstance() {
		static LEScnTab5 singleton;
		return singleton;
	}

	CXuiListItem ListChanges; 
	CXuiTextElement labeldesc;

	XUI_IMPLEMENT_CLASS(LEScnTab5, L"iniTab5", XUI_CLASS_SCENE)
	XUI_BEGIN_MSG_MAP()
	XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
	XUI_ON_XM_INIT(OnInit)
	XUI_ON_XM_ENTER_TAB(OnEnterTab)
	XUI_END_MSG_MAP()

	DWORD OnEnterTab(BOOL& bHandled);
	DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
	DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	DWORD InitializeChildren();
};

class ScnTab5 : CXuiSceneImpl {
public:
	ScnTab5() {}
	~ScnTab5() {}

	static ScnTab5& getInstance() {
		static ScnTab5 singleton;
		return singleton;
	}

	CXuiControl btnLEMenu;

	XUI_IMPLEMENT_CLASS(ScnTab5, L"lbTab5", XUI_CLASS_SCENE)
	XUI_BEGIN_MSG_MAP()
	XUI_ON_XM_NOTIFY_PRESS(ButtonMonitor)
	XUI_ON_XM_INIT(OnInit)
	XUI_ON_XM_ENTER_TAB(OnEnterTab)
	XUI_END_MSG_MAP()

	DWORD OnEnterTab(BOOL& bHandled);
	DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
	DWORD ButtonMonitor(HXUIOBJ hObjPressed, BOOL& bHandled);
	DWORD InitializeChildren();
};

class ScnGuideInfo : CXuiSceneImpl {
public:

	static ScnGuideInfo& getInstance() {
		static ScnGuideInfo singleton;
		return singleton;
	}

	CXuiControl ui_footer;
   	CXuiTextElement txt_TimeLabel, txt_Time, txt_KvUsedOnLabel, txt_KvUsedOn;

	XUI_IMPLEMENT_CLASS(ScnGuideInfo, L"GuideMainAdditionalScene", XUI_CLASS_SCENE)
	XUI_BEGIN_MSG_MAP()
	XUI_ON_XM_INIT(OnInit)
	XUI_ON_XM_RENDER(Render)
	XUI_END_MSG_MAP()

	DWORD OnInit(XUIMessageInit *pInitData, BOOL& bHandled);
	DWORD Render(XUIMessageRender *PRenderData, BOOL& bHandled);
	DWORD InitializeChildren();
};