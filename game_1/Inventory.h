#ifndef GAME_1_INVENTORY_H
#define GAME_1_INVENTORY_H

#include "Item.h"

class Inventory {
public:
    std::vector<Item*> items;

    void AddItem(Item* item) {
        items.push_back(item);
    }
};

#endif //GAME_1_INVENTORY_H
