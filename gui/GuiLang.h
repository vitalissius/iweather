#pragma once

#include "Resource.h"
#include "../core/LanguagePack.h"
#include "../core/Utility.h"

#include <Windows.h>

#include <unordered_map>
#include <set>

class GuiLang : private Noncopyable {
private:
    class Dict {
    private:
        const tstring eng;
        const tstring rus;
        const tstring ukr;

    public:
        Dict(tstring eng, tstring rus, tstring ukr);
        const tstring& GetWord(LanguagePack::LanguageUnits langUnit) const;
    };

public:
    LanguagePack::LanguageUnits GetLanguageUnit() const;
    units::PressureUnits GetPressureUnit() const;
    units::SpeedUnits GetSpeedUnit() const;
    units::TemperatureUnits GetTemperatureUnit() const;
    void SetLanguageUnit(LanguagePack::LanguageUnits languageUnit);
    void SetPressureUnit(units::PressureUnits pressureUnit);
    void SetSpeedUnit(units::SpeedUnits speedUnit);
    void SetTemperatureUnit(units::TemperatureUnits temperatureUnit);
    const tstring& GetWord(const UINT id) const;

private:
    const static std::unordered_map<UINT, const Dict> ms_dict;
    LanguagePack& m_langPack = LanguagePack::Instance();
};
