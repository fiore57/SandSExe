#include "pch.hpp"

#include "resource.h"
#include "KeyboardHook.hpp"
#include "HiddenWindow.hpp"
#include "../SandSExe/Define.hpp"

namespace {
#ifndef _WIN64
    // 32bit
    constexpr auto CLASS_NAME = fiore::sands::CLASS_NAME_HIDDEN_32;
#else
    // 64bit
    constexpr auto CLASS_NAME = fiore::sands::CLASS_NAME_HIDDEN_64;
#endif

    std::optional<HiddenWindow> hiddenWnd;
    std::optional<KeyboardHook> kbh;

    LRESULT CALLBACK WndProc(
        const HWND hWnd,
        const UINT message,
        const WPARAM wParam,
        const LPARAM lParam)
    {
        if (message == fiore::sands::WM_UPDATE_IME_STATE) {
            ::PostMessage(
                ::FindWindow(fiore::sands::CLASS_NAME_TRAY, 0),
                fiore::sands::WM_UPDATE_IME_STATE, wParam, lParam);
            return 0;
        }
        switch (message)
        {
        case WM_DESTROY:
            ::PostQuitMessage(0);
            break;
        default:
            return ::DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
    }
}

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE /* hPrevInstance */,
	_In_ LPWSTR /* lpCmdLine */,
	_In_ int /* nCmdShow */)
{
    { // 多重起動防止
        HWND hWnd = ::FindWindow(CLASS_NAME, 0);
        if (hWnd) {
            ::SendMessage(hWnd, WM_CLOSE, 0, 0);
        }
    }

    try {
        // ウィンドウ
        hiddenWnd.emplace(CLASS_NAME, hInstance, ::WndProc);
        // フック
        kbh.emplace(hiddenWnd->getHWnd());
    }
    catch (const std::runtime_error& e) {
        std::stringstream ss;
        ss << "runtime_error: " << e.what() << '\n';
        std::string s = ss.str();
        ::MessageBox(nullptr, std::wstring(s.begin(), s.end()).c_str(), _T("Error"), MB_OK);
        ::exit(1);
    }

	HACCEL hAccelTable = ::LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SANDSEXE));

    // Main message loop
    MSG msg;
    while (::GetMessage(&msg, nullptr, 0, 0))
    {
        if (!::TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    return static_cast<int>(msg.wParam);
}

