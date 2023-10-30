#ifndef GAME_1_TRAINER_H
#define GAME_1_TRAINER_H

#include "Inventory.h"

class Trainer {
public:
    int kAlivePokemons = 0;
    std::string name;
    std::vector<Pokemon*> pokemons;
    Inventory inventory;
    Pokemon* activePokemon;

    Trainer(std::string name){
        this->name = std::move(name);
    }
    void SetInventory(Inventory inv){
        this->inventory = inv;
    }

    static void UseHealingItem(HealingItem* item, Pokemon* target){
        auto *healingItem = reinterpret_cast<HealingItem*>(item);
        target->health += healingItem->healingPower;
        std::cout << target->name << " has been healed by " << healingItem->healingPower << " HP.\n";
   }
    static void UseBattleItem(BattleItem* item, Pokemon* target){
        auto *battleItem = reinterpret_cast<BattleItem*>(item);
        target->health -= battleItem->battleDamage;
        std::cout << "The grenade has been thrown at " << target->name << " dealing 10 damage to it!";
   }

    void AddPokemon(Pokemon* pokemon){
        this->pokemons.push_back(pokemon);
        this->activePokemon = pokemon;
        ++kAlivePokemons;
    }

    void SwitchPokemon() {
        std::cout << "Choose one of your pokemons. Enter the number of it:\n";
        for (int i = 1; i < this->pokemons.size() + 1; ++i) {
            std::cout << i << ".\t" <<  this->pokemons[i - 1]->name << "\t";
            if(!this->pokemons[i-1]->isDead){
                std::cout << this->pokemons[i - 1]->health << "\n";
            }
            else std::cout << "fainted\n";
        }
        int kChosenPokemon;
        std::cin >> kChosenPokemon;
        --kChosenPokemon;


        if (kChosenPokemon >= 0 && kChosenPokemon < this->pokemons.size()) {
            Pokemon* chosenPokemon = this->pokemons[kChosenPokemon];
            if (chosenPokemon->health > 0) {
                this->activePokemon = chosenPokemon;
                return;
            } else {
                std::cout << "This Pokemon is fainted. Please, enter another number from the list.\n";
                SwitchPokemon();
            }
        } else {
            std::cout << "Invalid choice. Please enter a valid number from the list!\n";
            SwitchPokemon();
        }
    }
};

#endif //GAME_1_TRAINER_H
