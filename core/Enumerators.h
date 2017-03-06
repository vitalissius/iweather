#pragma once

namespace units {
    
    enum PressureUnits : char { PU_ATMOSPHERES, PU_MILLIMETERSOFMERCURY, PU_MILLIBARS, PU_INCHESOFMERCURY };

    enum SpeedUnits : char { SU_KILOMETERSPERHOUR, SU_METERSPERSECOND, SU_MILESPERHOUR, SU_FEETPERSECOND };
	
    enum TemperatureUnits : char { TU_CELSIUS, TU_FAHRENHEIT };
	
    enum PressureStateUnits { PSU_STEADY, PSU_RISING, PSU_FALLING };

	enum CompassRoseUnits {
        CRU_NORTH, CRU_NORTHNORTHEAST, CRU_NORTHEAST, CRU_EASTNORTHEAST,
        CRU_EAST, CRU_EASTSOUTHEAST, CRU_SOUTHEAST, CRU_SOUTHSOUTHEAST,
        CRU_SOUTH, CRU_SOUTHSOUTHWEST, CRU_SOUTHWEST, CRU_WESTSOUTHWEST,
        CRU_WEST, CRU_WESTNORTHWEST, CRU_NORTHWEST, CRU_NORTHNORTHWEST
	};

    enum class WeatherProvider : char { Yahoo, Accuweather };

    enum BeaufortNumbers : char {
        BN_CALM,
        BN_LIGHT_AIR,
        BN_LIGHT_BREEZE,
        BN_GENTLE_BREEZE,
        BN_MODERATE_BREEZE,
        BN_FRESH_BREEZE,
        BN_STRONG_BREEZE,
        BN_HIGH_WIND,          // MODERATE GALE / NEAR GALE
        BN_GALE,               // FRESH GALE
        BN_STRONG_GALE,        // SEVERE GALE
        BN_STORM,              // WHOLE GALE
        BN_VIOLENT_STORM,
        BN_HURRICANE_FORCE
    };
}
