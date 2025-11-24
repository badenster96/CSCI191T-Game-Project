#ifndef _ITEM_H
#define _ITEM_H

#include <_common.h>
#include <_sprite.h>

class _item : public _sprite
{
    public:
        _item();
        virtual ~_item();

        Type type;
        unordered_map<string,float> stats;
    protected:


    private:
};

#endif // _ITEM_H
