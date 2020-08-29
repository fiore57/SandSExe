#pragma once
#include "pch.hpp"

class HiddenWindow {
public:
    HiddenWindow(
        LPCTSTR szWindowClass,
        HINSTANCE hInstance,
        WNDPROC lpfnWndProc);

    [[nodiscard]] HWND getHWnd() const { return hWnd; }

private:
    HWND hWnd = 0;

public:
    HiddenWindow() = delete;
    ~HiddenWindow() = default;
    HiddenWindow(const HiddenWindow&) = delete;
    HiddenWindow(HiddenWindow&&) = delete;
    HiddenWindow& operator=(const HiddenWindow&) = delete;
    HiddenWindow& operator=(HiddenWindow&&) = delete;
};