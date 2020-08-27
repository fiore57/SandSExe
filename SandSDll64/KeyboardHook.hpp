#pragma once
#include "pch.h"

#ifdef _WIN32
    #ifdef EXPORT_
    #define EXPORT_API_ __declspec(dllexport)
    #else
    #define EXPORT_API_ __declspec(dllimport)
    #endif
#else
    #define EXPORT_API_
#endif

class EXPORT_API_ KeyboardHook {
public:
    KeyboardHook(HWND hWnd, DWORD hookingProcessId);
    ~KeyboardHook();

    KeyboardHook() = default;
    KeyboardHook(const KeyboardHook&) = default;
    KeyboardHook(KeyboardHook&&) = default;
    KeyboardHook& operator=(const KeyboardHook&) = default;
    KeyboardHook& operator=(KeyboardHook&&) = default;

private:
    const HWND hWnd = 0;
};

