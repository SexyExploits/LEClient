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
	Dashboard_Calls::XuiElementGetId = (HRESULT(*)(HXUIOBJ, LPCWSTR*))Utilities::Resolve(Games::DashboardHandle, 10176);
	Dashboard_Calls::XuiElementGetChildById = (HRESULT(*)(HXUIOBJ, LPCWSTR, HXUIOBJ*))Utilities::Resolve(Games::DashboardHandle, 10177);

	Dashboard_Calls::XuiElementGetParent = (HRESULT(*)(HXUIOBJ, HXUIOBJ*))Utilities::Resolve(Games::DashboardHandle, 10157);

	Dashboard_Calls::XuiFigureSetFill = (HRESULT(*)(HXUIOBJ, XUI_FILL_TYPE, DWORD, XUIGradientStop*, int, float, const D3DXVECTOR2*, const D3DXVECTOR2*))Utilities::Resolve(Games::DashboardHandle, 10210);
}