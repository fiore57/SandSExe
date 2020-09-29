#include "pch.hpp"
#include "LowLevelKeyboardHook.hpp"

#include "../SandSDll/Hook.hpp"
#include "Config.hpp"

#include <chrono>

std::int32_t LLKeyboardHook::imeState = 0;

namespace {
    bool otherKeyInput = false;

    using system_clock = std::chrono::system_clock;
    std::optional<system_clock::time_point> spacePressedTime;

    LRESULT CALLBACK LowLevelKeyboardProc(
        const int nCode,
        const WPARAM wp,
        const LPARAM lp)
    {
        if (nCode != HC_ACTION) {
            return ::CallNextHookEx(nullptr, nCode, wp, lp);
        }

        const auto curMode = Config::getIns()->getCurMode();
        // off なら何もしない
        if (curMode == Config::eMode::Off) {
            return ::CallNextHookEx(nullptr, nCode, wp, lp);
        }

        KBDLLHOOKSTRUCT* kbs = (KBDLLHOOKSTRUCT*)lp;
        if (kbs->flags & LLKHF_INJECTED) {
            return ::CallNextHookEx(nullptr, nCode, wp, lp);
        }

        // japanese only かつ （IME off または 直接入力）なら何もしない
        if (curMode == Config::eMode::JapaneseOnly && LLKeyboardHook::getImeState() <= 0) {
            return ::CallNextHookEx(nullptr, nCode, wp, lp);
        }
        // IME on only かつ IME off なら何もしない
        if (curMode == Config::eMode::IMEOnOnly && LLKeyboardHook::getImeState < 0) {
            return ::CallNextHookEx(nullptr, nCode, wp, lp);
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
        }
        return ::CallNextHookEx(nullptr, nCode, wp, lp);
    }
}

LLKeyboardHook::LLKeyboardHook(const HWND hwnd)
    : hWnd(hwnd)
{
    hLLKeyHook = ::SetWindowsHookEx(
        WH_KEYBOARD_LL, //フック関数のタイプ
        ::LowLevelKeyboardProc, //フックプロシージャのアドレス
        ::GetModuleHandle(0), //フックプロシージャが入っているインスタンスハンドル
        0); //フックされるスレッド　０ならすべてのスレッド

    if (!hLLKeyHook) {
        throw std::runtime_error("failed to start LowLevelKeyboardHook");
    }
}

LLKeyboardHook::~LLKeyboardHook()
{
    if(hLLKeyHook) ::UnhookWindowsHookEx(hLLKeyHook);
}

