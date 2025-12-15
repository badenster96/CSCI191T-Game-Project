#include "_inventory.h"

_inventory::_inventory()
{
    //ctor
}

_inventory::~_inventory()
{
    //dtor
}
void _inventory::loadItemSprites() {
    for(auto& i : itemsMaster){
        std::string spriteFile = "items/" + i.first + ".png";
        i.second.spriteInit(spriteFile.data(), 1, 1);
    }
}
void _inventory::initInv() {
    if(!isLoaded) itemsMaster = fl.loadItems("items");

    isLoaded = true;
}

void _inventory::addItem(_item item) {
    std::string itemName = item.name;
    int newQ = inventory[itemName].quantity;
    if(inventory.find(itemName) != inventory.end()) inventory[itemName] = itemsMaster[itemName];
    inventory[itemName].quantity = newQ + 1;
}
void _inventory::setPlayerStats(
    unordered_map<std::string, float>& player){
    for(const auto& item : inventory){
        for(const auto& stat : item.second.stats){
            player[stat.first] = stat.second * item.second.quantity;
        }
    }
}
void _inventory::resetItems(){
    inventory.clear();
}
_item _inventory::randomItem() {
    int randomItem = rand()%itemsMaster.size();
    int itemNumber = 0;
    for(const auto& i : itemsMaster){
        if(itemNumber == randomItem) return i.second;
        else itemNumber++;
    }

}
_item _inventory::pickupItem(){
    std::vector<_item> pickupChoices;
    _item pickupItem = randomItem();
    int iteminput;
    pickupChoices.clear();
    for(int i = 0; i < 3; i++){
        pickupChoices.push_back(randomItem());
    }
    addItem(pickupItem);
    return randomItem();
}
