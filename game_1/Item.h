#ifndef GAME_1_ITEM_H
#define GAME_1_ITEM_H

#include "Pokemon.h"

class Item {
public:
    std::string name;
};

class HealingItem : public Item {
public:
    int healingPower;
    HealingItem(std::string name, int healingPower){
        this->name = std::move(name);
        this->healingPower = healingPower;
    }
};

class BattleItem : public Item {
public:
    int battleDamage;

    BattleItem(std::string name, int power){
        this->name = std::move(name);
        this->battleDamage = power;
    }
};

#endif //GAME_1_ITEM_H
