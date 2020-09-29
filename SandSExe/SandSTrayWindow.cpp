#include "SandSTrayWindow.hpp"

#include "resource.h"
#include "Config.hpp"
#include "./Define.hpp"

#include <commctrl.h>

namespace {
    // Message handler for config box
    INT_PTR CALLBACK ConfigProc(
        const HWND hDlg,
        const UINT message,
        const WPARAM wParam,
        const LPARAM lParam)
    {
        switch (message)
        {
        case WM_INITDIALOG:
        {
            { // space待機時間
                const HWND hBar = ::GetDlgItem(hDlg, IDC_SLIDER1);
                ::InitCommonControls();
                ::SendMessage(hBar, TBM_SETRANGE, TRUE, MAKELPARAM(0, 10000));  // 範囲の設定
                ::SendMessage(hBar, TBM_SETTICFREQ, 500, 0);   // 目盛りの増分
                ::SendMessage(hBar, TBM_SETPOS, TRUE, Config::getIns()->getSpaceWaitTimeMs());    // 位置の設定
                ::SendMessage(hBar, TBM_SETPAGESIZE, 0, 100);   // クリック時の移動量

                const auto pos = static_cast<std::int32_t>(
                    ::SendMessage(hBar, TBM_GETPOS, NULL, NULL)); // 現在の値を取得
                std::wstringstream ss;
                ss << pos << _T("ms\n");
                ::SetWindowText(::GetDlgItem(hDlg, IDC_STATIC1), ss.str().c_str());
            }

            { // モード
                const auto curMode = Config::getIns()->getCurMode();
                const HWND hCheckedRadio = ::GetDlgItem(hDlg, static_cast<std::int32_t>(curMode));
                ::SendMessage(hCheckedRadio, BM_SETCHECK, BST_CHECKED, 0);
            }

            return (INT_PTR)TRUE;
        }
        case WM_HSCROLL:
        {
            const HWND hBar = ::GetDlgItem(hDlg, IDC_SLIDER1);
            if (hBar == (HWND)lParam) {
                const auto pos = static_cast<std::int32_t>(::SendMessage(hBar, TBM_GETPOS, NULL, NULL)); // 現在の値を取得
                std::wstringstream ss;
                ss << pos << _T("ms\n");
                ::SetWindowText(::GetDlgItem(hDlg, IDC_STATIC1), ss.str().c_str());
            }
            return (INT_PTR)TRUE;
        }
        case WM_COMMAND:
        {
            if (LOWORD(wParam) == IDOK) {
                // コンフィグのデータを保存する
                { // space待機時間
                    const HWND hBar = ::GetDlgItem(hDlg, IDC_SLIDER1);
                    const auto pos = static_cast<std::int32_t>(::SendMessage(hBar, TBM_GETPOS, NULL, NULL));
                    Config::getIns()->setSpaceWaitTimeMs(pos);
                }

                { // モード
                    for (const auto mode : { 
                        Config::eMode::Off,
                        Config::eMode::JapaneseOnly,
                        Config::eMode::IMEOnOnly,
                        Config::eMode::Always })
                    {
                        if (::SendMessage(
                            ::GetDlgItem(hDlg, static_cast<std::int32_t>(mode)),
                            BM_GETCHECK, 0, 0) == BST_CHECKED) { 
                            Config::getIns()->setCurMode(mode);
                        }
                    }
                }

                ::EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            else if (LOWORD(wParam) == IDCANCEL) {
                ::EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
        }

        }
        return (INT_PTR)FALSE;
    }

    // Message handler for about box.
    INT_PTR CALLBACK AboutProc(
        const HWND hDlg,
        const UINT message,
        const WPARAM wParam,
        const LPARAM /* lParam */)
    {
        switch (message)
        {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
        }
        return (INT_PTR)FALSE;
    }
} // namespace

SandSTrayWindow::SandSTrayWindow(
    const LPCTSTR szWindowClass,
    const LPCTSTR szTitle,
    const HINSTANCE hInstance)
    : TrayWindow(szWindowClass, szTitle, hInstance)
{
    const auto hicon = ::LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
    // notifyIcon(eNotifyIconMessage::Add, hicon, _T("SandS(Enable)"));
    notifyIcon(eNotifyIconMessage::Add, hicon, fiore::sands::WINDOW_TITLE);
}

SandSTrayWindow::~SandSTrayWindow()
{
    notifyIcon(eNotifyIconMessage::Delete, nullptr, _T(""));
}

void SandSTrayWindow::onLButtonClick()
{
    // TODO: ON/OFFの切り替え
    /*
    if (isSandSEnabled) {
        isSandSEnabled = false;
        const HICON hIcon = ::LoadIcon(getHInst(), MAKEINTRESOURCE(IDI_SANDSEXE));
        notifyIcon(eNotifyIconMessage::Modify, hIcon, _T("SandS(Disable)"));
    }
    else {
        isSandSEnabled = true;
        const HICON hIcon = ::LoadIcon(getHInst(), MAKEINTRESOURCE(IDI_TEST));
        notifyIcon(eNotifyIconMessage::Modify, hIcon, _T("SandS(Enable)"));
    }
    */
}

void SandSTrayWindow::onRButtonClick()
{
    POINT pt;
    ::GetCursorPos(&pt);

    const HWND hwnd = getHWnd();
    const HINSTANCE hinst = getHInst();

    const HMENU hMenu = ::LoadMenu(hinst, MAKEINTRESOURCE(IDM_TRAY_R));
    if (!hMenu) {
        // メニューが読み込めなかった
        return;
    }
    const HMENU hPopup = ::GetSubMenu(hMenu, 0);

    ::SetForegroundWindow(hwnd);
    ::TrackPopupMenu(hPopup, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, nullptr);
    ::PostMessage(hwnd, WM_NULL, 0, 0);
}

// 何らかの操作を行った場合、trueを返す
// 何も行わなかった場合、falseを返す
bool SandSTrayWindow::onMenuSelected(
    const WORD wmId)
{
    switch (wmId) {
    case IDM_APP_CONFIG:
        onAppConfig();
        return true;
    case IDM_APP_ABOUT:
        onAppAbout();
        return true;
    case IDM_APP_EXIT:
        onAppExit();
        return true;
    }
    return false;
}

void SandSTrayWindow::onAppConfig()
{
    ::DialogBox(getHInst(), MAKEINTRESOURCE(IDD_CONFIGBOX), getHWnd(), ::ConfigProc);
}

void SandSTrayWindow::onAppAbout()
{
    ::DialogBox(getHInst(), MAKEINTRESOURCE(IDD_ABOUTBOX), getHWnd(), ::AboutProc);
}

void SandSTrayWindow::onAppExit()
{
    ::PostMessage(getHWnd(), WM_CLOSE, 0, 0);
}

