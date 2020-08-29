#include "pch.h"
#include "LowLevelKeyboardHook.hpp"
#include "../SandSDll/KeyboardHook.hpp"
#include "Config.hpp"
#include <sstream>
#include <chrono>
#include <optional>

std::int32_t LLKeyboardHook::imeState = 0;

namespace {
    HHOOK g_hLLKeyHook;

    bool otherKeyInput = false;

    using system_clock = std::chrono::system_clock;
    std::optional<system_clock::time_point> spacePressedTime;

    LRESULT CALLBACK LowLevelKeyboardProc(
        const int nCode,
        const WPARAM wp,
        const LPARAM lp)
    {
        const auto curMode = Config::getIns()->getCurMode();
        // off なら何もしない
        if (curMode == Config::eMode::Off) {
            return ::CallNextHookEx(nullptr, nCode, wp, lp);
        }

        KBDLLHOOKSTRUCT* kbs = (KBDLLHOOKSTRUCT*)lp;
        if (kbs->flags & LLKHF_INJECTED) {
            return ::CallNextHookEx(nullptr, nCode, wp, lp);
        }


        std::wstringstream ss;
        ss << L"IME: " << LLKeyboardHook::getImeState() << L'\n';
        OutputDebugString(ss.str().c_str());

        // japanese only かつ （IME off または 直接入力）なら何もしない
        if (curMode == Config::eMode::JapaneseOnly && LLKeyboardHook::getImeState() <= 0) {
            return CallNextHookEx(nullptr, nCode, wp, lp);
        }
        // IME on only かつ IME off なら何もしない
        if (curMode == Config::eMode::IMEOnOnly && LLKeyboardHook::getImeState < 0) {
            return CallNextHookEx(nullptr, nCode, wp, lp);
        }

        INPUT input;
        input.type = INPUT_KEYBOARD;
        input.ki.dwFlags = (wp == WM_KEYDOWN || wp == WM_SYSKEYDOWN) ? 0 : KEYEVENTF_KEYUP;
        
        if (kbs->vkCode == VK_SPACE) {
            if (wp == WM_KEYDOWN) {
                if (spacePressedTime.has_value()) {
                    // spaceが押下状態
                    auto spacePressingDuration = system_clock::now() - spacePressedTime.value();
                    std::chrono::milliseconds spaceWaitTimeMs(Config::getIns()->getSpaceWaitTimeMs());
                    if (spacePressingDuration > spaceWaitTimeMs) {
                        // 長押しされている場合、普通にspaceを出力
                        input.ki.wVk = VK_SPACE;
                        ::SendInput(1, &input, sizeof(input));
                    }
                    return -1;
                }
                else {
                    // spaceキーが押下状態でない場合、shiftを押下
                    spacePressedTime = system_clock::now();

                    input.ki.wVk = VK_SHIFT;
                    ::SendInput(1, &input, sizeof(input));

                    return -1;
                }
            }
            else if (wp == WM_KEYUP) {
                spacePressedTime.reset();
                if (otherKeyInput) {
                    // 他のキー入力があった場合、shiftを離す
                    otherKeyInput = false;
                    input.ki.wVk = VK_SHIFT;
                    ::SendInput(1, &input, sizeof(input));
                    return -1;
                }
                else {
                    // shiftを離し、spaceを入力する
                    input.ki.wVk = VK_SHIFT;
                    input.ki.dwFlags = KEYEVENTF_KEYUP;
                    ::SendInput(1, &input, sizeof(input));

                    input.ki.wVk = VK_SPACE;
                    input.ki.dwFlags = 0;
                    ::SendInput(1, &input, sizeof(input));

                    input.ki.dwFlags = KEYEVENTF_KEYUP;
                    ::SendInput(1, &input, sizeof(input));

                    return -1;
                }
            }
        }
        else {
            if (spacePressedTime.has_value() && wp == WM_KEYDOWN) {
                spacePressedTime.reset();
                otherKeyInput = true;
            }
            return ::CallNextHookEx(nullptr, nCode, wp, lp);
        }
    }
}


LLKeyboardHook::LLKeyboardHook(const HWND hwnd)
    : hWnd(hwnd)
{
    HINSTANCE hInst;

#ifndef _WIN64
    // 32bit
    hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
#else
    // 64bit
    hInst = (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE);
#endif

    g_hLLKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, //フック関数のタイプ
        (HOOKPROC)LowLevelKeyboardProc, //フックプロシージャのアドレス
        hInst, //フックプロシージャが入っているインスタンスハンドル
        0); //フックされるスレッド　０ならすべてのスレッド
    if (g_hLLKeyHook == NULL) {
        ::MessageBox(hWnd, L"フック開始に失敗", L"Error", MB_OK);
        throw "Error";
    }
}

LLKeyboardHook::~LLKeyboardHook()
{
    if (!UnhookWindowsHookEx(g_hLLKeyHook)) {
        ::MessageBox(hWnd, L"フック解除に失敗", L"Error", MB_OK);
    }
}

