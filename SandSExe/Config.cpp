#include "pch.h"

#include "Config.hpp"
#include <string>
#include <filesystem>
#include <fstream>

namespace {
    const std::filesystem::path config_file_path = std::filesystem::absolute(_T("./config.ini"));
} // namespace

Config::Config()
    : spaceWaitTimeMs(
        clampSpaceWaitTimeMs(
            ::GetPrivateProfileInt(
                _T("SandS"),
                _T("spaceWaitTime"),
                5000,
                config_file_path.wstring().c_str())
        ))
    , curMode(
        clampCurMode(
            ::GetPrivateProfileInt(
                _T("SandS"),
                _T("mode"),
                static_cast<std::int32_t>(Config::eMode::JapaneseOnly),
                config_file_path.wstring().c_str())
        ))
{
    saveData();
}

void Config::saveData()
{
    ::WritePrivateProfileString(
        _T("SandS"),
        _T("spaceWaitTime"),
        std::to_wstring(getSpaceWaitTimeMs()).c_str(),
        config_file_path.wstring().c_str()
    );
    ::WritePrivateProfileString(
        _T("SandS"),
        _T("mode"),
        std::to_wstring(static_cast<std::int32_t>(getCurMode())).c_str(),
        config_file_path.wstring().c_str()
    );
}
