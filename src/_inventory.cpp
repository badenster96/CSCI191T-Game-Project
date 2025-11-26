#include "_inventory.h"

_inventory::_inventory()
{
    //ctor
}

_inventory::~_inventory()
{
    //dtor
}

void _inventory::initInv() {
    itemsMaster = fl.loadItems("items");
}
void _inventory::addItem(std::string itemName) {
    int newQ = inventory[itemName].quantity;
    if(inventory.find(itemName) != inventory.end()) inventory[itemName] = itemsMaster[itemName];
    inventory[itemName].quantity = newQ + 1;
    std::cout << "Added " << itemName << std::endl;
}
void _inventory::setPlayerStats(
    unordered_map<std::string, float>& player){
    for(const auto& item : inventory){
        for(const auto& stat : item.second.stats){
            player[stat.first] = stat.second * item.second.quantity;
        }
    }
}
