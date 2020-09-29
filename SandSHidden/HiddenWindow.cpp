#include "pch.hpp"
#include "HiddenWindow.hpp"

namespace {
    bool registerWindowClass(
        const HINSTANCE hInstance,
        LPCTSTR szWindowClass,
        WNDPROC lpfnWndProc)
    {
        WNDCLASSEX wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style          = 0;
        wcex.lpfnWndProc    = lpfnWndProc;
        wcex.cbClsExtra     = 0;
        wcex.cbWndExtra     = 0;
        wcex.hInstance      = hInstance;
        wcex.hIcon          = ::LoadIcon(0, IDI_APPLICATION);
        wcex.hCursor        = ::LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
        wcex.lpszMenuName   = nullptr;
        wcex.lpszClassName  = szWindowClass;
        wcex.hIconSm        = ::LoadIcon(0, IDI_APPLICATION);

        return ::RegisterClassEx(&wcex);
    }

} // namespace

HiddenWindow::HiddenWindow(
    LPCTSTR szWindowClass,
    HINSTANCE hInstance,
    WNDPROC lpfnWndProc)
{
    if (!::registerWindowClass(hInstance, szWindowClass, lpfnWndProc)) {
        throw std::runtime_error("failed to register a window class");
    }

    hWnd = ::CreateWindow(
        szWindowClass,
        nullptr,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
        nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        throw std::runtime_error("failed to create a window");
    }

    ::ShowWindow(hWnd, SW_HIDE);
    ::UpdateWindow(hWnd);
}
