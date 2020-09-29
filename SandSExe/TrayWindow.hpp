#pragma once
#include "pch.hpp"

class TrayWindow {
public:
    constexpr static std::int32_t WM_USER_TRAYNOTIFYICON = WM_APP + 100;

    TrayWindow(
        LPCTSTR szWindowClass,
        LPCTSTR szTitle,
        HINSTANCE hInstance);

    enum class eNotifyIconMessage {
        Add, Modify, Delete
    };

    void notifyIcon(
        eNotifyIconMessage nim,
        HICON hIcon,
        LPCTSTR pszTip);

    void onTrayNotify(
        UINT wParam,
        LONG lParam);

    virtual void onLButtonClick() {}
    virtual void onRButtonClick() {}

    [[nodiscard]] HWND getHWnd() const { return hWnd; }

protected:
    [[nodiscard]] HINSTANCE getHInst() const { return hInst; }

private:
    const HWND hWnd = nullptr;
    const HINSTANCE hInst = nullptr;
    HICON hIcon = nullptr;

public:
    TrayWindow() = delete;
    ~TrayWindow() = default;
    TrayWindow(const TrayWindow&) = delete;
    TrayWindow(TrayWindow&&) = delete;
    TrayWindow& operator=(const TrayWindow&) = delete;
    TrayWindow& operator=(TrayWindow&&) = delete;
};

