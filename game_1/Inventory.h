#ifndef GAME_1_INVENTORY_H
#define GAME_1_INVENTORY_H

#include "Item.h"

class Inventory {
public:
    std::vector<Item*> items;

    void AddItem(Item* item) {
        items.push_back(item);
    }

    void RemoveItem(Item* item) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (*it == item) {
                items.erase(it);
                break;
            }
        }
    }
};

#endif //GAME_1_INVENTORY_H
