// SandSExe.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "pch.h"

#include "resource.h"
#include "LowLevelKeyboardHook.hpp"
#include "SandSTrayWindow.hpp"
#include <Shlwapi.h>

#include <cstdlib>
#include <sstream>
#include <string>
#include <optional>

namespace {
    constexpr int MAX_LOADSTRING = 100;
    const UINT WM_UPDATE_IME_STATE = ::RegisterWindowMessage(L"WM_UPDATE_IME_STATE");

    // グローバル変数
    HINSTANCE hInst;                                // 現在のインターフェイス
    WCHAR szTitle[MAX_LOADSTRING]/* = L"SandS"*/;             // タイトルバーのテキスト

    std::optional<SandSTrayWindow> trayWnd;

    LPCTSTR CLASS_NAME_HIDDEN_32 = _T("SandSHiddenWindowClass32");
    LPCTSTR CLASS_NAME_HIDDEN_64 = _T("SandSHiddenWindowClass64");
    LPCTSTR CLASS_NAME_TRAY = _T("SandSTrayWindowClass");
    LPCTSTR CLASS_NAME = CLASS_NAME_TRAY;

#ifndef NDEBUG
    LPCTSTR HIDDEN_EXE_32_PATH = _T("../../x86-Debug/bin/SandSExeHidden32.exe");
    LPCTSTR HIDDEN_EXE_64_PATH = _T("../../x64-Debug/bin/SandSExeHidden64.exe");
#else
    LPCTSTR HIDDEN_EXE_32_PATH = _T("../../x86-Release/bin/SandSExeHidden32.exe");
    LPCTSTR HIDDEN_EXE_64_PATH = _T("../../x64-Release/bin/SandSExeHidden64.exe");
#endif

#ifndef _WIN64
    // 32bit

    // ref: https://blog.mono0x.net/2009/09/13/64bit-hook/
    bool IsWow64() {
        using LPFN_ISWOW64PROCESS = BOOL(WINAPI*)(HANDLE, PBOOL);
        // IsWow64Process関数が存在するかどうか調べる
        LPFN_ISWOW64PROCESS fnIsWow64Process = 
            reinterpret_cast<LPFN_ISWOW64PROCESS>(
                ::GetProcAddress(
                    ::GetModuleHandle(_T("kernel32")),
                    "IsWow64Process")
                );
        if (fnIsWow64Process) {
            // 存在する場合、呼び出して確認
            BOOL isWow64 = FALSE;
            return !!(fnIsWow64Process(::GetCurrentProcess(), &isWow64) && isWow64);
        }
        // 存在しない場合、確実に非WOW64環境
        return false;
    }
#else
    // 64bit
#endif

    LRESULT CALLBACK WndProc(
        const HWND hWnd,
        const UINT message,
        const WPARAM wParam,
        const LPARAM lParam)
    {
        if (message == WM_UPDATE_IME_STATE) {
            LLKeyboardHook::setImeState(wParam);
            return 0;
        }
        switch (message)
        {
        case TrayWindow::WM_USER_TRAYNOTIFYICON:
            trayWnd->onTrayNotify(wParam, lParam);
            break;
        case WM_COMMAND:
        {
            const int wmId = LOWORD(wParam);
            // Parse the menu selections:
            if (!trayWnd->onMenuSelected(wmId)) {
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        }
        case WM_DESTROY:
        {
            ::SendMessage(::FindWindow(CLASS_NAME_HIDDEN_32, 0), WM_CLOSE, 0, 0);
            ::SendMessage(::FindWindow(CLASS_NAME_HIDDEN_64, 0), WM_CLOSE, 0, 0);
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return 0;
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
        wcex.lpszClassName  = CLASS_NAME;
        wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        return ::RegisterClassExW(&wcex);
    }

}

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

    TCHAR szPath[MAX_PATH];
    GetModuleFileName(0, szPath, MAX_PATH);
    PathRemoveFileSpec(szPath);
    SetCurrentDirectory(szPath);

    // 多重起動防止
    {
        HWND hWnd = ::FindWindow(CLASS_NAME, 0);
        if (hWnd) {
            ::SendMessage(hWnd, WM_CLOSE, 0, 0);
        }
    }

    { // 32bit版のhiddenWindowを起動する
        STARTUPINFO si;
        ::GetStartupInfo(&si);
        PROCESS_INFORMATION pi;
        ::CreateProcess(HIDDEN_EXE_32_PATH, 0, 0, 0, TRUE, NORMAL_PRIORITY_CLASS, 0, 0, &si, &pi);
    }
    
#ifndef _WIN64
    // 32bit
    if (::IsWow64()) {
        // 32bit版のプログラムが64bitOSで実行された場合
        STARTUPINFO si;
        ::GetStartupInfo(&si);
        PROCESS_INFORMATION pi;
        // 64bit版のhiddenWindowを起動する
        ::CreateProcess(HIDDEN_EXE_64_PATH, 0, 0, 0, TRUE, NORMAL_PRIORITY_CLASS, 0, 0, &si, &pi);
    }
#else
    // 64bit
    { // 64bit版のhiddenWindowを起動する
        STARTUPINFO si;
        ::GetStartupInfo(&si);
        PROCESS_INFORMATION pi;
        ::CreateProcess(HIDDEN_EXE_64_PATH, 0, 0, 0, TRUE, NORMAL_PRIORITY_CLASS, 0, 0, &si, &pi);
    }
#endif

    try {
        trayWnd.emplace(CLASS_NAME, szTitle, ::hInst, nCmdShow);
    }
    catch (const std::runtime_error& e) {
        std::stringstream ss;
        ss << "runtime_error: " << e.what() << '\n';
        std::string s = ss.str();
        ::MessageBox(nullptr, std::wstring(s.begin(), s.end()).c_str(), _T("Error"), MB_OK);
        ::exit(1);
    }

	HACCEL hAccelTable = ::LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SANDSEXE));

    // キーボードフック
    const HWND hWnd = trayWnd->getHWnd();
    OutputDebugString(std::to_wstring((int)hWnd).c_str());
    LLKeyboardHook llkbh(hWnd);
    OutputDebugString(((std::wstring)(L"Error: ") + std::to_wstring(::GetLastError())).c_str());

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

