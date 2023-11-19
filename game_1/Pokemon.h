#ifndef GAME_1_POKEMON_H
#define GAME_1_POKEMON_H
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <ctime>

class Effect {
public:
    std::string name;
    int duration = 5;
};

class Pokemon {
public:
    /* Вновь такие вещи должны быть приватными */
    std::string name;
    int level;
    int health;
    Effect effect;
    int attackPower = 7;
    int defensePower = 2;
    bool isDead = false;

    bool HasStatusCondition() const{
        return effect.name != "None";
    }
    Pokemon(std::string name, int level){
        this->name = std::move(name);
        this->level = level;
        this->health = level * 10;
        this->effect.name = "None";
    }
    /* Что значит защитник? Защитник чего? 
       Апдейт через 10 минут: я думаю, это все-таки
       должен быть enemy */
    /* Ты ее нигде не вызываешь. Наличие просто виртуальной
     * функции без ее использования не закрывает этот пунктик
     * в задании. Ну и, конечно, не особенно уникальное поведение. 
     * Ввести бы каких-то особых эффектов, типа стана или заставляющие
     * противника сменить покемона */
    virtual void ApplyEffect(Pokemon* defender) {}
};

class FirePokemon : public Pokemon {
public:
    FirePokemon(std::string name, int level) : Pokemon(std::move(name), level) {}

    void ApplyEffect(Pokemon* defender) override{
        defender->effect.name = "Fire";
        defender->effect.duration = 3;
    }
};

class ElectricPokemon : public Pokemon {
public:
    ElectricPokemon(std::string name, int level) : Pokemon(std::move(name), level) {}

    void ApplyEffect(Pokemon* defender) override{
        defender->effect.name = "Electricity";
        defender->effect.duration = 3;
    }
};

class GrassPokemon : public Pokemon {
public:
    GrassPokemon(std::string name, int level) : Pokemon(std::move(name), level) {}

    void ApplyEffect(Pokemon* defender) override{
        /* Думаю, типы эффектов следовало бы сделать enum-ом (enum class)*/
        defender->effect.name = "Vines";
        defender->effect.duration = 3;
    }
};


#endif
