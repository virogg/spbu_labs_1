#ifndef GAME_1_ITEM_H
#define GAME_1_ITEM_H

#include "Pokemon.h"

class Item {
protected:
    int power_;
    std::string name_;

    friend class Trainer;
    friend class BattleArena;
};

class HealingItem : public Item {
public:
    HealingItem(std::string name, int healingPower){
        this->name_ = std::move(name);
        this->power_ = healingPower;
    }
};

class BattleItem : public Item {
public:
    BattleItem(std::string name, int damagePower){
        this->name_ = std::move(name);
        this->power_ = damagePower;
    }
};

#endif //GAME_1_ITEM_H
