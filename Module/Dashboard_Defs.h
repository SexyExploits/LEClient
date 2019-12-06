#pragma once
#include "stdafx.h"

namespace Dashboard_Structures {
	struct XUIElementPropVal {
		XUI_ELEMENT_PROPERTY_TYPE type;
		union {
			BOOL bVal;
			int nVal;
			unsigned int uVal;
			float fVal;
			WCHAR *szVal;
			D3DVECTOR vecVal;
			XUIQuaternion quatVal;
			IXUIPropObj *pObj;
			struct {
				void *pvCustomData;
				XUICustomPropHandler *pHandler;
			} CustomVal;
		};

#ifdef __cplusplus
		XUIElementPropVal() {
			((void(*)(...))(Utilities::Resolve(Games::DashboardHandle, 10079)))(this);
		}
		~XUIElementPropVal() {
			((void(*)(...))(Utilities::Resolve(Games::DashboardHandle, 10080)))(this);
		}
#endif
	};

	struct XUIElementPropDef {
		DWORD dwFlags;
		DWORD dwId;
		DWORD dwOffset;
		DWORD dwExtra;
		LPCWSTR szPropName;
		XUI_ELEMENT_PROPERTY_TYPE Type;
		const XUIElementPropVal *pDefaultVal;
		XUICustomPropHandler *pCustomHandler;
		PFN_GETPROP pfnGetProp;
		PFN_SETPROP pfnSetProp;
		PFN_GETPROPDEF pfnGetPropDef;
		PFN_GETCOUNT pfnGetCount;

#ifndef _XBOX
		LPCWSTR szPropertyEditor;
#endif
	};

	struct XUIClass {
		DWORD cbSize;
		LPCWSTR szClassName;
		LPCWSTR szBaseClassName;
		LPCWSTR szClassDetails;
		XUIObjectMethods Methods;
		const XUIElementPropDef *pPropDefs;
		DWORD dwPropDefCount;
		BOOL bExtensionClass;
	};
}

namespace Dashboard_Calls {
	extern VOID(*XuiFreeVisuals)(LPCWSTR szPrefix);
	extern HRESULT(*XuiElementGetId)(HXUIOBJ hObj, LPCWSTR *pszId);
	extern HRESULT(*XuiElementGetChildById)(HXUIOBJ hObj, LPCWSTR szId, HXUIOBJ *phChild);

	extern PVOID(*DashXuiAlloc)(size_t size);
	extern VOID(*DashXuiFree)(PVOID pv);
	extern VOID(*XUIElementPropVal_Construct)(Dashboard_Structures::XUIElementPropVal* pThis);
	extern VOID(*XUIElementPropVal_Destruct)(Dashboard_Structures::XUIElementPropVal* pThis);
	extern HRESULT(*XUIElementPropVal_SetString)(Dashboard_Structures::XUIElementPropVal* pThis, LPCWSTR val);
	extern HRESULT(*XUIElementPropVal_SetFloat)(Dashboard_Structures::XUIElementPropVal* pThis, float val);
	extern VOID(*XUIElementPropVal_SetColorFromUint)(Dashboard_Structures::XUIElementPropVal* pThis, unsigned int val);
	extern void(*XUIElementPropVal_SetBool)(Dashboard_Structures::XUIElementPropVal* pThis, BOOL val);
	extern void(*XUIElementPropVal_SetUint)(Dashboard_Structures::XUIElementPropVal* pThis, unsigned int val);

	extern HRESULT(*XUIElementPropVal_SetInt)(Dashboard_Structures::XUIElementPropVal* pThis, int val);
	extern HRESULT(*XuiObjectGetProperty)(HXUIOBJ hObj, DWORD dwPropId, DWORD dwIndex, Dashboard_Structures::XUIElementPropVal *pVal);
	extern HRESULT(*XuiObjectSetProperty)(HXUIOBJ hObj, DWORD dwPropId, DWORD dwIndex, const Dashboard_Structures::XUIElementPropVal *pVal);

	extern HRESULT(*XuiTextElementSetText)(HXUIOBJ hObj, LPCWSTR szText);

	extern HRESULT(*XuiObjectGetPropertyId)(HXUIOBJ hObj, LPCWSTR szPropName, DWORD *pdwId);

	extern HRESULT(*XuiElementGetOpacity)(HXUIOBJ hObj, float* pfOpacity);
	extern HRESULT(*XuiElementSetOpacity)(HXUIOBJ hObj, float fOpacity);

	extern HRESULT(*XuiElementGetRotation)(HXUIOBJ hObj, D3DXQUATERNION *pquatRot);
	extern HRESULT(*XuiElementSetRotation)(HXUIOBJ hObj, const D3DXQUATERNION *pquatRot);

	extern HRESULT(*XuiCreateObject)(LPCWSTR szClassName, HXUIOBJ *phObj);
	extern HRESULT(*XuiElementSetBounds)(HXUIOBJ hObj, float fWidth, float fHeight);
	extern HRESULT(*XuiElementSetPosition)(HXUIOBJ hObj, const D3DXVECTOR3 *pvPos);
	extern HRESULT(*XuiElementGetPosition)(HXUIOBJ hObj, D3DXVECTOR3 *pvPos);
	extern HRESULT(*XuiElementSetColorWriteFlags)(HXUIOBJ hObj, DWORD dwColorWriteFlags);
	extern HRESULT(*XuiElementGetColorWriteFlags)(HXUIOBJ hObj, DWORD* pdwColorWriteFlags);

