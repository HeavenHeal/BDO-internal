#pragma once
#include "../../main.h"

typedef HRESULT(__stdcall *PresentFn) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
PresentFn oPresent;

ID3D11Device *pDevice = NULL;
ID3D11DeviceContext *pContext = NULL;
ID3D11InputLayout *pLayout = NULL;            
ID3D11VertexShader *pVS = NULL;            
ID3D11PixelShader *pPS = NULL;             
ID3D11Buffer *pVBuffer = NULL;              
DWORD_PTR* pSwapChainVtable = NULL;
DWORD_PTR* pContextVTable = NULL;
DWORD_PTR* pDeviceVTable = NULL;
bool first_time = true;
bool open = false;
struct VERTEX { FLOAT X, Y, Z; D3DXCOLOR Color; };

WNDPROC	oWndProc;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYDOWN) {
		if (wParam == VK_INSERT)
			open = !open;
	}

	if (open) {
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
			ImGui::GetIO().MouseDown[0] = true;
			break;
		case WM_LBUTTONUP:
			ImGui::GetIO().MouseDown[0] = false;
			break;
		case WM_RBUTTONDOWN:
			ImGui::GetIO().MouseDown[1] = true;
			break;
		case WM_RBUTTONUP:
			ImGui::GetIO().MouseDown[1] = false;
			break;
		case WM_MBUTTONDOWN:
			ImGui::GetIO().MouseDown[2] = true;
			break;
		case WM_MBUTTONUP:
			ImGui::GetIO().MouseDown[2] = false;
			break;
		case WM_MOUSEWHEEL:
			ImGui::GetIO().MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) > 0 ? +1.0f : -1.0f;
			break;
		case WM_MOUSEMOVE:
			ImGui::GetIO().MousePos.x = (signed short)(lParam); ImGui::GetIO().MousePos.y = (signed short)(lParam >> 16);
			break;
		}
	}

	return CallWindowProc(oWndProc, hwnd, uMsg, wParam, lParam);
}

CLocalPlayer* pLocalPlayer = nullptr;

HRESULT __stdcall hookD3D11Present(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (first_time)
	{
		first_time = false;

		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void **)&pDevice)))
		{
			pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
			pDevice->GetImmediateContext(&pContext);
			ImGui_ImplDX11_Init(FindWindowA("BlackDesertWindowClass", 0), pDevice, pContext);
			oWndProc = (WNDPROC)SetWindowLongPtr(FindWindowA("BlackDesertWindowClass", 0), GWLP_WNDPROC, (__int3264)(LONG_PTR)WndProc);

			auto style = ImGui::GetStyle();
			style.AntiAliasedLines = true;
			style.AntiAliasedFill = true;
			style.FramePadding = ImVec2(0, 0);
			style.ScrollbarSize = 5.f;

			style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		}

	}

	if(!pLocalPlayer)
		pLocalPlayer = (CLocalPlayer*)*(uint64_t*)offys.dw_localplayer;
	if (pLocalPlayer) {
		unsigned long OldProtection;
		VirtualProtect((LPVOID)((offys.dw_clientspeedac)), 1, PAGE_EXECUTE_READWRITE, &OldProtection); //disable velocity checks
		memcpy((LPVOID)((offys.dw_clientspeedac)), "\xC3", 1);
		VirtualProtect((LPVOID)((offys.dw_clientspeedac)), 1, OldProtection, NULL);
		unsigned long OldProtection2;
		VirtualProtect((LPVOID)((offys.dw_clientattackac)), 1, PAGE_EXECUTE_READWRITE, &OldProtection2); //disable attack checks
		memcpy((LPVOID)((offys.dw_clientattackac)), "\xC3", 1);
		VirtualProtect((LPVOID)((offys.dw_clientattackac)), 1, OldProtection2, NULL);
		if(g_movingspeed)
			pLocalPlayer->speedbuff = 2147483647;
		else{
			pLocalPlayer->speedbuff = 90000;
		}
		if (g_attackspeed) {
			pLocalPlayer->attackbuff = 1111111;
			pLocalPlayer->castbuff = 1111111;
		}
		else {
			pLocalPlayer->attackbuff = 50000;
			pLocalPlayer->castbuff = 50000;
		}
	}

	*(float*)offys.f_cam_max_dist = g_cameradist;

	if (open) {
		ImGui_ImplDX11_NewFrame();
		ImGui::SetNextWindowSize(ImVec2(600, 250), ImGuiSetCond_FirstUseEver);
		if (ImGui::Begin("gideonhack.pw", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings)) {

			ImGui::BeginChild("hacks"); {

				if (pLocalPlayer) {
					std::wstring wstr(pLocalPlayer->name);
					std::string str(wstr.begin(), wstr.end());
					ImGui::Text("Hello: %s", str.c_str());
				}

				ImGui::SliderFloat("maximum camera dist", &g_cameradist, 70.f, 10000.f);
				ImGui::Checkbox("speed hack", &g_movingspeed);
				ImGui::Checkbox("attack speed hack", &g_attackspeed);

			} ImGui::EndChild();

			ImGui::End();
		}
		ImGui::Render();
	}
	return oPresent(pSwapChain, SyncInterval, Flags);
}
