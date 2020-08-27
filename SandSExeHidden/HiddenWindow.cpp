#include "pch.h"
#include "HiddenWindow.hpp"

#include <stdexcept>

HiddenWindow::HiddenWindow(
    LPCTSTR szWindowClass,
    LPCTSTR szTitle,
    const HINSTANCE hInstance,
    const int nCmdShow)
    : hWnd(
        ::CreateWindowEx(
            0,
            szWindowClass,
            szTitle,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
            nullptr, nullptr, hInstance, nullptr)
    )
{
    if (!hWnd) {
        throw std::runtime_error("failed to create a window");
    }
    ::ShowWindow(hWnd, SW_HIDE);
    ::UpdateWindow(hWnd);

    ::MessageBox(hWnd, _T("Created Hidden Window!"), _T("Info"), MB_OK);
}

HiddenWindow::~HiddenWindow()
{
    ::MessageBox(hWnd, _T("Destroyed Hidden Window!"), _T("Info"), MB_OK);
}
