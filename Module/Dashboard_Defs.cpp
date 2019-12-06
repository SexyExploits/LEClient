#include "stdafx.h"

namespace Dashboard_Calls {
	VOID(*XuiFreeVisuals)(LPCWSTR szPrefix);
	HRESULT(*XuiElementGetId)(HXUIOBJ hObj, LPCWSTR *pszId);
	HRESULT(*XuiElementGetChildById)(HXUIOBJ hObj, LPCWSTR szId, HXUIOBJ *phChild);

	PVOID(*DashXuiAlloc)(size_t size);
	VOID(*DashXuiFree)(PVOID pv);
	VOID(*XUIElementPropVal_Construct)(Dashboard_Structures::XUIElementPropVal* pThis);
	VOID(*XUIElementPropVal_Destruct)(Dashboard_Structures::XUIElementPropVal* pThis);
	HRESULT(*XUIElementPropVal_SetString)(Dashboard_Structures::XUIElementPropVal* pThis, LPCWSTR val);
	HRESULT(*XUIElementPropVal_SetFloat)(Dashboard_Structures::XUIElementPropVal* pThis, float val);
	VOID(*XUIElementPropVal_SetColorFromUint)(Dashboard_Structures::XUIElementPropVal* pThis, unsigned int val);
	void(*XUIElementPropVal_SetBool)(Dashboard_Structures::XUIElementPropVal* pThis, BOOL val);
	void(*XUIElementPropVal_SetUint)(Dashboard_Structures::XUIElementPropVal* pThis, unsigned int val);

	HRESULT(*XUIElementPropVal_SetInt)(Dashboard_Structures::XUIElementPropVal* pThis, int val);
	HRESULT(*XuiObjectGetProperty)(HXUIOBJ hObj, DWORD dwPropId, DWORD dwIndex, Dashboard_Structures::XUIElementPropVal *pVal);
	HRESULT(*XuiObjectSetProperty)(HXUIOBJ hObj, DWORD dwPropId, DWORD dwIndex, const Dashboard_Structures::XUIElementPropVal *pVal);

	HRESULT(*XuiTextElementSetText)(HXUIOBJ hObj, LPCWSTR szText);

	HRESULT(*XuiObjectGetPropertyId)(HXUIOBJ hObj, LPCWSTR szPropName, DWORD *pdwId);

	HRESULT(*XuiElementGetOpacity)(HXUIOBJ hObj, float* pfOpacity);
	HRESULT(*XuiElementSetOpacity)(HXUIOBJ hObj, float fOpacity);

	HRESULT(*XuiElementGetRotation)(HXUIOBJ hObj, D3DXQUATERNION *pquatRot);
	HRESULT(*XuiElementSetRotation)(HXUIOBJ hObj, const D3DXQUATERNION *pquatRot);

	HRESULT(*XuiCreateObject)(LPCWSTR szClassName, HXUIOBJ *phObj);
	HRESULT(*XuiElementSetBounds)(HXUIOBJ hObj, float fWidth, float fHeight);
	HRESULT(*XuiElementSetPosition)(HXUIOBJ hObj, const D3DXVECTOR3 *pvPos);
	HRESULT(*XuiElementGetPosition)(HXUIOBJ hObj, D3DXVECTOR3 *pvPos);
	HRESULT(*XuiElementSetColorWriteFlags)(HXUIOBJ hObj, DWORD dwColorWriteFlags);
	HRESULT(*XuiElementGetColorWriteFlags)(HXUIOBJ hObj, DWORD* pdwColorWriteFlags);

	HRESULT(*XuiElementAddChild)(HXUIOBJ hObj, HXUIOBJ hChild);
	HRESULT(*XuiElementGetParent)(HXUIOBJ hObj, HXUIOBJ* phParent);
	HRESULT(*XuiElementGetLastChild)(HXUIOBJ hObj, HXUIOBJ* phLastChild);
	HRESULT(*XuiElementGetFirstChild)(HXUIOBJ hObj, HXUIOBJ* phFirstChild);
	HRESULT(*XuiElementGetNext)(HXUIOBJ hObj, HXUIOBJ* phNext);
	HRESULT(*XuiElementGetPrev)(HXUIOBJ hObj, HXUIOBJ* phPrev);

	HRESULT(*XuiRegisterClass)(const XUIClass *pClass, HXUICLASS *phClass);
	HRESULT(*XuiUnregisterClass)(LPCWSTR szClassName);

	HRESULT(*XuiDestroyObject)(HXUIOBJ hObj);

	HRESULT(*XuiProgressBarSetRange)(HXUIOBJ hObj, INT nRangeMin, INT nRangeMax);
	HRESULT(*XuiProgressBarSetValue)(HXUIOBJ hObj, INT nValue);

	HRESULT(*XuiSetColorFactor)(HXUIDC hDC, DWORD dwColorFactor);
	HRESULT(*XuiGetColorFactor)(HXUIDC hDC, DWORD* pdwColorFactor);

	HRESULT(*XuiDrawShape)(HXUIDC hDC, HXUISHAPE hShape);

	HRESULT(*XuiClassGetPropDef)(HXUICLASS hClass, const XUIElementPropDef **ppDef, DWORD* pdwCount);
	HXUICLASS(*XuiFindClass)(LPCWSTR szClassName);

	HRESULT(*XuiSendMessage)(HXUIOBJ hObj, XUIMessage *pMessage);
	HRESULT(*XuiSceneCreate)(LPCWSTR szBasePath, LPCWSTR szScenePath, PVOID pvInitData, HXUIOBJ* phScene);

