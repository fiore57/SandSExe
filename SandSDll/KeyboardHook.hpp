#pragma once
#include "pch.hpp"

#define EXPORT_API_ extern "C"

EXPORT_API_ BOOL WINAPI StartHook(HWND);
EXPORT_API_ BOOL WINAPI FinishHook();
