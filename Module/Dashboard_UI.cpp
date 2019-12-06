#include "stdafx.h"

//DWORD BrandColor = 0xFF0A7562;

DWORD Color1 = 0xFF209079;
DWORD Color2 = 0xFF30C6AA;

tXuiElementBeginRender XuiElementBeginRenderStub;

HRESULT XuiElementBeginRenderHook(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct) {
	HXUIOBJ hParentObj; LPCWSTR ObjID, ParentText;
	Dashboard_Calls::XuiElementGetId(hObj, &ObjID);
	Dashboard_Calls::XuiElementGetParent(hObj, &hParentObj);
	Dashboard_Calls::XuiElementGetId(hParentObj, &ParentText);

	if (NT_SUCCESS(XuiElementBeginRenderStub(hObj, pRenderData, pRenderStruct))) {
		if (lstrcmpW(ObjID, L"Background") == 0 || lstrcmpW(ObjID, L"GreenHighlight") == 0 || lstrcmpW(ObjID, L"_Background") == 0 ||
			lstrcmpW(ObjID, L"figSearchBox") == 0 || lstrcmpW(ParentText, L"DescriptionGrid") == 0 || lstrcmpW(ParentText, L"HubSlotXzpScene") == 0 ||
			lstrcmpW(ParentText, L"Back") == 0 || lstrcmpW(ParentText, L"Color") == 0 || lstrcmpW(ParentText, L"Front") == 0 ||
			lstrcmpW(ParentText, L"_SceneHost") == 0)

			Dashboard_Calls::SetFill(hObj, Color2, Color1);
		return ERROR_SUCCESS;
	}

	return E_FAIL;
}

