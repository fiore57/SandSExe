#include "pch.hpp"
#include "TrayWindow.hpp"

#include <shellapi.h>
#include <cassert>

namespace {
    constexpr std::int32_t ID_ICON = 100;
    constexpr std::int32_t ID_CLICKTIMER = 4;
}

TrayWindow::TrayWindow(
    LPCTSTR szWindowClass,
    LPCTSTR szTitle,
    const HINSTANCE hInstance)
    : hWnd(
        ::CreateWindowEx(
            WS_EX_TOOLWINDOW, // ツールウィンドウ
            szWindowClass,
            szTitle,
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 0, 0,
            nullptr, nullptr, hInstance, nullptr
        )
    )
    , hInst(hInstance)
{
    if (!hWnd) {
        throw std::runtime_error("failed to create a window");
    }

    ::ShowWindow(hWnd, SW_HIDE);
    ::UpdateWindow(hWnd);
}

void TrayWindow::notifyIcon(
    const eNotifyIconMessage nim,
    const HICON hicon,
    const LPCTSTR pszTip)
{
    NOTIFYICONDATA nid = {};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hWnd;
    nid.uID = ID_ICON;

    nid.uCallbackMessage = WM_USER_TRAYNOTIFYICON;
    nid.uFlags = NIF_MESSAGE;

    if (hicon != nullptr && TrayWindow::hIcon != hicon) {
        // アイコン変更
        nid.uFlags |= NIF_ICON;
        nid.hIcon = hicon;
        TrayWindow::hIcon = hicon;
    }

    if (pszTip != nullptr) {
        // ツールチップ表示
        nid.uFlags |= NIF_TIP;
        // 注意：配列の要素数を求めるときは、sizeof(配列) / sizeof(配列の要素) を使う
        _tcsncpy_s(nid.szTip, sizeof(nid.szTip) / sizeof(nid.szTip[0]), pszTip, _TRUNCATE);
    }

    const DWORD dwMessage = [nim]() {
        switch (nim) {
        case eNotifyIconMessage::Add: return NIM_ADD;
        case eNotifyIconMessage::Modify: return NIM_MODIFY;
        case eNotifyIconMessage::Delete: return NIM_DELETE;
        }
        assert(0);
        return -1;
    }();

    if (!Shell_NotifyIcon(dwMessage, &nid)) {
        throw std::runtime_error("failed to update notifyicon");
    }
}

void TrayWindow::onTrayNotify(
    const UINT wParam,
    const LONG lParam)
{
    const UINT uiIconId = static_cast<UINT>(wParam);
    const UINT uiMouseMsg = static_cast<UINT>(lParam);

    if (ID_ICON != uiIconId) return;

    switch (uiMouseMsg)
    {
    case WM_LBUTTONUP:
        onLButtonClick();
        break;
    case WM_RBUTTONUP:
        onRButtonClick();
        break;
    }
}
