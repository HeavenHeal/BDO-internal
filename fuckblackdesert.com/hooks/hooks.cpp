#include "../main.h"
#include "../hooks/function/Present.hpp"

LRESULT CALLBACK FakeWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) { return DefWindowProc(hwnd, uMsg, wParam, lParam); }

bool Hooks() {

	HMODULE hDxgidll = 0;

	while (!hDxgidll) {
		hDxgidll = GetModuleHandleA("dxgi.dll");
		std::cout << "dxgi.dll" << std::endl;
		std::this_thread::sleep_for(15ms);
	}

	IDXGISwapChain* pSwapChain;

	WNDCLASSEXA wc = { sizeof(WNDCLASSEX), CS_CLASSDC, FakeWnd, 0L, 0L, GetModuleHandleA(NULL), NULL, NULL, NULL, NULL, "huyy", NULL };
	RegisterClassExA(&wc);
	HWND hWnd = CreateWindowA("huyy", NULL, WS_OVERLAPPEDWINDOW, 100, 100, 300, 300, NULL, NULL, wc.hInstance, NULL);

	D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1 };
	D3D_FEATURE_LEVEL obtainedLevel;
	ID3D11Device* d3dDevice = nullptr;
	ID3D11DeviceContext* d3dContext = nullptr;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = FindWindowA("BlackDesertWindowClass", 0);
	scd.SampleDesc.Count = 1;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = ((GetWindowLongPtr(hWnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

	// LibOVR 0.4.3 requires that the width and height for the backbuffer is set even if
	// you use windowed mode, despite being optional according to the D3D11 documentation.
	scd.BufferDesc.Width = 1;
	scd.BufferDesc.Height = 1;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	IDXGISwapChain* d3dSwapChain = 0;

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		requestedLevels,
		sizeof(requestedLevels) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION,
		&scd,
		&pSwapChain,
		&pDevice,
		&obtainedLevel,
		&pContext)))
	{
		return false;
	}


	pSwapChainVtable = (DWORD_PTR*)pSwapChain;
	pSwapChainVtable = (DWORD_PTR*)pSwapChainVtable[0];

	pContextVTable = (DWORD_PTR*)pContext;
	pContextVTable = (DWORD_PTR*)pContextVTable[0];

	pDeviceVTable = (DWORD_PTR*)pDevice;
	pDeviceVTable = (DWORD_PTR*)pDeviceVTable[0];
	if (MH_Initialize() != MH_OK) { return false; }
	if (MH_CreateHook((DWORD_PTR*)pSwapChainVtable[8], hookD3D11Present, reinterpret_cast<void**>(&oPresent)) != MH_OK) { return false; }
	if (MH_EnableHook((DWORD_PTR*)pSwapChainVtable[8]) != MH_OK) { return false; }
	DWORD dwOld;
	VirtualProtect(oPresent, 2, PAGE_EXECUTE_READWRITE, &dwOld);
	return true;
}

