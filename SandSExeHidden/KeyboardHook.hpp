#pragma once
#include "pch.hpp"

class KeyboardHook {
public:
    KeyboardHook(HWND hWnd);
    ~KeyboardHook();

    KeyboardHook() = delete;
    KeyboardHook(const KeyboardHook&) = delete;
    KeyboardHook(KeyboardHook&&) = delete;
    KeyboardHook& operator=(const KeyboardHook&) = delete;
    KeyboardHook& operator=(KeyboardHook&&) = delete;

private:
    const HWND hWnd = 0;
    const HMODULE hDll = 0;
    HHOOK hKeyHook = 0;
};
