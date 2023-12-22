#ifndef GAME_1_POKEMON_H
#define GAME_1_POKEMON_H
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "Effect.h"

class Effect {
public:
    EffectType type_;
    int duration_ = 0;
    Effect(EffectType type) {
        type_ = type;
    }
};

class Pokemon {
private:
    std::string name_;
    int level_;
    int health_;


    friend class Trainer;
    friend class BattleArena;
public:
    /* Странно иметь эти поля публичными */
    Effect effect;
    int attackPower = 7;
    int defensePower = 2;
    bool isDead = false;
    bool isStunned = false;

    bool HasStatusCondition() const{
        return effect.type_ != EffectType::kNone;
    }
    Pokemon(std::string name, int level, Effect effect) : effect(effect) {
        this->name_ = std::move(name);
        this->level_ = level;
        this->health_ = level * 10;
        this->effect = EffectType::kNone;
    }
    // Function to check and remove status conditions at the end of a turn
    void RemoveStatusConditions() {
        this->effect = EffectType::kNone;
    }
    /* Можно оставить без реализации. Если у тебя не может быть просто покемона типа Pokemon,
     * то можно удалить реализацию, написав = 0 */
    virtual void ApplyEffect(Pokemon* enemy) {
        //????
    }
};

class FirePokemon : public Pokemon {
public:
    /* Тут в списке инициализации ты уровень не инициализируешь и странное значение
     * эффекта */
    FirePokemon(std::string name, int level) : Pokemon(std::move(name), level, Effect(EffectType::kShock)) {}

    void ApplyEffect(Pokemon* enemy) override{
        enemy->effect = EffectType::kFire;
        enemy->effect.duration_ = 3;
    }
};

class ElectricPokemon : public Pokemon {
public:
    ElectricPokemon(std::string name, int level) : Pokemon(std::move(name), level, Effect(EffectType::kShock)) {}

    void ApplyEffect(Pokemon* enemy) override{
        enemy->effect = EffectType::kShock;
    }
};

class GrassPokemon : public Pokemon {
public:
    GrassPokemon(std::string name, int level) : Pokemon(std::move(name), level, Effect(EffectType::kShock)) {}

    void ApplyEffect(Pokemon* enemy) override{
        enemy->effect = EffectType::kStun;
        enemy->isStunned = true;
        enemy->effect.duration_ = 1;
    }
};


#endif
