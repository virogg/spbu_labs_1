#ifndef GAME_1_TRAINER_H
#define GAME_1_TRAINER_H

#include <utility>
#include <memory>
#include "Inventory.h"

class Trainer {
private:
    int kAlivePokemons_ = 0;
    std::string name_;
    std::vector<Pokemon*> pokemons_;

    friend class BattleArena;
public:
    Inventory inventory;
    Pokemon* activePokemon;

    explicit Trainer(std::string  name) : name_(std::move(name)){}
    inline void SetInventory(Inventory inv){
        this->inventory = std::move(inv);
    }

    inline void UseHealingItem(HealingItem* item, Pokemon* target){
        target->health_ += item->power_;
        std::cout << target->name_ << " has been healed by " << item->power_ << " HP.\n";
    }
    inline void UseBattleItem(BattleItem* item, Pokemon* target){
        target->health_ -= item->power_;
        std::cout << "The grenade has been thrown at " << target->name_ << " dealing 10 damage to it!";
    }

    inline void AddPokemon(Pokemon *pokemon){
        this->pokemons_.push_back(pokemon);
        this->activePokemon = pokemon;
        ++kAlivePokemons_;
    }
    void RemovePokemon(){
        for(auto & pokemon : this->pokemons_){
            if(pokemon->health_ <= 0 && !pokemon->isDead){
                std::cout << pokemon->name_ << " is dead!\n";
                pokemon->isDead = true;
                this->kAlivePokemons_ -= 1;
            }
        }
    }
    void SwitchPokemon();
    void SwitchPokemonToRandom();

    inline void ReplaceIfDead(){
        if(this->activePokemon->isDead || this->activePokemon->isStunned){
            std::cout << "Your active pokemon is " << (this->activePokemon->isDead ? "fainted" : "stunned") << ". Please, choose another one:\n";
            this->SwitchPokemon();
        }
    }
};

#endif //GAME_1_TRAINER_H
