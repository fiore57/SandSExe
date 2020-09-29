#include "pch.hpp"

#include "Config.hpp"

#include <string>
#include <filesystem>
#include <fstream>

namespace {
    // const std::filesystem::path CONFIG_FILE_PATH = std::filesystem::absolute(_T("./config.ini"));
    constexpr auto CONFIG_FILE_PATH = _T("./config.ini");
    constexpr auto APP_NAME = _T("SandS");
    constexpr auto KEY_NAME_SPACE_WAIT_TIME = _T("spaceWaitTime");
    constexpr auto KEY_NAME_MODE = _T("mode");
    constexpr std::int32_t DEFAULT_SPACE_WAIT_TIME = 5000;
    constexpr std::int32_t DEFAULT_MODE = static_cast<std::int32_t>(Config::eMode::JapaneseOnly);

    std::int32_t getConfigInt(LPCTSTR keyName, const std::int32_t defaultValue) {
        return ::GetPrivateProfileInt(APP_NAME, keyName, defaultValue, CONFIG_FILE_PATH);
    };
    void writeConfigInt(LPCTSTR keyName, const std::int32_t value) {
        ::WritePrivateProfileString(APP_NAME, keyName, std::to_wstring(value).c_str(), CONFIG_FILE_PATH);
    };
} // namespace

Config::Config()
    : spaceWaitTimeMs(clampSpaceWaitTimeMs(::getConfigInt(KEY_NAME_SPACE_WAIT_TIME, DEFAULT_SPACE_WAIT_TIME)))
    , curMode(clampCurMode(::getConfigInt(KEY_NAME_MODE, DEFAULT_MODE)))
{
    saveData();
}

void Config::saveData()
{
    ::writeConfigInt(KEY_NAME_SPACE_WAIT_TIME, getSpaceWaitTimeMs());
    ::writeConfigInt(KEY_NAME_MODE, static_cast<std::int32_t>(getCurMode()));
}
