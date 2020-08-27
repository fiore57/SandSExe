#pragma once
#include "pch.h"

class HiddenWindow {
public:
    HiddenWindow(
        LPCTSTR szWindowClass,
        LPCTSTR szTitle,
        HINSTANCE hInstance,
        int nCmdShow);

    [[nodiscard]] HWND getHWnd() const { return hWnd; }

private:
    const HWND hWnd = 0;

public:
    HiddenWindow() = delete;
    ~HiddenWindow();
    HiddenWindow(const HiddenWindow&) = delete;
    HiddenWindow(HiddenWindow&&) = delete;
    HiddenWindow& operator=(const HiddenWindow&) = delete;
    HiddenWindow& operator=(HiddenWindow&&) = delete;
};