#pragma once

#include "Enumerators.h"
#include "Utility.h"

#include <unordered_map>
#include <vector>
#include <string>

class LanguagePack final : private Noncopyable {
public:
    /* Enums */
    enum Keys { KEY_PRESSURE, KEY_SPEED, KEY_TEMPERATURE, KEY_PRESSURESTATE, KEY_COMPASSROSE };
    enum class LanguageUnits : char { ENG, RUS };

private:
    explicit LanguagePack(LanguageUnits langUnit=LanguageUnits::ENG);

public:
    LanguagePack(LanguagePack&&) = default;
    LanguagePack& operator=(LanguagePack&&) = default;

    /* Methods */
    static LanguagePack& Instance();

    void SetLanguageUnit(LanguageUnits language);
    void SetPressureUnit(units::PressureUnits pressure);
    void SetSpeedUnit(units::SpeedUnits speed);
    void SetTemperatureUnit(units::TemperatureUnits temperature);

    LanguageUnits GetLanguageUnit() const;
	units::PressureUnits GetPressureUnit() const;
	units::SpeedUnits GetSpeedUnit() const;
	units::TemperatureUnits GetTemperatureUnit() const;

    const std::string& GetPressureName() const;
    const std::string& GetSpeedName() const;
    const std::string& GetTemperatureName(units::TemperatureUnits temperature) const;
    const std::string& GetPressureStateName(units::PressureStateUnits pressureState) const;
    const std::string& GetCompassRoseName(units::CompassRoseUnits compassRose) const;

private:
    /* Private const static members */
    static const std::unordered_map<Keys, std::vector<std::string>> csm_engPack;
    static const std::unordered_map<Keys, std::vector<std::string>> csm_rusPack;

public:
    /* Public const static member */
    static const int csm_compassPoints = 16;

private:
    /* Private members */
	const std::unordered_map<Keys, std::vector<std::string>>* m_pack;     // 4 bytes
    LanguageUnits m_languageUnit;                               // 1 byte
	units::PressureUnits m_pressureUnit;                 // 1 byte
	units::SpeedUnits m_speedUnit;                       // 1 byte
	units::TemperatureUnits m_temperatureUnit;           // 1 byte
};
