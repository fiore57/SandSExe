#include "pch.hpp"

#include "Hook.hpp"
#include "../SandSExe/Define.hpp"

// すべてのスレッドにセットされるフックになるので
// グローバル変数を共有する必要がある
// 共有セグメント
#pragma data_seg(".shareddata")
HWND g_hWnd = 0;
HINSTANCE g_hInstance = 0;
HHOOK g_hKeyHook = 0;
// IMEの状態
// -1: IME off
// 0: 直接入力
// 正: 日本語入力
std::int32_t g_imeState = 0;
#pragma data_seg()

void enableChangeWindowMessageFilter() {
    ::ChangeWindowMessageFilterEx(g_hWnd, fiore::sands::WM_UPDATE_IME_STATE, MSGFLT_ALLOW, 0);
}
void disableChangeWindowMessageFilter() {
    ::ChangeWindowMessageFilterEx(g_hWnd, fiore::sands::WM_UPDATE_IME_STATE, MSGFLT_RESET, 0);
}

void updateImeState(const std::int32_t newImeState) {
    g_imeState = newImeState;
    ::PostMessage(g_hWnd, fiore::sands::WM_UPDATE_IME_STATE, g_imeState, 0);
}

void Initialize(HINSTANCE hInstance) {
    g_hInstance = hInstance;
}

LRESULT CALLBACK HookProc(
    const int nCode,
    const WPARAM wp,
    const LPARAM lp)
{
    if (nCode != HC_ACTION) {
        return ::CallNextHookEx(nullptr, nCode, wp, lp);
    }

    CWPSTRUCT* pcwp;
    if (wp == 0) {
        pcwp = (CWPSTRUCT*)lp;
        if (pcwp->message == WM_IME_NOTIFY) {
            if (pcwp->wParam == IMN_OPENSTATUSWINDOW ||
                pcwp->wParam == IMN_SETCONVERSIONMODE ||
                pcwp->wParam == IMN_SETOPENSTATUS) {
                // IMEの状態を取得
                const HWND hActiveWnd = ::GetActiveWindow();
                const HWND hWnd = ::ImmGetDefaultIMEWnd(hActiveWnd);
                const HIMC hImm = ::ImmGetContext(hWnd);
                if (!::ImmGetOpenStatus(hImm)) {
                    ::updateImeState(-1);
                    ::ImmReleaseContext(hWnd, hImm);
                    return ::CallNextHookEx(nullptr, nCode, wp, lp);
                }
                // g_imeStateを更新
                DWORD dwConversion, dwSentence;
                ::ImmGetConversionStatus(hImm, &dwConversion, &dwSentence);
                ::updateImeState(dwConversion);
                ::ImmReleaseContext(hWnd, hImm);
            }
        }
    }
    return ::CallNextHookEx(nullptr, nCode, wp, lp);
}


BOOL WINAPI StartHook(HWND hWnd) {
    if (!g_hInstance || !hWnd) {
        return FALSE;
    }
    if (g_hKeyHook) {
        return TRUE;
    }
    g_hWnd = hWnd;
    if (!g_hKeyHook) {
        g_hKeyHook = ::SetWindowsHookEx(WH_CALLWNDPROC, ::HookProc, g_hInstance, 0);
        if (!g_hKeyHook) {
            return FALSE;
        }
    }
    ::enableChangeWindowMessageFilter();
    return TRUE;
}

BOOL WINAPI EndHook() {
    if (!g_hInstance) {
        return FALSE;
    }
    if (g_hKeyHook && !::UnhookWindowsHookEx(g_hKeyHook)) {
        return FALSE;
    }
    g_hKeyHook = 0;
    ::disableChangeWindowMessageFilter();
    ::PostMessage(HWND_BROADCAST, WM_NULL, 0, 0);

    return TRUE;
}