	HRESULT(*XuiSceneNavigateFirst)(HXUIOBJ hParent, HXUIOBJ hFirst, BYTE UserIndex);
	HRESULT(*XuiSceneNavigateForward)(HXUIOBJ hCur, BOOL bStayVisible, HXUIOBJ hFwd, BYTE UserIndex);

	BOOL(*XuiHandleIsValid)(HXUIOBJ hObj);

	HRESULT(*XuiElementSetFocus)(HXUIOBJ hObj, BYTE UserIndex);
	HXUIOBJ(*XuiElementGetUserFocus)(BYTE UserIndex);

	HXUIOBJ(*XuiGetOuter)(HXUIOBJ hObj);

	HRESULT(*XuiElementGetScale)(HXUIOBJ hObj, D3DXVECTOR3 *pvScale);
	HRESULT(*XuiElementSetScale)(HXUIOBJ hObj, const D3DXVECTOR3 *pvScale);

	HRESULT(*XuiGetClass)(HXUICLASS hClass, Dashboard_Structures::XUIClass *pClass);
	HRESULT(*XuiGetBaseClass)(HXUICLASS hClass, HXUICLASS * phBaseClass);
	HXUICLASS(*XuiGetObjectClass)(HXUIOBJ hObj);

	HRESULT(*XuiFigureSetFill)(HXUIOBJ hObj, XUI_FILL_TYPE nFillType, DWORD dwFillColor, XUIGradientStop *pStops, int nNumStops, float fGradientAngle, const D3DXVECTOR2 *pvScale, const D3DXVECTOR2 *pvTrans);

	HRESULT XuiListInsertItems(HXUIOBJ hObj, INT iAtItem, INT cItems) {
		XUIMessage msg;
		XUIMessageInsertItems msgData;
		XuiMessageInsertItems(&msg, &msgData, iAtItem, cItems);
		return Dashboard_Calls::XuiSendMessage(hObj, &msg);
	}

	HRESULT XuiSetSourceText(HXUIOBJ hObj, INT iItem, BOOL bItemData, LPCWSTR szText) {
		XUIMessage msg;
		XUIMessageSetSourceText msgExt;
		XuiMessageSetSourceText(&msg, &msgExt, iItem, 0, bItemData, szText);
		return Dashboard_Calls::XuiSendMessage(hObj, &msg);
	}

	HRESULT XuiSetSourceImage(HXUIOBJ hObj, INT iItem, BOOL bItemData, LPCWSTR szText) {
		XUIMessage msg;
		XUIMessageSetSourceImage msgExt;
		XuiMessageSetSourceImage(&msg, &msgExt, iItem, 0, bItemData, szText, NULL, NULL);
		return Dashboard_Calls::XuiSendMessage(hObj, &msg);
	}

	HRESULT XuiGetSourceText(HXUIOBJ hObj, XUIMessage *Message, INT iItem, BOOL bItemData) {
		XUIMessageGetSourceText msgExt;
		XuiMessageGetSourceText(Message, &msgExt, iItem, 0, bItemData);
		return Dashboard_Calls::XuiSendMessage(hObj, Message);
	}

	HRESULT XuiListDeleteItems(HXUIOBJ hObj, INT iAtItem, INT cItems) {
		XUIMessage msg;
		XUIMessageDeleteItems msgData;
		XuiMessageDeleteItems(&msg, &msgData, iAtItem, cItems);
		return Dashboard_Calls::XuiSendMessage(hObj, &msg);
	}

	INT XuiContainerGetItemCount(HXUIOBJ hObj) {
		XUIMessage msg;
		XUIMessageGetItemCount msgData;
		XuiMessageGetItemCount(&msg, &msgData, XUI_ITEMCOUNT_ALL);
		HRESULT Ret = Dashboard_Calls::XuiSendMessage(hObj, &msg);
		if (FAILED(Ret)) return 0;

		if (msgData.cItems < 0) msgData.cItems = 0;
		return msgData.cItems;
	}

	HRESULT SetFill(HXUIOBJ hObj, DWORD Color1, DWORD Color2) {
		D3DXVECTOR2 pvScalingFactor(1.0f, 1.0f), pTrnas(0.0f, 0.0f);

		XUIGradientStop gs[2];
		gs[0].dwColor = Color1;
		gs[0].fPos = 0.0f;

		gs[1].dwColor = Color2;
		gs[1].fPos = 1.0f;

		return Dashboard_Calls::XuiFigureSetFill(hObj, XUI_FILL_TYPE::XUI_FILL_LINEAR_GRADIENT, Color1, gs, 2, 0, &pvScalingFactor, &pTrnas);
	}

	HRESULT SetProperty(HXUIOBJ Obj, LPCWSTR szPropName, Dashboard_Structures::XUIElementPropVal* propVal) {
		HRESULT result = E_FAIL;

		DWORD propID = 0;
		if (NT_SUCCESS(result = XuiObjectGetPropertyId(Obj, szPropName, &propID)))
			result = XuiObjectSetProperty(Obj, propID, 0, propVal);

		return result;
	}

	HRESULT GetProperty(HXUIOBJ Obj, LPCWSTR szPropName, Dashboard_Structures::XUIElementPropVal* propVal) {
		HRESULT result = E_FAIL;

		DWORD propID = 0;
		if (NT_SUCCESS(result = XuiObjectGetPropertyId(Obj, szPropName, &propID)))
			result = XuiObjectGetProperty(Obj, propID, 0, propVal);

		return result;
	}
}