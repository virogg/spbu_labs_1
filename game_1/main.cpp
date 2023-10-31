#include <iostream>
#include <Windows.h>
#include "BattleArena.h"


int main() {
    Trainer trainer1("Trainer_1");
    Trainer trainer2("Trainer_2");

    FirePokemon firePokemon1("Charizard_1", 10), firePokemon2("Charizard_2", 10);
    ElectricPokemon electricPokemon1("Pikachu_1", 10), electricPokemon2("Pikachu_2", 10);
    GrassPokemon grassPokemon1("Venasaur_1", 10), grassPokemon2("Venasaur_2", 10);

    trainer1.AddPokemon(&firePokemon1);
    trainer1.AddPokemon(&electricPokemon1);
    trainer1.AddPokemon(&grassPokemon1);

    trainer2.AddPokemon(&firePokemon2);
    trainer2.AddPokemon(&electricPokemon2);
    trainer2.AddPokemon(&grassPokemon2);

    Inventory inventory1;
    Inventory inventory2;

    HealingItem potion_1("Potion", 20);
    HealingItem potion_2("Potion", 20);
    BattleItem bomb_1("Bomb", 10);
    BattleItem bomb_2("Bomb", 10);

    inventory1.AddItem(&potion_1);
    inventory1.AddItem(&bomb_1);

    inventory2.AddItem(&potion_2);
    inventory2.AddItem(&bomb_2);

    trainer1.SetInventory(inventory1);
    trainer2.SetInventory(inventory2);

    BattleArena arena(trainer1, trainer2, trainer1.name, trainer2.name);

    arena.StartBattle();

    return 0;
}
