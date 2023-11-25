#ifndef GAME_1_EFFECT_H
#define GAME_1_EFFECT_H
#include "string"

enum class EffectType{
    kNone,
    kFire,
    kStun,
    kShock
};
inline std::string ShowEffect(EffectType effect){
    switch (effect) {
        case EffectType::kNone:
            return "None";
        case EffectType::kFire:
            return "Fire";
        case EffectType::kStun:
            return "Stunned";
        case EffectType::kShock:
            return "Shocked";
    }
}

#endif //GAME_1_EFFECT_H
