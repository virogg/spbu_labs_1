#ifndef GAME_1_BATTLEARENA_H
#define GAME_1_BATTLEARENA_H

#include "Trainer.h"
#ifdef _WIN32
#include <Windows.h>  // Required for system("cls") on Windows
#else
#include <cstdlib>     // Required for system("clear") on Unix/Linux
#endif

enum class ActionType {
    kSwitchPokemon,
    kUseItem,
    kAttack,
    kQuit
};

class BattleArena {
public:
    bool battleOver = false;
    int turnCounter = 0;
    Trainer trainer1;
    Trainer trainer2;

    BattleArena(Trainer&& tr1, Trainer&& tr2) : trainer1(std::move(tr1)), trainer2(std::move(tr2)) {}

    // Function to display the current state of the battle
    static void DisplayBattleStatus(Trainer* currentTrainer, Trainer* opponentTrainer);

    // Function to calculate damage based on the type advantages, attack power, etc.
    inline int CalculateDamage(Pokemon* attacker, Pokemon* enemy) {
        int damage = (attacker->level_ * attacker->attackPower) - (enemy->level_ * enemy->defensePower);
        return (damage < 0) ? 0 : damage;
    }

    // Function to check and apply status effects (e.g., poison, burn) at the end of each turn
    void ApplyStatusConditions(Trainer* trainer);

    // Function to handle a Pokemon's turn
    void HandlePokemonTurn(Pokemon* activePokemon, Pokemon* targetPokemon);

    // Function to handle the overall flow of a turn in the battle
    void HandleTurn(Trainer* currentTrainer, Trainer* opponentTrainer);

    Pokemon* GetUserChoiceOfPokemon(Trainer* currentTrainer);

    // Function to get the user's choice of item
    Item* GetUserChoiceOfItem(Trainer* currentTrainer);

    void StartBattle();
private:
    // Function to get the user's chosen action
    ActionType GetUserAction();

    // Function to handle the user's chosen action
    void HandleUserAction(Trainer* currentTrainer, Trainer* opponentTrainer);
};


#endif //GAME_1_BATTLEARENA_H
