#pragma once
#include "stdafx.h"

typedef HRESULT(*tXuiElementBeginRender)(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct);
extern tXuiElementBeginRender XuiElementBeginRenderStub;

extern HRESULT XuiElementBeginRenderHook(HXUIOBJ hObj, XUIMessageRender *pRenderData, XUIRenderStruct *pRenderStruct);

extern VOID ResolveFunctions();