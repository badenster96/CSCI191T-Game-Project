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
