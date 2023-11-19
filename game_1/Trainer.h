#ifndef GAME_1_TRAINER_H
#define GAME_1_TRAINER_H

#include <utility>

#include "Inventory.h"

class Trainer {
public:
    /* Эти поля однозначно не должны быть публичными */
    int kAlivePokemons = 0;
    std::string name;
    std::vector<Pokemon*> pokemons;
    Inventory inventory;
    Pokemon* activePokemon;

    explicit Trainer(std::string name){
        this->name = std::move(name);
    }
    void SetInventory(Inventory inv){
        this->inventory = std::move(inv);
    }
    /* Реализации длинее двух строк надо уже выносить в cpp файл. 
     * Узнай, что такое inline */
    /* И почему функци статик? */
    static void UseHealingItem(HealingItem* item, Pokemon* target){
        /* Что это за страшная конструкция. Ты же и так передаешь 
         * указатель нужного типа. Так еще и используешь самый опасный
         * вид каста */
        auto *healing_item = reinterpret_cast<HealingItem*>(item);
        target->health += healing_item->healingPower;
        std::cout << target->name << " has been healed by " << healing_item->healingPower << " HP.\n";
   }
    static void UseBattleItem(BattleItem* item, Pokemon* target){
        auto *battle_item = reinterpret_cast<BattleItem*>(item);
        target->health -= battle_item->battleDamage;
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
        int k_chosen_pokemon;
        std::cin >> k_chosen_pokemon;
        --k_chosen_pokemon;


        if (k_chosen_pokemon >= 0 && k_chosen_pokemon < this->pokemons.size()) {
            Pokemon* chosen_pokemon = this->pokemons[k_chosen_pokemon];
            if (chosen_pokemon->health > 0) {
                this->activePokemon = chosen_pokemon;
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
