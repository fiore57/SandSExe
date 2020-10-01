#pragma once

// Application Data
#ifdef DEBUG
#define VER_DEBUG   VS_FF_DEBUG
#define VER_PRIVATE VS_FF_PRIVATEBUILD
#else
#define VER_DEBUG   0
#define VER_PRIVATE 0
#endif
#define VER_STR_COMMENTS         ""
#define VER_STR_COMPANYNAME      ""
#define VER_STR_FILEDESCRIPTION  "SandSExe v1.0.0"
#define VER_FILEVERSION          1,0,0,0
#define VER_STR_FILEVERSION      "1.0.0"
#define VER_STR_INTERNALNAME     "SandSExe"
#define VER_STR_ORIGINALFILENAME "SandSExe.exe"
#define VER_STR_LEGALCOPYRIGHT   "(C) 2020 fiore All Rights Reserved"
#define VER_STR_PRODUCTNAME      "SandSExe"
#define VER_PRODUCTVERSION       1,0,0,0
#define VER_STR_PRODUCTVERSION   "1.0.0"

// Icon
#define IDI_SANDSEXE            107
// Menu
#define IDM_TRAY_R              110
#define IDM_APP_CONFIG          111
#define IDM_APP_ABOUT           112
#define IDM_APP_EXIT            113
// About Dialog
#define IDD_ABOUTBOX            103
#define IDR_MAINFRAME           128
// Config Dialog
#define IDD_CONFIGBOX           114
#define IDC_SLIDER1             1001
#define IDC_STATIC1             1005
#define IDC_RADIO1              1024
#define IDC_RADIO2              1025
#define IDC_RADIO3              1026
#define IDC_RADIO4              1027

#ifndef IDC_STATIC
#define IDC_STATIC              -1
#endif

