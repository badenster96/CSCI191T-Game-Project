#include "_player.h"

_player::_player()
{
    //ctor
    stats["AttackSpeed"] = 20.0f;
}

_player::~_player()
{
    //dtor
}
void _player::init(const char* model) {

    initModel(model);
}
void _player::draw(){

}