VOID ResolveFunctions() {
	Dashboard_Calls::XuiFreeVisuals = (VOID(*)(LPCWSTR))Utilities::Resolve(Games::DashboardHandle, 10231);
	Dashboard_Calls::XuiElementGetId = (HRESULT(*)(HXUIOBJ, LPCWSTR*))Utilities::Resolve(Games::DashboardHandle, 10176);
	Dashboard_Calls::XuiElementGetChildById = (HRESULT(*)(HXUIOBJ, LPCWSTR, HXUIOBJ*))Utilities::Resolve(Games::DashboardHandle, 10177);
	Dashboard_Calls::DashXuiAlloc = (PVOID(*)(size_t))Utilities::Resolve(Games::DashboardHandle, 10000);
	Dashboard_Calls::DashXuiFree = (VOID(*)(PVOID))Utilities::Resolve(Games::DashboardHandle, 10002);
	Dashboard_Calls::XUIElementPropVal_Construct = (VOID(*)(Dashboard_Structures::XUIElementPropVal*))Utilities::Resolve(Games::DashboardHandle, 10079);
	Dashboard_Calls::XUIElementPropVal_Destruct = (VOID(*)(Dashboard_Structures::XUIElementPropVal*))Utilities::Resolve(Games::DashboardHandle, 10080);
	Dashboard_Calls::XUIElementPropVal_SetString = (HRESULT(*)(Dashboard_Structures::XUIElementPropVal*, LPCWSTR))Utilities::Resolve(Games::DashboardHandle, 10094);
	Dashboard_Calls::XUIElementPropVal_SetFloat = (HRESULT(*)(Dashboard_Structures::XUIElementPropVal*, float))Utilities::Resolve(Games::DashboardHandle, 10091);
	Dashboard_Calls::XUIElementPropVal_SetColorFromUint = (VOID(*)(Dashboard_Structures::XUIElementPropVal*, unsigned int))Utilities::Resolve(Games::DashboardHandle, 10092);
	Dashboard_Calls::XUIElementPropVal_SetBool = (void(*)(Dashboard_Structures::XUIElementPropVal* pThis, BOOL val))Utilities::Resolve(Games::DashboardHandle, 10088);
	Dashboard_Calls::XUIElementPropVal_SetUint = (void(*)(Dashboard_Structures::XUIElementPropVal* pThis, unsigned int val))Utilities::Resolve(Games::DashboardHandle, 10090);

	Dashboard_Calls::XUIElementPropVal_SetInt = (HRESULT(*)(Dashboard_Structures::XUIElementPropVal*, int))Utilities::Resolve(Games::DashboardHandle, 10089);
	Dashboard_Calls::XuiObjectGetProperty = (HRESULT(*)(HXUIOBJ, DWORD, DWORD, Dashboard_Structures::XUIElementPropVal*))Utilities::Resolve(Games::DashboardHandle, 10047);
	Dashboard_Calls::XuiObjectSetProperty = (HRESULT(*)(HXUIOBJ, DWORD, DWORD, const Dashboard_Structures::XUIElementPropVal *))Utilities::Resolve(Games::DashboardHandle, 10048);

	Dashboard_Calls::XuiTextElementSetText = (HRESULT(*)(HXUIOBJ, LPCWSTR))Utilities::Resolve(Games::DashboardHandle, 10309);

	Dashboard_Calls::XuiObjectGetPropertyId = (HRESULT(*)(HXUIOBJ, LPCWSTR, DWORD *))Utilities::Resolve(Games::DashboardHandle, 10044);

	Dashboard_Calls::XuiElementGetOpacity = (HRESULT(*)(HXUIOBJ, float*))Utilities::Resolve(Games::DashboardHandle, 10172);
	Dashboard_Calls::XuiElementSetOpacity = (HRESULT(*)(HXUIOBJ, float))Utilities::Resolve(Games::DashboardHandle, 10173);

	Dashboard_Calls::XuiElementGetRotation = (HRESULT(*)(HXUIOBJ, D3DXQUATERNION *))Utilities::Resolve(Games::DashboardHandle, 10170);
	Dashboard_Calls::XuiElementSetRotation = (HRESULT(*)(HXUIOBJ, const D3DXQUATERNION *))Utilities::Resolve(Games::DashboardHandle, 10171);

	Dashboard_Calls::XuiCreateObject = (HRESULT(*)(LPCWSTR, HXUIOBJ *))Utilities::Resolve(Games::DashboardHandle, 10028);
	Dashboard_Calls::XuiElementSetBounds = (HRESULT(*)(HXUIOBJ, float, float))Utilities::Resolve(Games::DashboardHandle, 10165);
	Dashboard_Calls::XuiElementSetPosition = (HRESULT(*)(HXUIOBJ, const D3DXVECTOR3*))Utilities::Resolve(Games::DashboardHandle, 10167);
	Dashboard_Calls::XuiElementGetPosition = (HRESULT(*)(HXUIOBJ, D3DXVECTOR3 *))Utilities::Resolve(Games::DashboardHandle, 10166);
	//Dashboard_Calls::XuiElementSetColorWriteFlags = (HRESULT(*)(HXUIOBJ, DWORD))(IsDevkit ? 0x921DFD20 : 0x921DFBD0); //Get rid of this!
	//Dashboard_Calls::XuiElementGetColorWriteFlags = (HRESULT(*)(HXUIOBJ, DWORD*))(IsDevkit ? 0x921DFD80 : 0x921DFC30); //Get rid of this!

	Dashboard_Calls::XuiElementAddChild = (HRESULT(*)(HXUIOBJ, HXUIOBJ))Utilities::Resolve(Games::DashboardHandle, 10155);
	Dashboard_Calls::XuiElementGetParent = (HRESULT(*)(HXUIOBJ, HXUIOBJ*))Utilities::Resolve(Games::DashboardHandle, 10157);
	Dashboard_Calls::XuiElementGetLastChild = (HRESULT(*)(HXUIOBJ, HXUIOBJ*))Utilities::Resolve(Games::DashboardHandle, 10159);
	Dashboard_Calls::XuiElementGetFirstChild = (HRESULT(*)(HXUIOBJ, HXUIOBJ*))Utilities::Resolve(Games::DashboardHandle, 10158);
	Dashboard_Calls::XuiElementGetNext = (HRESULT(*)(HXUIOBJ, HXUIOBJ*))Utilities::Resolve(Games::DashboardHandle, 10160);
	Dashboard_Calls::XuiElementGetPrev = (HRESULT(*)(HXUIOBJ, HXUIOBJ*))Utilities::Resolve(Games::DashboardHandle, 10161);

	Dashboard_Calls::XuiRegisterClass = (HRESULT(*)(const XUIClass *, HXUICLASS *))Utilities::Resolve(Games::DashboardHandle, 10018);
	Dashboard_Calls::XuiUnregisterClass = (HRESULT(*)(LPCWSTR))Utilities::Resolve(Games::DashboardHandle, 10019);

	Dashboard_Calls::XuiDestroyObject = (HRESULT(*)(HXUIOBJ))Utilities::Resolve(Games::DashboardHandle, 10029);

	Dashboard_Calls::XuiProgressBarSetRange = (HRESULT(*)(HXUIOBJ, INT, INT))Utilities::Resolve(Games::DashboardHandle, 10249);
	Dashboard_Calls::XuiProgressBarSetValue = (HRESULT(*)(HXUIOBJ, INT))Utilities::Resolve(Games::DashboardHandle, 10250);

	Dashboard_Calls::XuiSetColorFactor = (HRESULT(*)(HXUIDC, DWORD))Utilities::Resolve(Games::DashboardHandle, 10420);
	//Dashboard_Calls::XuiGetColorFactor = (HRESULT(*)(HXUIDC, DWORD*))(IsDevkit ? 0x9221D610 : 0x9221D5F8); //Get rid of this!

	Dashboard_Calls::XuiDrawShape = (HRESULT(*)(HXUIDC, HXUISHAPE))Utilities::Resolve(Games::DashboardHandle, 10418);

	Dashboard_Calls::XuiClassGetPropDef = (HRESULT(*)(HXUICLASS, const XUIElementPropDef **, DWORD*))Utilities::Resolve(Games::DashboardHandle, 10022);
	Dashboard_Calls::XuiFindClass = (HXUICLASS(*)(LPCWSTR))Utilities::Resolve(Games::DashboardHandle, 10016);

	Dashboard_Calls::XuiSendMessage = (HRESULT(*)(HXUIOBJ, XUIMessage *))Utilities::Resolve(Games::DashboardHandle, 10033);
	Dashboard_Calls::XuiSceneCreate = (HRESULT(*)(LPCWSTR, LPCWSTR, PVOID, HXUIOBJ*))Utilities::Resolve(Games::DashboardHandle, 10252);

	Dashboard_Calls::XuiSceneNavigateFirst = (HRESULT(*)(HXUIOBJ, HXUIOBJ, BYTE))Utilities::Resolve(Games::DashboardHandle, 10253);
	Dashboard_Calls::XuiSceneNavigateForward = (HRESULT(*)(HXUIOBJ, BOOL, HXUIOBJ, BYTE))Utilities::Resolve(Games::DashboardHandle, 10254);

	Dashboard_Calls::XuiHandleIsValid = (BOOL(*)(HXUIOBJ))Utilities::Resolve(Games::DashboardHandle, 10030);

	Dashboard_Calls::XuiElementSetFocus = (HRESULT(*)(HXUIOBJ, BYTE))Utilities::Resolve(Games::DashboardHandle, 10196);
	Dashboard_Calls::XuiElementGetUserFocus = (HXUIOBJ(*)(BYTE))Utilities::Resolve(Games::DashboardHandle, 10205);

	Dashboard_Calls::XuiGetOuter = (HXUIOBJ(*)(HXUIOBJ))Utilities::Resolve(Games::DashboardHandle, 10036);

	Dashboard_Calls::XuiElementGetScale = (HRESULT(*)(HXUIOBJ, D3DXVECTOR3 *))Utilities::Resolve(Games::DashboardHandle, 10168);
	Dashboard_Calls::XuiElementSetScale = (HRESULT(*)(HXUIOBJ, const D3DXVECTOR3 *))Utilities::Resolve(Games::DashboardHandle, 10169);

	Dashboard_Calls::XuiGetClass = (HRESULT(*)(HXUICLASS, Dashboard_Structures::XUIClass *))Utilities::Resolve(Games::DashboardHandle, 10020);
	Dashboard_Calls::XuiGetBaseClass = (HRESULT(*)(HXUICLASS, HXUICLASS *))Utilities::Resolve(Games::DashboardHandle, 10021);
	Dashboard_Calls::XuiGetObjectClass = (HXUICLASS(*)(HXUIOBJ))Utilities::Resolve(Games::DashboardHandle, 10027);

	Dashboard_Calls::XuiFigureSetFill = (HRESULT(*)(HXUIOBJ, XUI_FILL_TYPE, DWORD, XUIGradientStop*, int, float, const D3DXVECTOR2*, const D3DXVECTOR2*))Utilities::Resolve(Games::DashboardHandle, 10210);
}