#include "_inventory.h"

_inventory::_inventory()
{
    //ctor
    stats["Health"]         = 0.0f;
    stats["Speed"]          = 0.0f;
    stats["AttackSpeed"]    = 0.0f;
    stats["Damage"]         = 0.0f;
    stats["CriticalChance"] = 0.0f;
    stats["Armor"]          = 0.0f;
    stats["ArmorPiercing"]  = 0.0f;
}

_inventory::~_inventory()
{
    //dtor
}

void _inventory::initInv() {
    // Create a list of all of the items in the game
    _item addItem;
    // Spinach
    addItem.type = GIZMO;
    addItem.stats["Health"] = 25.0f;
    addItem.stats["Speed"]  = 0.1f;
    items["Spinach"] = addItem;
}
void _inventory::convertItemStatsToStats() {

}
void _inventory::addItemStatsToPlayer(unordered_map<std::string, float> &player){

}
