#ifndef _LEVEL_3_H
#define _LEVEL_3_H

#include<_common.h>
#include<_light.h>
#include<_model.h>
#include <_player.h>
#include<_inputs.h>
#include<_textureloader.h>
#include<_parallax.h>
#include<_skybox.h>
#include<_sprite.h>
#include<_timer.h>
#include<_3dmodelloader.h>
#include<_bullets.h>
#include<_collisioncheck.h>
#include<_sounds.h>
#include <_scene.h>
#include <_enemy.h>
#include <_EnemyHandler.h>
#include <_inventory.h>
#include <_hud.h>
#include <_capsule.h>
#include <_capsuleHandler.h>
#include <_waveHandler.h>


class _level3 : public _Scene
{
    public:
        _level3();           //constructor
        virtual ~_level3();  //Destructor

        // Level objects
        //_light *myLight             = new _light();   //light settings
        //_model *myModel             = new _model();   //create a model
        _inputs *myInput            = new _inputs(); // input handle
        _textureLoader *myTexture   = new _textureLoader();// for loading images
        //_textureLoader *myFloor     = new _textureLoader();
        //_parallax *myPrlx           = new _parallax();
        _skyBox *mySkyBox           = new _skyBox();
        _timer *myTime              = new _timer();
        _collisionCheck *myCol      = new _collisionCheck();
        _player *player              = new _player();
        _sounds *snds               = new _sounds();
        _EnemyHandler *enemyHandler = new _EnemyHandler();
        _capsuleHandler *capsuleHandler = new _capsuleHandler();
        //_sprite *mySprite           = new _sprite();
        _hud *myHUD                 = new _hud();
        _inventory *myInv           = new _inventory();
        _waveHandler* myWave        = new _waveHandler();
        _bullets b[10];
        std::vector<_sprite*> items;
        _enemy* boss = new _enemy();
        std::unordered_map<std::string, char*> myFiles;
        _enemy* nearestEnemy;

        // Init functions - functions that run once at init
        void* getPlayer() override{return player;}
        void* getInventory() override{return myInv;}
        void init(std::unordered_map<std::string, char*> files);
        void init();
        void initFiles();
        void initGL() override;                            // initialize GL graphics
        // Draw Functions - Functions that run every frame
        void winLossCheck();
        void enemyDamagePlayer(_player* player);
        void attackHandler(vec3 nearestE, vec3 player);
        void waveSpawn();
        void capsuleSpawner(int range, int add);
        void pickupMenu();

        void update();
        void clampLevel();
        void drawFloor();
        void drawScene() override;                         // render scene
        // Input functions - Functions that handle input mapping
        //void mouseMapping(int,int);
        int winMsg(HWND,UINT,WPARAM,LPARAM) override;      // to get keyboard interrupts and pass it to inputs

        // Attack Logic
        float lastAttackTime = 0.0f;
        float minDistance = 0.0f;
        double msX,msY,msZ;
        // World Boundaries
        int boundarySize;
        vec3 minBound;
        vec3 maxBound;

        // Menu
    protected:

    private:
};

#endif // _LEVEL_1_H
