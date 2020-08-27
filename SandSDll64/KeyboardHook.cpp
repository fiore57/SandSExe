#include "pch.h"

#define _EXPORT
#include "KeyboardHook.hpp"

#include <string>

// すべてのスレッドにセットされるフックになるので
// グローバル変数を共有する必要がある
// 共有セグメント
#pragma comment(linker, "/SECTION:.shareddata,RWS")
#pragma data_seg(".shareddata")
HWND g_hWnd = 0;
HHOOK g_hKeyHook = 0;
// IMEの状態
// -1: IME off
// 0: 直接入力
// 正: 日本語入力
std::int32_t g_imeState = 0;
DWORD g_hookingProcessId = 0;
#pragma data_seg

namespace {
    HINSTANCE hInst;
    
    constexpr UINT WM_UPDATE_IME_STATE = WM_USER + 101;

    void updateImeState(const std::int32_t newImeState) {
        g_imeState = newImeState;
        ::PostMessage(g_hWnd, WM_UPDATE_IME_STATE, g_imeState, 0);
    }

    LRESULT CALLBACK KeyHookProc(
        const int nCode,
        const WPARAM wp,
        const LPARAM lp)
    {
        if (nCode < 0) { // 決まり事
            return ::CallNextHookEx(g_hKeyHook, nCode, wp, lp);
        }

        if (::GetCurrentProcessId() == g_hookingProcessId) {
            return ::CallNextHookEx(g_hKeyHook, nCode, wp, lp);
        }

        if (nCode == HC_ACTION) {
            // IMEの状態を取得
            const HWND hActiveWnd = ::GetActiveWindow();
            const HWND hWnd = ::ImmGetDefaultIMEWnd(hActiveWnd);

            const HIMC hImm = ::ImmGetContext(hWnd);
            if (!::ImmGetOpenStatus(hImm)) {
                // IMEの状態を取得できなかった場合
                if (g_imeState != -1) {
                    ::updateImeState(-1);
                }
                ::ImmReleaseContext(hWnd, hImm);
                return ::CallNextHookEx(g_hKeyHook, nCode, wp, lp);
            }
            // g_imeStateを更新
            DWORD dwConversion, dwSentence;
            ::ImmGetConversionStatus(hImm, &dwConversion, &dwSentence);
            if (g_imeState != dwConversion) {
                ::updateImeState(dwConversion);
            }
            ::ImmReleaseContext(hWnd, hImm);
        }

        return ::CallNextHookEx(g_hKeyHook, nCode, wp, lp);
    }
} // namespace

KeyboardHook::KeyboardHook(
    const HWND hwnd,
    const DWORD hookingProcessId)
    : hWnd(hwnd)
{
    g_hWnd = hWnd;
    g_hookingProcessId = hookingProcessId;
    // TODO: WH_KEYBOARD ではなく、 WH_CALLWNDPROC などをフックする
    g_hKeyHook = ::SetWindowsHookEx(
        WH_KEYBOARD, KeyHookProc, hInst, 0);
    if (g_hKeyHook == NULL) {
        MessageBox(hWnd, _T("キーボードフックに失敗しました"), _T("SandS"), MB_ICONERROR | MB_OK);
    }
}

KeyboardHook::~KeyboardHook()
{
    if (!::UnhookWindowsHookEx(g_hKeyHook)) {
        // フック解除失敗
        MessageBox(hWnd, _T("キーボードフックの解除に失敗しました"), _T("SandS"), MB_ICONERROR | MB_OK);
    }
}

// エントリポイント
BOOL APIENTRY DllMain(
    const HMODULE hModule,
    const DWORD ul_reason_for_call,
    const LPVOID /* lpReserved */)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        // アタッチ
        hInst = hModule;
        break;
    case DLL_PROCESS_DETACH:
        // デタッチ
        break;
    }
    return TRUE;
}
