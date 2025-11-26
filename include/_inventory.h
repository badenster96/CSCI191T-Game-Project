#ifndef _INVENTORY_H
#define _INVENTORY_H

#include <_item.h>
#include <_common.h>
#include<_FileLoader.h>
#include <_player.h>

class _inventory
{
    public:
        _inventory();
        virtual ~_inventory();
        void initInv();
        _item getItem(std::string itemName){return itemsMaster[itemName];}
        void convertItemStatsToStats();
        void addItem(std::string itemName);
        void setPlayerStats(unordered_map<std::string,float>& player);
        _FileLoader fl;


    protected:
        std::unordered_map<std::string, _item> itemsMaster;
        std::unordered_map<std::string, _item> inventory;
        std::unordered_map<std::string, float> playerStats;
    private:
};

#endif // _INVENTORY_H
