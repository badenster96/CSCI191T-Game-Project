#include "_capsuleHandler.h"

_capsuleHandler::_capsuleHandler()
{
    //ctor
}

_capsuleHandler::~_capsuleHandler()
{
    //dtor
}

void _capsuleHandler::setup(int quantity){
    capsules.resize(quantity);
    for( int i = 0; i < capsules.size(); i++){
        _capsule* capsule = new _capsule;
        capsules.at(i) = capsule;
    }
}
void _capsuleHandler::draw(){
    for(const auto& c : capsules) {
        if(c->state != DESPAWNED)c->draw();
    }
}
void _capsuleHandler::update(){
    for(const auto& c : capsules){
        c->update();
    }
}
void _capsuleHandler::capsuleSpawner(int numCaps, vec3 point) {
    int spawned = 0;
    for(auto& c : capsules) {
        if(c->state == DESPAWNED && spawned < numCaps){
            cout << c->state << std::endl;
            c->spawn(point);
            spawned++;
        }
    }
}

bool _capsuleHandler::checkPickup(vec3 p,_collisionCheck* myCol) {
    // Check if the player is colliding with a capsule, and give the player an item if they are
    for(const auto& c : capsules){
        if(myCol->isSphereCol(p, c->pos, 2.0f, 2.0f, 1.0f) && c->state == ONGROUND){
            c->state = COLLECTED;
            return true;
        }
    }
    return false;
}
