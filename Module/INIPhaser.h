#pragma once
#include "stdafx.h"

class INI {
public:

	
	// LiveEnvironment
	static BOOL EnableBlockXblDns;

	// cheats
	static BOOL MW3Engine;

	// ui
	static BOOL CustomNotification;
	static BOOL LEImage;
	static BOOL DrawGuideInformation;
	static BOOL Gradient;
	static BOOL GradientBG;
	static BOOL GradientBtns;
	static BOOL ColorFade;

	// bypass
	static BOOL GhostBypass;
	static BOOL AwBypass;
	static BOOL BO2Bypass;
	static BOOL B03Bypass;

	// console
	static BOOL MachineIDSpoofing;

	static VOID Init();
	static VOID Read();
	static VOID Create();
	static VOID UpdateSettings();
};
