#pragma once
#include "pch.hpp"

namespace fiore::sands {
    const static UINT WM_UPDATE_IME_STATE = ::RegisterWindowMessage(L"WM_UPDATE_IME_STATE");

    constexpr auto CLASS_NAME_HIDDEN_32 = _T("SandSHiddenWindowClass32");
    constexpr auto CLASS_NAME_HIDDEN_64 = _T("SandSHiddenWindowClass64");
    constexpr auto CLASS_NAME_TRAY = _T("SandSTrayWindowClass");

    constexpr auto WINDOW_TITLE = _T("SandS");

    constexpr auto SANDS_HIDDEN_KEY = _T("CallFromSandSExe");
}
