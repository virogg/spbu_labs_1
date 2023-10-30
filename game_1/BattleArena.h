#ifndef GAME_1_BATTLEARENA_H
#define GAME_1_BATTLEARENA_H

#include "Trainer.h"

enum class ActionType {
    SwitchPokemon,
    UseItem,
    Attack,
    Quit
};

class BattleArena {
public:
    bool battleOver = false;
    int turnCounter = 0;
    Trainer trainer1;
    Trainer trainer2;

    BattleArena(Trainer tr1, Trainer tr2, const std::string& trainerName1,  const std::string& trainerName2): trainer1(trainerName1), trainer2(trainerName2) {
        this->trainer1 = std::move(tr1);
        this->trainer2 = std::move(tr2);
    }

    // Function to display the current state of the battle
    static void DisplayBattleStatus(Trainer* currentTrainer, Trainer* opponentTrainer) {
        std::cout << "Current Trainer: " << currentTrainer->name << "\n";
        // Display current trainer's active Pokemon and their health, status, etc.
        std::cout << "Active Pokemon: " << currentTrainer->activePokemon->name << " (Health: " << currentTrainer->activePokemon->health << ")\n";

        std::cout << "\n";

        // Display opponent trainer's active Pokemon and their health, status, etc.
        std::cout << "Opponent Trainer: " << opponentTrainer->name << "\n";
        std::cout << "Active Pokemon: " << opponentTrainer->activePokemon->name << " (Health: " << opponentTrainer->activePokemon->health << ")\n";
        std::cout << "\nYour pokemon:\n";

        //Display current trainer's pokemon and their status
        for(Pokemon* pokemon : currentTrainer->pokemons){
            std::cout << pokemon->name;
            if(!pokemon->isDead) {
                std::cout << "\tHealth: " << pokemon->health << "\tEffect: " << pokemon->effect.name << "\n";
            }
            else{
                std::cout << "\tfainted";
            }
        }
        std::cout << "\n";
    }

    // Function to calculate damage based on the type advantages, attack power, etc.
    static int CalculateDamage(Pokemon* attacker, Pokemon* defender) {
        int damage = (attacker->level * attacker->attackPower) - (defender->level * defender->defensePower);
        return damage;
    }

    static void RemovePokemon(Trainer* trainer){
        for(auto & pokemon : trainer->pokemons){
            if(pokemon->health <= 0 && !pokemon->isDead){
                std::cout << pokemon->name << " is dead!\n";
                pokemon->isDead = true;
                trainer->kAlivePokemons -= 1;
            }
        }
    }

    // Function to check and remove status conditions at the end of a turn
    static void RemoveStatusConditions(Pokemon* pokemon) {
        pokemon->effect.name = "None";
        pokemon->effect.duration = 0;
    }

    // Function to check and apply status effects (e.g., poison, burn) at the end of each turn
    static void ApplyStatusConditions(Trainer* trainer) {
        for (Pokemon* pokemon : trainer->pokemons) {
            if(pokemon->HasStatusCondition() && pokemon->health > 0){
                pokemon->effect.duration -= 1;
                pokemon->health -= 5;
                if(pokemon->effect.duration == 0){
                    RemoveStatusConditions(pokemon);
                }
            }
        }
    }
    // Function to handle a Pokemon's turn
    static void HandlePokemonTurn(Pokemon* activePokemon, Pokemon* targetPokemon) {
        int damage = CalculateDamage(activePokemon, targetPokemon);
        activePokemon->ApplyEffect(targetPokemon);
        std::cout << activePokemon->name << " attacks " << targetPokemon->name << " for " << damage << " damage!\n";
        targetPokemon->health -= damage;
    }

