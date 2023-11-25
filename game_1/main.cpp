#include "BattleArena.h"
#include <memory>

int main() {
    Trainer trainer1("Trainer_1");
    Trainer trainer2("Trainer_2");

    std::unique_ptr<FirePokemon> fire_pokemon1(new FirePokemon("Charizard_1", 10));
    std::unique_ptr<FirePokemon> fire_pokemon2(new FirePokemon("Charizard_2", 10));
    std::unique_ptr<ElectricPokemon> electric_pokemon1(new ElectricPokemon("Pikachu_1", 10));
    std::unique_ptr<ElectricPokemon> electric_pokemon2(new ElectricPokemon("Pikachu_2", 10));
    std::unique_ptr<GrassPokemon> grass_pokemon1(new GrassPokemon("Venasaur_1", 10));
    std::unique_ptr<GrassPokemon> grass_pokemon2(new GrassPokemon("Venasaur_2", 10));

    trainer1.AddPokemon(fire_pokemon1.get());
    trainer1.AddPokemon(electric_pokemon1.get());
    trainer1.AddPokemon(grass_pokemon1.get());

    trainer2.AddPokemon(fire_pokemon2.get());
    trainer2.AddPokemon(electric_pokemon2.get());
    trainer2.AddPokemon(grass_pokemon2.get());

    Inventory inventory1;
    Inventory inventory2;

    std::unique_ptr<HealingItem> potion_1(new HealingItem("Potion", 20));
    std::unique_ptr<HealingItem> potion_2(new HealingItem("Potion", 20));
    std::unique_ptr<BattleItem> bomb_1(new BattleItem("Bomb", 10));
    std::unique_ptr<BattleItem> bomb_2(new BattleItem("Bomb", 10));

    inventory1.AddItem(potion_1.get());
    inventory1.AddItem(bomb_1.get());

    inventory2.AddItem(potion_2.get());
    inventory2.AddItem(bomb_2.get());

    trainer1.SetInventory(inventory1);
    trainer2.SetInventory(inventory2);

    BattleArena arena(std::move(trainer1), std::move(trainer2));

    arena.StartBattle();

    return 0;
}
