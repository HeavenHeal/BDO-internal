#include "main.h"

float g_cameradist = 900.f;
bool g_movingspeed = false;
bool g_attackspeed = false;;

void __stdcall Main(LPVOID pParam) {

	AllocConsole();
	freopen("CON", "w", stdout);
	
	CollectOffsets();
	Hooks();
}

bool __stdcall DllMain(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
){
	if (fdwReason == DLL_PROCESS_ATTACH) {
		CreateRemoteThread(GetCurrentProcess(), 0, 0, (LPTHREAD_START_ROUTINE)Main, hinstDLL, 0, 0);
		return true;
	}
	return false;
}


