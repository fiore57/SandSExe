#pragma once
#include "pch.hpp"

class LLKeyboardHook {
public:
    LLKeyboardHook(const HWND hwnd);
    ~LLKeyboardHook();

    static std::int32_t imeState;
    [[nodiscard]] static std::int32_t getImeState() { return imeState; }
    static void setImeState(const std::int32_t newImeState) {
        imeState = newImeState;
#ifndef NDEBUG
        // デバッグ時のみ出力
        std::wstringstream ss;
        ss << L"NewImeState: " << newImeState << '\n';
        OutputDebugString(ss.str().c_str());
#endif
    }

private:
    const HWND hWnd;
    HHOOK hLLKeyHook;
};

