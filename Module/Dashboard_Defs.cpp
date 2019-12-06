#include "stdafx.h"

namespace Dashboard_Calls {
	HRESULT(*XuiElementGetId)(HXUIOBJ hObj, LPCWSTR *pszId);
	HRESULT(*XuiElementGetChildById)(HXUIOBJ hObj, LPCWSTR szId, HXUIOBJ *phChild);

	HRESULT(*XuiElementGetParent)(HXUIOBJ hObj, HXUIOBJ* phParent);

	HRESULT(*XuiFigureSetFill)(HXUIOBJ hObj, XUI_FILL_TYPE nFillType, DWORD dwFillColor, XUIGradientStop *pStops, int nNumStops, float fGradientAngle, const D3DXVECTOR2 *pvScale, const D3DXVECTOR2 *pvTrans);

	HRESULT SetFill(HXUIOBJ hObj, DWORD Color1, DWORD Color2) {
		D3DXVECTOR2 pvScalingFactor(1.0f, 1.0f), pTrnas(0.0f, 0.0f);

		XUIGradientStop gs[2];
		gs[0].dwColor = Color1;
		gs[0].fPos = 0.0f;

		gs[1].dwColor = Color2;
		gs[1].fPos = 1.0f;

		return Dashboard_Calls::XuiFigureSetFill(hObj, XUI_FILL_TYPE::XUI_FILL_LINEAR_GRADIENT, Color1, gs, 2, 0, &pvScalingFactor, &pTrnas);
	}
}