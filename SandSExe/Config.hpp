#pragma once

#include "pch.h"
#include "Singleton.hpp"
#include "resource.h"
#include <algorithm>

class Config final : public Singleton<Config> {
    Config();
    ~Config() { saveData(); }
    friend Singleton<Config>;

public:
    std::int32_t clampSpaceWaitTimeMs(const std::int32_t timeMs) const {
        return std::clamp(timeMs, 0, 10000);
    }
    std::int32_t getSpaceWaitTimeMs() const { return spaceWaitTimeMs; }
    void setSpaceWaitTimeMs(const std::int32_t timeMs) {
        spaceWaitTimeMs = clampSpaceWaitTimeMs(timeMs);
        saveData();
    }

    enum class eMode {
        Off = IDC_RADIO1,
        JapaneseOnly = IDC_RADIO2,
        IMEOnOnly = IDC_RADIO3,
        Always = IDC_RADIO4,
    };
    eMode clampCurMode(const std::int32_t curModeInt) const {
        return static_cast<eMode>(std::clamp(
            static_cast<std::int32_t>(curModeInt),
            static_cast<std::int32_t>(eMode::Off),
            static_cast<std::int32_t>(eMode::Always)));
    }
    eMode getCurMode() const { return curMode; }
    void setCurMode(const eMode newMode) {
        curMode = newMode;
        saveData();
    }

private:
    void saveData();

    std::int32_t spaceWaitTimeMs;
    eMode curMode;
};