	extern HRESULT(*XuiElementAddChild)(HXUIOBJ hObj, HXUIOBJ hChild);
	extern HRESULT(*XuiElementGetParent)(HXUIOBJ hObj, HXUIOBJ* phParent);
	extern HRESULT(*XuiElementGetLastChild)(HXUIOBJ hObj, HXUIOBJ* phLastChild);
	extern HRESULT(*XuiElementGetFirstChild)(HXUIOBJ hObj, HXUIOBJ* phFirstChild);
	extern HRESULT(*XuiElementGetNext)(HXUIOBJ hObj, HXUIOBJ* phNext);
	extern HRESULT(*XuiElementGetPrev)(HXUIOBJ hObj, HXUIOBJ* phPrev);

	extern HRESULT(*XuiRegisterClass)(const XUIClass *pClass, HXUICLASS *phClass);
	extern HRESULT(*XuiUnregisterClass)(LPCWSTR szClassName);

	extern HRESULT(*XuiDestroyObject)(HXUIOBJ hObj);

	extern HRESULT(*XuiProgressBarSetRange)(HXUIOBJ hObj, INT nRangeMin, INT nRangeMax);
	extern HRESULT(*XuiProgressBarSetValue)(HXUIOBJ hObj, INT nValue);

	extern HRESULT(*XuiSetColorFactor)(HXUIDC hDC, DWORD dwColorFactor);
	extern HRESULT(*XuiGetColorFactor)(HXUIDC hDC, DWORD* pdwColorFactor);

	extern HRESULT(*XuiDrawShape)(HXUIDC hDC, HXUISHAPE hShape);

	extern HRESULT(*XuiClassGetPropDef)(HXUICLASS hClass, const XUIElementPropDef **ppDef, DWORD* pdwCount);
	extern HXUICLASS(*XuiFindClass)(LPCWSTR szClassName);

	extern HRESULT(*XuiSendMessage)(HXUIOBJ hObj, XUIMessage *pMessage);
	extern HRESULT(*XuiSceneCreate)(LPCWSTR szBasePath, LPCWSTR szScenePath, PVOID pvInitData, HXUIOBJ* phScene);

	extern HRESULT(*XuiSceneNavigateFirst)(HXUIOBJ hParent, HXUIOBJ hFirst, BYTE UserIndex);
	extern HRESULT(*XuiSceneNavigateForward)(HXUIOBJ hCur, BOOL bStayVisible, HXUIOBJ hFwd, BYTE UserIndex);

	extern BOOL(*XuiHandleIsValid)(HXUIOBJ hObj);

	extern HRESULT(*XuiElementSetFocus)(HXUIOBJ hObj, BYTE UserIndex);
	extern HXUIOBJ(*XuiElementGetUserFocus)(BYTE UserIndex);

	extern HXUIOBJ(*XuiGetOuter)(HXUIOBJ hObj);

	extern HRESULT(*XuiElementGetScale)(HXUIOBJ hObj, D3DXVECTOR3 *pvScale);
	extern HRESULT(*XuiElementSetScale)(HXUIOBJ hObj, const D3DXVECTOR3 *pvScale);

	extern HRESULT(*XuiGetClass)(HXUICLASS hClass, Dashboard_Structures::XUIClass *pClass);
	extern HRESULT(*XuiGetBaseClass)(HXUICLASS hClass, HXUICLASS * phBaseClass);
	extern HXUICLASS(*XuiGetObjectClass)(HXUIOBJ hObj);

	extern HRESULT(*XuiFigureSetFill)(HXUIOBJ hObj, XUI_FILL_TYPE nFillType, DWORD dwFillColor, XUIGradientStop *pStops, int nNumStops, float fGradientAngle, const D3DXVECTOR2 *pvScale, const D3DXVECTOR2 *pvTrans);

	extern HRESULT XuiListInsertItems(HXUIOBJ hObj, INT iAtItem, INT cItems);

	extern HRESULT XuiSetSourceText(HXUIOBJ hObj, INT iItem, BOOL bItemData, LPCWSTR szText);

	extern HRESULT XuiSetSourceImage(HXUIOBJ hObj, INT iItem, BOOL bItemData, LPCWSTR szText);

	extern HRESULT XuiGetSourceText(HXUIOBJ hObj, XUIMessage *Message, INT iItem, BOOL bItemData);

	extern HRESULT XuiListDeleteItems(HXUIOBJ hObj, INT iAtItem, INT cItems);

	extern INT XuiContainerGetItemCount(HXUIOBJ hObj);

	extern HRESULT SetFill(HXUIOBJ hObj, DWORD Color1, DWORD Color2);

	extern HRESULT SetProperty(HXUIOBJ Obj, LPCWSTR szPropName, Dashboard_Structures::XUIElementPropVal* propVal);

	extern HRESULT GetProperty(HXUIOBJ Obj, LPCWSTR szPropName, Dashboard_Structures::XUIElementPropVal* propVal);
}