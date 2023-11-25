#include <ctime>
#include "Trainer.h"

void Trainer::SwitchPokemon() {
    std::cout << "Choose one of your pokemons. Enter the number of it:\n";
    for (int i = 1; i < this->pokemons_.size() + 1; ++i) {
        std::cout << i << ".\t" <<  this->pokemons_[i - 1]->name_ << "\t";
        if(!this->pokemons_[i-1]->isDead){
            std::cout << this->pokemons_[i - 1]->health_ << "\n";
        }
        else std::cout << (this->pokemons_[i-1]->isStunned ? "stunned\n" : "fainted\n");
    }
    int k_chosen_pokemon;
    std::cin >> k_chosen_pokemon;
    --k_chosen_pokemon;


    if (k_chosen_pokemon >= 0 && k_chosen_pokemon < this->pokemons_.size()) {
        Pokemon* chosen_pokemon = this->pokemons_[k_chosen_pokemon];
        if (!chosen_pokemon->isDead && !chosen_pokemon->isStunned) {
            this->activePokemon = chosen_pokemon;
        } else {
            std::cout << "This Pokemon is" << (chosen_pokemon->isStunned ?  "stunned." : "fainted.") <<  "Please, enter another number from the list.\n";
            SwitchPokemon();
        }
    } else {
        std::cout << "Invalid choice. Please enter a valid number from the list!\n";
        SwitchPokemon();
    }
}
void Trainer::SwitchPokemonToRandom(){
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    this->activePokemon = this->pokemons_[rand() % this->pokemons_.size()];
}
