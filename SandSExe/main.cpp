#include "pch.hpp"

#include "resource.h"
#include "LowLevelKeyboardHook.hpp"
#include "SandSTrayWindow.hpp"
#include "./Define.hpp"

namespace {
    std::optional<SandSTrayWindow> trayWnd;
    std::optional<LLKeyboardHook> llkbh;

    constexpr auto CLASS_NAME = fiore::sands::CLASS_NAME_TRAY;

#ifndef NDEBUG
    constexpr auto HIDDEN_EXE_32_PATH = _T("../../x86-Debug/bin/SandSHidden32.exe");
    constexpr auto HIDDEN_EXE_64_PATH = _T("../../x64-Debug/bin/SandSHidden64.exe");
#else
    constexpr auto HIDDEN_EXE_32_PATH = _T("./SandSHidden32.exe");
    constexpr auto HIDDEN_EXE_64_PATH = _T("./SandSHidden64.exe");
#endif

#ifndef _WIN64
    // 32bit

    // ref: https://blog.mono0x.net/2009/09/13/64bit-hook/
    bool IsWow64() {
        using LPFN_ISWOW64PROCESS = BOOL(WINAPI*)(HANDLE, PBOOL);
        // IsWow64Process関数が存在するかどうか調べる
        const auto hModule = ::GetModuleHandle(_T("kernel32"));
        if (!hModule) return false;
        LPFN_ISWOW64PROCESS fnIsWow64Process =
            reinterpret_cast<LPFN_ISWOW64PROCESS>(
                ::GetProcAddress(
                    hModule,
                    "IsWow64Process")
                );
        if (fnIsWow64Process) {
            // 存在する場合、呼び出して確認
            BOOL isWow64 = FALSE;
            return (fnIsWow64Process(::GetCurrentProcess(), &isWow64) && isWow64);
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
        if (message == fiore::sands::WM_UPDATE_IME_STATE) {
            LLKeyboardHook::setImeState(static_cast<std::int32_t>(wParam));
            return 0;
        }
        switch (message)
        {
        case TrayWindow::WM_USER_TRAYNOTIFYICON:
            trayWnd->onTrayNotify(static_cast<UINT>(wParam), static_cast<LONG>(lParam));
            break;
        case WM_COMMAND:
        {
            const auto wmId = LOWORD(wParam);
            // Parse the menu selections:
            if (!trayWnd->onMenuSelected(wmId)) {
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;
        }
        case WM_DESTROY:
        {
            ::SendMessage(::FindWindow(fiore::sands::CLASS_NAME_HIDDEN_32, 0), WM_CLOSE, 0, 0);
            ::SendMessage(::FindWindow(fiore::sands::CLASS_NAME_HIDDEN_64, 0), WM_CLOSE, 0, 0);
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

    void runExe(LPCTSTR exePath) {
        STARTUPINFO si;
        ::GetStartupInfo(&si);
        PROCESS_INFORMATION pi;
        if (!::CreateProcess(
            exePath, 0, 0, 0, TRUE, NORMAL_PRIORITY_CLASS, 0, 0, &si, &pi)) {
            std::stringstream ss;
            std::wstring tmp(exePath);
            ss << "Can't open " << std::string(tmp.begin(), tmp.end());
            throw std::runtime_error(ss.str().c_str());
        }
        ::CloseHandle(pi.hProcess);
        ::CloseHandle(pi.hThread);
    }
}

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE /* hPrevInstance */,
    _In_ LPWSTR /* lpCmdLine */,
    _In_ int /* nCmdShow */)
{
    ::MyRegisterClass(hInstance);

    // 多重起動防止
    {
        HWND hWnd = ::FindWindow(CLASS_NAME, 0);
        if (hWnd) {
            ::SendMessage(hWnd, WM_CLOSE, 0, 0);
        }
    }

    try {
        // 32bit版のhiddenWindowを起動する
        ::runExe(HIDDEN_EXE_32_PATH);
    
#ifndef _WIN64
        if (::IsWow64()) // 32bit版のプログラムが64bitOSで実行されたかチェック
#endif
        {
            // 64bitOSで実行された場合、64bit版のhiddenWindowを起動する
            ::runExe(HIDDEN_EXE_64_PATH);
        }

        trayWnd.emplace(CLASS_NAME, fiore::sands::WINDOW_TITLE, hInstance);
        llkbh.emplace(trayWnd->getHWnd());
    }
    catch (const std::runtime_error& e) {
        std::stringstream ss;
        ss << "runtime_error: " << e.what() << '\n';
        std::string s = ss.str();
        ::MessageBox(nullptr, std::wstring(s.begin(), s.end()).c_str(), _T("Error"), MB_OK);
        ::SendMessage(::FindWindow(fiore::sands::CLASS_NAME_HIDDEN_32, 0), WM_CLOSE, 0, 0);
        ::SendMessage(::FindWindow(fiore::sands::CLASS_NAME_HIDDEN_64, 0), WM_CLOSE, 0, 0);
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

