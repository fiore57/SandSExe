// SandSExe.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "pch.h"
#include "resource.h"
#include "../SandSDll64/KeyboardHook.hpp"
#include "HiddenWindow.hpp"

#include <cstdlib>
#include <sstream>
#include <string>
#include <memory>
#include <optional>

#pragma comment(lib, "SandSDll64.lib")

namespace {
    constexpr int MAX_LOADSTRING = 100;
    constexpr UINT WM_UPDATE_IME_STATE = WM_USER + 101;

    // グローバル変数
    HINSTANCE hInst;                                // 現在のインターフェイス
    WCHAR szTitle[MAX_LOADSTRING]/* = L"SandS"*/;             // タイトルバーのテキスト

    LPCTSTR CLASS_NAME_32 = _T("SandSWindowClass32");
    LPCTSTR CLASS_NAME_64 = _T("SandSWindowClass64");
    LPCTSTR CLASS_NAME_TRAY = CLASS_NAME_32;
    LPCTSTR CLASS_NAME_HIDDEN = CLASS_NAME_64;

    std::optional<HiddenWindow> hiddenWnd;
}

// 前方宣言
ATOM                MyRegisterClass(HINSTANCE);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE /* hPrevInstance */,
	_In_ LPWSTR /* lpCmdLine */,
	_In_ int nCmdShow)
{
	// Initialize global strings
    ::LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	::MyRegisterClass(hInstance);

	// Perform application initialization:
    ::hInst = hInstance;

    // 多重起動防止
    {
        HWND hWnd = ::FindWindow(CLASS_NAME_HIDDEN, 0);
        if (hWnd) {
            ::SendMessage(hWnd, WM_CLOSE, 0, 0);
        }
    }

    try {
        hiddenWnd.emplace(CLASS_NAME_HIDDEN, szTitle, ::hInst, nCmdShow);
    }
    catch (const std::runtime_error& e) {
        std::stringstream ss;
        ss << "runtime_error: " << e.what() << '\n';
        std::string s = ss.str();
        ::MessageBox(nullptr, std::wstring(s.begin(), s.end()).c_str(), _T("Error"), MB_OK);
        ::exit(1);
    }

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SANDSEXE));

    // キーボードフック
    HWND hWnd = hiddenWnd->getHWnd();
    KeyboardHook kbh(hWnd, ::GetCurrentProcessId());

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

ATOM MyRegisterClass(const HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SANDSEXE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = nullptr;
    wcex.lpszClassName  = CLASS_NAME_HIDDEN;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return ::RegisterClassExW(&wcex);
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(
    const HWND hWnd,
    const UINT message,
    const WPARAM wParam,
    const LPARAM lParam)
{
    switch (message)
    {
    case WM_UPDATE_IME_STATE:
    {
        ::PostMessage(
            ::FindWindow(CLASS_NAME_TRAY, 0),
            WM_UPDATE_IME_STATE, wParam, lParam);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

