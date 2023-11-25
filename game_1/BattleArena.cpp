#include "BattleArena.h"

void BattleArena::ApplyStatusConditions(Trainer* trainer) {
    for (Pokemon* pokemon : trainer->pokemons_) {
        if(pokemon->effect.type_ == EffectType::kShock){
            trainer->SwitchPokemonToRandom();
            pokemon->RemoveStatusConditions();
            return;
        }
        if(pokemon->HasStatusCondition() && pokemon->health_ > 0){
            pokemon->effect.duration_ -= 1;
            pokemon->health_ -= 5;
            if(pokemon->effect.duration_ == 0){
                pokemon->RemoveStatusConditions();
            }
        }
    }
}

void BattleArena::DisplayBattleStatus(Trainer* currentTrainer, Trainer* opponentTrainer) {
    std::cout << "Current Trainer: " << currentTrainer->name_ << "\n";
    // Display current trainer's active Pokemon and their health, status, etc.
    std::cout << "Active Pokemon: " << currentTrainer->activePokemon->name_ << " (Health: " << currentTrainer->activePokemon->health_ << ")\n";

    std::cout << "\n";

    // Display opponent trainer's active Pokemon and their health, status, etc.
    std::cout << "Opponent Trainer: " << opponentTrainer->name_ << "\n";
    std::cout << "Active Pokemon: " << opponentTrainer->activePokemon->name_ << " (Health: " << opponentTrainer->activePokemon->health_ << ")\n";
    std::cout << "\nYour pokemon:\n";

    //Display current trainer's pokemon and their status
    for(Pokemon* pokemon : currentTrainer->pokemons_){
        std::cout << pokemon->name_;
        if(!pokemon->isDead) {
            std::cout << "\tHealth: " << pokemon->health_ << "\tEffect: " << ShowEffect(pokemon->effect.type_) << "\n";
        }
        else{
            std::cout << "\tfainted";
        }
    }
    std::cout << "\n";
}

void BattleArena::HandlePokemonTurn(Pokemon* activePokemon, Pokemon* targetPokemon) {
    int damage = CalculateDamage(activePokemon, targetPokemon);
    activePokemon->ApplyEffect(targetPokemon);
    std::cout << activePokemon->name_ << " attacks " << targetPokemon->name_ << " for " << damage << " damage!\n";
    targetPokemon->health_ -= damage;
}
void BattleArena::HandleTurn(Trainer* currentTrainer, Trainer* opponentTrainer) {
    // Apply status conditions at the end of the turn
    ApplyStatusConditions(currentTrainer);
    ApplyStatusConditions(opponentTrainer);

    currentTrainer->RemovePokemon();
    opponentTrainer->RemovePokemon();
}
Pokemon* BattleArena::GetUserChoiceOfPokemon(Trainer* currentTrainer) {
    std::cout << "Choose a Pokemon:" << "\n";
    int index = 1;

    // Display the user's available Pokemon and their names
    for (Pokemon* pokemon : currentTrainer->pokemons_) {
        std::cout << index << ". " << pokemon->name_;
        if(!pokemon->isDead) {
            std::cout << " (Health " << pokemon->health_ << ")" << "\n";
        } else {
            std::cout << " fainted\n";
        }
        index++;
    }

    int choice;
    std::cin >> choice;

    Pokemon* chosen_pokemon = currentTrainer->pokemons_[choice - 1];

    // Validate the choice and return the selected Pokemon
    if (choice >= 1 && choice <= currentTrainer->pokemons_.size() && !chosen_pokemon->isDead) {
        return chosen_pokemon;
    }
    std::cout << "Invalid choice. Try again.\n";
    return GetUserChoiceOfPokemon(currentTrainer); // Recursive call for invalid choices
}
Item* BattleArena::GetUserChoiceOfItem(Trainer* currentTrainer) {
    std::cout << "Choose an item:\n";
    int index = 1;

    // Display the user's available items and their names
    for (Item* item : currentTrainer->inventory.items) {
        std::cout << index << ". " << item->name_ << "\n";
        index++;
    }

    int choice;
    std::cin >> choice;

    // Validate the choice and return the selected item
    if (choice >= 1 && choice <= currentTrainer->inventory.items.size()) {
        return currentTrainer->inventory.items[choice - 1];
    }
    std::cout << "Invalid choice. Try again.\n";
    return GetUserChoiceOfItem(currentTrainer); // Recursive call for invalid choices
}
void BattleArena::StartBattle() {
    Trainer* current_trainer = &trainer1;
    Trainer* opponent_trainer = &trainer2;
    while (!battleOver) {
        if(turnCounter % 2 == 0){
            current_trainer = &trainer1;
            opponent_trainer = &trainer2;
        }
        else{
            current_trainer = &trainer2;
            opponent_trainer = &trainer1;
        }
        if (opponent_trainer->kAlivePokemons_ == 0 || current_trainer->kAlivePokemons_ == 0) {
            battleOver = true;
        }
        if (battleOver) {
            std::cout << "The battle is over!\n";
            break;
        }

        current_trainer->ReplaceIfDead();
        opponent_trainer->ReplaceIfDead();

        DisplayBattleStatus(current_trainer, opponent_trainer);

        HandleUserAction(current_trainer, opponent_trainer);

//#ifdef _WIN32
//        system("cls");  // Clear the console on Windows
//#else
//        system("clear"); // Clear the console on Unix/Linux
//#endif

        HandleTurn(current_trainer, opponent_trainer);

        turnCounter++;
    }
}
ActionType BattleArena::GetUserAction() {
    int choice;
    std::cout << "Choose your action:" << '\n';
    std::cout << "1. Switch Pokemon" << '\n';
    std::cout << "2. Use Item" << '\n';
    std::cout << "3. Attack" << '\n';
    std::cout << "4. Quit" << '\n';

    std::cin >> choice;

    switch (choice) {
        case 1:
            return ActionType::kSwitchPokemon;
        case 2:
            return ActionType::kUseItem;
        case 3:
            return ActionType::kAttack;
        case 4:
            return ActionType::kQuit;
        default:
            std::cout << "Invalid choice. Try again.\n";
            return GetUserAction();
    }
}
void BattleArena::HandleUserAction(Trainer* currentTrainer, Trainer* opponentTrainer) {
    ActionType action_choice = GetUserAction();

    switch (action_choice) {
        case ActionType::kSwitchPokemon:
            currentTrainer->SwitchPokemon();
            break;

        case ActionType::kUseItem: {
            auto *item = GetUserChoiceOfItem(currentTrainer);
            if(item->name_ == "Potion"){
                Pokemon* target = GetUserChoiceOfPokemon(currentTrainer);
                currentTrainer->UseHealingItem(reinterpret_cast<HealingItem*>(item), target);
            }else{
                currentTrainer->UseBattleItem(reinterpret_cast<BattleItem*>(item), GetUserChoiceOfPokemon(opponentTrainer));
            }
            std::cout << currentTrainer->name_ << " used " << item->name_ << "\n";
            break;
        }

        case ActionType::kAttack: {
            auto *target = GetUserChoiceOfPokemon(opponentTrainer);
            HandlePokemonTurn(currentTrainer->activePokemon, target);
            break;
        }
        case ActionType::kQuit:
            battleOver = true;
            break;

        default:
            std::cout << "Invalid action selected. Try again.\n";
            HandleUserAction(currentTrainer, opponentTrainer);
            break;
    }
}
