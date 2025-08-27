#pragma once

enum class PowerType { DL, TS, BL, PS, LS, ES, UF, AC, NT, EX, None };

struct PowerUp {
    PowerType type;
};

inline const char *PowerCode(PowerType t) {
    switch (t) {
        case PowerType::DL: return "DL";
        case PowerType::TS: return "TS";
        case PowerType::BL: return "BL";
        case PowerType::PS: return "PS";
        case PowerType::LS: return "LS";
        case PowerType::ES: return "ES";
        case PowerType::UF: return "UF";
        case PowerType::AC: return "AC";
        case PowerType::NT: return "NT";
        case PowerType::EX: return "EX";
        default: return "  ";
    }
}
