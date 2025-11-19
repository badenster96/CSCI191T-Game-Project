#ifndef _INVENTORY_H
#define _INVENTORY_H

#include <_item.h>
#include <_common.h>

class _inventory
{
    public:
        _inventory();
        virtual ~_inventory();
        void initInv();
        _item getItem(std::string itemName){return items[itemName];}



    protected:
        std::unordered_map<std::string, _item> items;
    private:
};

#endif // _INVENTORY_H
