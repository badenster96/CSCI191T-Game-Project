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
    // Create a list of all of the items in the game
    _item addItem;
    // Spinach
    addItem.type = GIZMO;
    addItem.stats["Health"] = 25.0f;
    addItem.stats["Speed"]  = 0.1f;
    items["Spinach"] = addItem;
}
