#include "pch.hpp"

void Initialize(HINSTANCE hInstance);

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID) {
	switch(dwReason) {
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hInstance);
		Initialize(hInstance);
		return TRUE;

	default:
		break;
	}
	return TRUE;
}