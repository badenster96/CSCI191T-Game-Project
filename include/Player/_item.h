#ifndef _ITEM_H
#define _ITEM_H

#include <_common.h>
#include <Graphics/_sprite.h>
#include <string>

class _item : public _sprite
{
    public:
        _item();
        virtual ~_item();

        std::string itemTypeStr;
        ItemType type;
        std::string name;
        unordered_map<string,float> stats;
        int quantity = 0;
    protected:


    private:
};

#endif // _ITEM_H
