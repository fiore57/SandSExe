#pragma once
#include "pch.h"
#include <string>
#include <sstream>

class LLKeyboardHook {
public:
    LLKeyboardHook(const HWND hwnd);
    ~LLKeyboardHook();

    static std::int32_t imeState;
    [[nodiscard]] static std::int32_t getImeState() { return imeState; }
    static void setImeState(const std::int32_t newImeState) {
        imeState = newImeState;
        std::wstringstream ss;
        ss << L"NewImeState: " << newImeState << '\n';
        OutputDebugString(ss.str().c_str());
    }

private:
    const HWND hWnd;
};

