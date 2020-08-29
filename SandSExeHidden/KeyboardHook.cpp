#include "pch.hpp"

#include "KeyboardHook.hpp"
#include "../SandSDll/KeyboardHook.hpp"

namespace {
#ifndef _WIN64
    // 32bit
    constexpr auto HOOK_DLL_FILE_W = L"SandSDll32.dll";
    constexpr auto HOOK_DLL_FILE_A = "SandSDll32.dll";
#else
    constexpr auto HOOK_DLL_FILE_W = L"SandSDll64.dll";
    constexpr auto HOOK_DLL_FILE_A = "SandSDll64.dll";
#endif
}

KeyboardHook::KeyboardHook(HWND hwnd)
    : hWnd(hwnd), hDll(::LoadLibrary(HOOK_DLL_FILE_W))
{
    if (!hDll) {
        std::stringstream ss;
        ss << "failed to load " << HOOK_DLL_FILE_A;
        throw std::runtime_error(ss.str());
    }
    auto startHook = reinterpret_cast<decltype(::StartHook)*>(::GetProcAddress(hDll, "StartHook"));
    if (!startHook) {
        std::stringstream ss;
        ss << "failed to load StartHook from " << HOOK_DLL_FILE_A;
        throw std::runtime_error(ss.str());
    }
    if (!(*startHook)(hWnd)) {
        std::stringstream ss;
        ss << "failed to start hook in " << HOOK_DLL_FILE_A;
        throw std::runtime_error(ss.str());
    }
}

KeyboardHook::~KeyboardHook()
{
    auto finishHook = reinterpret_cast<decltype(::FinishHook)*>(::GetProcAddress(hDll, "FinishHook"));
    if (finishHook) {
        (*finishHook)();
    }
    ::FreeLibrary(hDll);
}
