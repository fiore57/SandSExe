#pragma once
#include "pch.hpp"

#include "TrayWindow.hpp"

class SandSTrayWindow : public TrayWindow {
public:
    SandSTrayWindow(
        LPCTSTR szWindowClass,
        LPCTSTR szTitle,
        HINSTANCE hInstance);
    ~SandSTrayWindow();

    void onLButtonClick() override;
    void onRButtonClick() override;

    bool onMenuSelected(WORD wmId);

private:
    bool isSandSEnabled = true;
    void onAppConfig();
    void onAppAbout();
    void onAppExit();

public:

    SandSTrayWindow() = delete;
    SandSTrayWindow(const SandSTrayWindow&) = delete;
    SandSTrayWindow(SandSTrayWindow&&) = delete;
    SandSTrayWindow& operator=(const SandSTrayWindow&) = delete;
    SandSTrayWindow& operator=(SandSTrayWindow&&) = delete;
};

