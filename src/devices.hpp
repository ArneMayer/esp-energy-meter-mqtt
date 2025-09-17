#pragma once

enum class DeviceType {
    SDM72D_M_V1,
    SDM72D_M_V2,
    DTS238_7,
    SDM630_V2,
    Growatt_MIC
};

inline const char* to_string(DeviceType type) {
    switch (type) {
        case DeviceType::SDM72D_M_V1:
            return "SDM72D_M_V1";
        case DeviceType::SDM72D_M_V2:
            return "SDM72D_M_V2";
        case DeviceType::DTS238_7:
            return "DTS238_7";
        case DeviceType::SDM630_V2:
            return "SDM630_V2";
        case DeviceType::Growatt_MIC:
            return "Growatt_MIC";
        default:
            return "Unknown";
    }
}