    // Function to handle the overall flow of a turn in the battle
    static void HandleTurn(Trainer* currentTrainer, Trainer* opponentTrainer) {
        // Apply status conditions at the end of the turn
        ApplyStatusConditions(currentTrainer);
        ApplyStatusConditions(opponentTrainer);

        RemovePokemon(currentTrainer);
        RemovePokemon(opponentTrainer);
    }
    static Pokemon* GetUserChoiceOfPokemon(Trainer* currentTrainer) {
        std::cout << "Choose a Pokemon:" << "\n";
        int index = 1;

        // Display the user's available Pokemon and their names
        for (Pokemon* pokemon : currentTrainer->pokemons) {
            std::cout << index << ". " << pokemon->name;
            if(!pokemon->isDead) {
                std::cout << " (Health " << pokemon->health << ")" << "\n";
            } else {
                std::cout << " fainted\n";
            }
            index++;
        }

        int choice;
        std::cin >> choice;

        Pokemon* chosenPokemon = currentTrainer->pokemons[choice - 1];

        // Validate the choice and return the selected Pokemon
        if (choice >= 1 && choice <= currentTrainer->pokemons.size() && !chosenPokemon->isDead) {
            return chosenPokemon;
        } else {
            std::cout << "Invalid choice. Try again.\n";
            return GetUserChoiceOfPokemon(currentTrainer); // Recursive call for invalid choices
        }
    }
    // Function to get the user's choice of item
    Item* GetUserChoiceOfItem(Trainer* currentTrainer) {
        std::cout << "Choose an item:\n";
        int index = 1;

        // Display the user's available items and their names
        for (Item* item : currentTrainer->inventory.items) {
            std::cout << index << ". " << item->name << "\n";
            index++;
        }

        int choice;
        std::cin >> choice;

        // Validate the choice and return the selected item
        if (choice >= 1 && choice <= currentTrainer->inventory.items.size()) {
            return currentTrainer->inventory.items[choice - 1];
        } else {
            std::cout << "Invalid choice. Try again.\n";
            return GetUserChoiceOfItem(currentTrainer); // Recursive call for invalid choices
        }
    }
    void ReplaceIfDead(Trainer* trainer){
        if(trainer->activePokemon->isDead){
            trainer->SwitchPokemon();
        }
    }

    void StartBattle() {
        Trainer* currentTrainer = &trainer1;
        Trainer* opponentTrainer = &trainer2;
        while (!battleOver) {
            if(turnCounter % 2 == 0){
                currentTrainer = &trainer1;
                opponentTrainer = &trainer2;
            }
            else{
                currentTrainer = &trainer2;
                opponentTrainer = &trainer1;
            }
            if (opponentTrainer->kAlivePokemons == 0 || currentTrainer->kAlivePokemons == 0) {
                battleOver = true;
            }
            if (battleOver) {
                std::cout << "The battle is over!\n";
                break;
            }

            ReplaceIfDead(currentTrainer);
            ReplaceIfDead(opponentTrainer);

            DisplayBattleStatus(currentTrainer, opponentTrainer);

            HandleUserAction(currentTrainer, opponentTrainer);

            system("cls");

            HandleTurn(currentTrainer, opponentTrainer);

            turnCounter++;
        }
    }
private:
    // Function to get the user's chosen action
    ActionType GetUserAction() {
        int choice;
        std::cout << "Choose your action:" << std::endl;
        std::cout << "1. Switch Pokemon" << std::endl;
        std::cout << "2. Use Item" << std::endl;
        std::cout << "3. Attack" << std::endl;
        std::cout << "4. Quit" << std::endl;

        std::cin >> choice;

        switch (choice) {
            case 1:
                return ActionType::SwitchPokemon;
            case 2:
                return ActionType::UseItem;
            case 3:
                return ActionType::Attack;
            case 4:
                return ActionType::Quit;
            default:
                std::cout << "Invalid choice. Try again.\n";
                return GetUserAction();
        }
    }

    // Function to handle the user's chosen action
    void HandleUserAction(Trainer* currentTrainer, Trainer* opponentTrainer) {
        ActionType actionChoice = GetUserAction();

        switch (actionChoice) {
            case ActionType::SwitchPokemon:
                currentTrainer->SwitchPokemon();
                break;

            case ActionType::UseItem: {
                auto item = GetUserChoiceOfItem(currentTrainer);
                if(item->name == "Potion"){
                    auto target = GetUserChoiceOfPokemon(currentTrainer);
                    currentTrainer->UseHealingItem(reinterpret_cast<HealingItem*>(item), target);
                }else{
                    currentTrainer->UseBattleItem(reinterpret_cast<BattleItem*>(item), GetUserChoiceOfPokemon(opponentTrainer));
                }
                std::cout << currentTrainer->name << " used " << item->name << "\n";
                break;
            }

            case ActionType::Attack: {
                auto target = GetUserChoiceOfPokemon(opponentTrainer);
                HandlePokemonTurn(currentTrainer->activePokemon, target);
                break;
            }
            case ActionType::Quit:
                battleOver = true;
                break;

            default:
                std::cout << "Invalid action selected. Try again.\n";
                HandleUserAction(currentTrainer, opponentTrainer);
                break;
        }
    }
};


#endif //GAME_1_BATTLEARENA_H
