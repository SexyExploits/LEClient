#include "stdafx.h"

DWORD BrandColorPress = 0xFF0fb391;

HMODULE Games::DashboardHandle = nullptr;

BOOL Games::TitlePatch::DashLoaded = false;
VOID Games::TitlePatch::InitializeTitlePatching(PLDR_DATA_TABLE_ENTRY ModuleHandle) {
	Utilities::PatchModuleImport(ModuleHandle, MODULE_KERNEL, 0x197, reinterpret_cast<DWORD>(Hooks::XexGetProcedureAddressHook));
	Utilities::PatchModuleImport(ModuleHandle, MODULE_KERNEL, 0x198, reinterpret_cast<DWORD>(Hooks::XexLoadHooks));
	Utilities::PatchModuleImport(ModuleHandle, MODULE_KERNEL, 0x199, reinterpret_cast<DWORD>(Hooks::XexLoadImageHooks));
	Utilities::PatchModuleImport(ModuleHandle, MODULE_XAM, 0x191, reinterpret_cast<DWORD>(Hooks::XamInputGetStateHook));

	auto *execution_id = reinterpret_cast<XEX_EXECUTION_ID*>(RtlImageXexHeaderField(ModuleHandle->XexHeaderBase, 0x00040006));
	if (execution_id == nullptr)
		return;

	//INI::Read();
	//Utilities::PatchModuleImport(MODULE_HUD, MODULE_XAM, 0x357, reinterpret_cast<DWORD>(Xui::XuiSceneCreateHook));
	//Utilities::PatchInJump(reinterpret_cast<PDWORD>(Utilities::ResolveFunction(MODULE_XAM, 0x31B)), reinterpret_cast<DWORD>(Xui::XamBuildResourceLocator), FALSE);

#ifdef DEVELOPERSERVER
	if (wcscmp(ModuleHandle->BaseDllName.Buffer, L"Guide.MP.Purchase.xex") == NULL) {
		BYTE Patch[0x1C] = { 0x38, 0x80, 0x00, 0x05, 0x80, 0x63, 0x00, 0x1C, 0x90, 0x83, 0x00, 0x04, 0x38, 0x80, 0x05, 0x39, 0x90, 0x83, 0x00, 0x08, 0x38, 0x60, 0x00, 0x00, 0x4E, 0x80, 0x00, 0x20 };
		CopyMemory((PDWORD)(KV::IsDevkit ? 0x8169B8A0 : 0x8168A658), Patch, 0x1C); // note update patch above

		*(PDWORD)(KV::IsDevkit ? 0x8190A19C : 0x818E928C) = 0x60000000;
		*(PDWORD)(KV::IsDevkit ? 0x8190EB64 : 0x818EDAD4) = 0x480000C8;
		// Guide.MP.Purchase.xex proab need updating too on devkit
		*(PDWORD)0x9015C15C = 0x39600000;
		*(PDWORD)0x9015C108 = 0x60000000; 
	}
#endif

	/*if (wcscmp(ModuleHandle->BaseDllName.Buffer, L"dash.social.lex") == NULL) {
		if (INI::DashUiModifications) {
			*reinterpret_cast<PDWORD>(0x9AFC4876) = BrandColor;
		}
	}

	if (wcscmp(ModuleHandle->BaseDllName.Buffer, L"dash.search.lex") == NULL) {
		if (INI::DashUiModifications) {
			*reinterpret_cast<PDWORD>(0xC371B948) = BrandColor;
			*reinterpret_cast<PDWORD>(0xC375B948) = BrandColor;
		}
	}

	if (wcscmp(ModuleHandle->BaseDllName.Buffer, L"Dash.Search.xex") == NULL) {
		if (INI::DashUiModifications) {
			*reinterpret_cast<PDWORD>(0x9AD8C9D4) = BrandColor;
			*reinterpret_cast<PDWORD>(0x9AD8CA0C) = BrandColor;
		}
	}

	if (wcscmp(ModuleHandle->BaseDllName.Buffer, L"dash.mp.contentexplorer.lex") == NULL) {
		if (INI::DashUiModifications) {
			// Games | A - Z Buttons
			*reinterpret_cast<PDWORD>(0x9B111169) = BrandColor;
			*reinterpret_cast<PDWORD>(0x9B11CBB8) = BrandColor;
			*reinterpret_cast<PDWORD>(0x9B11DFCF) = BrandColor;
			*reinterpret_cast<PDWORD>(0x9B11F133) = BrandColor;
			*reinterpret_cast<PDWORD>(0x9B11F31A) = BrandColor;
			*reinterpret_cast<PDWORD>(0x9B120CF0) = BrandColor;
			*reinterpret_cast<PDWORD>(0x9B120F1C) = BrandColor;
			*reinterpret_cast<PDWORD>(0x9B121556) = BrandColor;
			*reinterpret_cast<PDWORD>(0x9B1224FA) = BrandColor;
			*reinterpret_cast<PDWORD>(0x9B12271F) = BrandColor;
			*reinterpret_cast<PDWORD>(0x9B1243C7) = BrandColor;
			*reinterpret_cast<PDWORD>(0x9B124609) = BrandColor;
		}
	}
	*/
	if (wcscmp(ModuleHandle->BaseDllName.Buffer, FormatUtils::toWCHAR("%s", MODULE_DASH)) == NULL || wcscmp(ModuleHandle->BaseDllName.Buffer, FormatUtils::toWCHAR("%s", MODULE_XSHELL)) == NULL) {
		DashLoaded = true;
		DashboardHandle = (HMODULE)ModuleHandle;
		//Xui::vDashHome.emplace_back(std::make_pair(std::make_pair("manifest.xboxlive.com", "/manifest/epix/en-US/dashhome.xml"), "/LE-XUI/manifest/epix/en-US/dashhome.xml"));
		//Xui::NetDll_XHttpConnect_Original = reinterpret_cast<Xui::tNetDll_XHttpConnect>(Utilities::HookFunctionStub(Utilities::Resolve(static_cast<HMODULE>(LE::Xamhandle), 0xCD), Xui::NetDll_XHttpConnect_Hook));
		//Xui::NetDll_XHttpSendRequest_Original = reinterpret_cast<Xui::tNetDll_XHttpSendRequest>(Utilities::HookFunctionStub(Utilities::Resolve(static_cast<HMODULE>(LE::Xamhandle), 0xD1), Xui::NetDll_XHttpSendRequest_Hook));
		
		//Xui::DashBuildResourceLocatorStub = reinterpret_cast<Xui::tDashBuildResourceLocator>(Utilities::HookFunctionStub(0x922005B8, Xui::DashBuildResourceLocator));
		//Xui::DashCommandHandlerStub = reinterpret_cast<Xui::tDashCommandHandler>(Utilities::HookFunctionStub(0x92324498, Xui::DashCommandHandlerHook));

		if (INI::DashUiModifications) {
			XuiElementBeginRenderStub = (tXuiElementBeginRender)Utilities::HookFunctionStub(Utilities::Resolve(DashboardHandle, 10451), XuiElementBeginRenderHook);
		}
		/*strcpy(reinterpret_cast<PCHAR>(0x92BAF010), "LiveEmulation  "); // sign in our out 
		strcpy(reinterpret_cast<PCHAR>(0x92BAEFFC), "LiveEmulation.com  "); // Choose your profile
		strcpy(reinterpret_cast<PCHAR>(0x9140D2DC), "LE Home  "); // Xbox Home Button 

		if (INI::DashUiModifications) {
			// DashboardUI
			*reinterpret_cast<PDWORD>(0x92C59F28) = BrandColor; // Home & Games, Movies & TV, Apps + System Settings (dashuisk)
			*reinterpret_cast<PDWORD>(0x92ACBA30) = BrandColor; // Social Offline (soc)
			*reinterpret_cast<PDWORD>(0x92C73B73) = BrandColor; // Networking Still Bubbles (Networking settings)
			*reinterpret_cast<PDWORD>(0x92C59F24) = BrandColorPress; // Button Press (dashuisk)

			// DashboardBG
			*reinterpret_cast<PDWORD>(0x92B1A236) = 0x13; // Remove aura_background.jpg
			*reinterpret_cast<PDWORD>(0x92B50DA7) = 0xff039678; // Our Color | Center
			*reinterpret_cast<PDWORD>(0x92B50DAB) = 0xFF515151; // Our Dark Gradient
			*reinterpret_cast<PDWORD>(0x92B50DAF) = 0xff039678; // Our Color
			*reinterpret_cast<PDWORD>(0x92B50DB3) = 0xFF515151; // Our Dark Gradient
			*reinterpret_cast<PDWORD>(0x92B54D33) = 0xff039678;
			*reinterpret_cast<PDWORD>(0x92B56FA9) = 0xff039678;
			*reinterpret_cast<PDWORD>(0x92B50E03) = 0xff039678;
			*reinterpret_cast<PDWORD>(0x92B50D93) = 0xFF515151; // Our (system settings) Dark Gradient
			*reinterpret_cast<PDWORD>(0x92B50D97) = 0xff039678;
			*reinterpret_cast<PDWORD>(0x92B50D9F) = 0xff039678;
			*reinterpret_cast<PDWORD>(0x92B50E03) = 0xff039678;
			*reinterpret_cast<PDWORD>(0x92BADFC0) = BrandColorPress; // Sign In Text Color
		}
		*/


	}
	else {
		if (wcscmp(ModuleHandle->BaseDllName.Buffer, FormatUtils::toWCHAR("%s", MODULE_HUD)) == NULL) {
			if (INI::HudColorModifications) {
				/* hud skin
				*reinterpret_cast<PDWORD>(0xC1197248) = BrandColor; // xbox Dark Green
				*reinterpret_cast<PDWORD>(0xC1197278) = BrandColorPress; // xbox light green

				*reinterpret_cast<PDWORD>(0xC000B7C2) = BrandColor;
				*reinterpret_cast<PDWORD>(0xC1197339) = BrandColor;
				*reinterpret_cast<PDWORD>(0xC119733D) = BrandColor;
				*reinterpret_cast<PDWORD>(0xC1197341) = BrandColor;
				*reinterpret_cast<PDWORD>(0xC1074394) = BrandColor; // Keyboard Blinker
				*/
			}

			//Utilities::PatchModuleImport(ModuleHandle, MODULE_XAM, 0x34A, reinterpret_cast<DWORD>(Xui::XuiRegisterClassHook));
			//Utilities::PatchModuleImport(ModuleHandle, MODULE_XAM, 0x362, reinterpret_cast<DWORD>(Xui::XuiUnregisterClassHook));
		}

		auto Game = execution_id->TitleID;
		if (LE::Auth_status >= AUTHED) {
			if (Game != NULL) {
				switch (Game) {
				case COD_GHOSTS:
					if (wcscmp(ModuleHandle->BaseDllName.Buffer, MODULE_MPXEX) == NULL) {
						Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(Ghosts::GhostBypass));
					}
					break;
				case COD_AW:
					if (wcscmp(ModuleHandle->BaseDllName.Buffer, MODULE_MPXEX) == NULL) {
						Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(AW::AwBypass));
					}
					break;
				case COD_BO2:
					if (wcscmp(ModuleHandle->BaseDllName.Buffer, MODULE_MPXEX) == NULL) {
						Utilities::StartThread(reinterpret_cast<LPTHREAD_START_ROUTINE>(BO2::BO2Bypass));
					}
					break;
				default:
					break;
				}
			}
		}
	}
}
