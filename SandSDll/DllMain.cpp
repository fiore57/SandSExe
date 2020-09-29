#include "pch.hpp"

void Initialize(HINSTANCE hInstance);

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID)
{
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        // DLL_THREAD_ATTACH / DLL_THREAD_DETACH を無効化
        ::DisableThreadLibraryCalls(hInstance);
        ::Initialize(hInstance);
        return TRUE;
    }
    return TRUE;
}