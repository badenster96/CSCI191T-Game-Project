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
        void convertItemStatsToStats();
        void addItemStatsToPlayer(unordered_map<std::string,float>& player);



    protected:
        std::unordered_map<std::string, _item> items;
        std::unordered_map<std::string, float> stats;
    private:
};

#endif // _INVENTORY_H
