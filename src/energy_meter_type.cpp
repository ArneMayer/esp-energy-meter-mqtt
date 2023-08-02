#include "energy_meter_type.hpp"
#include <WString.h>

String meter_type_to_string(EnergyMeterType type) {
    switch (type) {
        case EnergyMeterType::SDM72D_M_V1:
            return "SDM72D_M_V1";
        case EnergyMeterType::SDM72D_M_V2:
            return "SDM72D_M_V2";
        case EnergyMeterType::DTS238_7:
            return "DTS238_7";
        case EnergyMeterType::SDM630_V2:
            return "SDM630_V2";
        default:
            return "Unknown EnergyMeterType";
    }
}

EnergyMeterType string_to_meter_type(String str) {
    if (str == "SDM72D_M_V1") {
        return EnergyMeterType::SDM72D_M_V1;
    } else if (str == "SDM72D_M_V2") {
        return EnergyMeterType::SDM72D_M_V2;
    } else if (str == "DTS238_7") {
        return EnergyMeterType::DTS238_7;
    } else if (str == "SDM630_V2") {
        return EnergyMeterType::SDM630_V2;
    } else {
        return EnergyMeterType::UNKNOWN;
    }
}
