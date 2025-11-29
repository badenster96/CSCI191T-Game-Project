#ifndef _LEVEL_1_H
#define _LEVEL_1_H

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
#include<_camera.h>
#include<_bullets.h>
#include<_collisioncheck.h>
#include<_sounds.h>
#include <_scene.h>
#include <_enemy.h>
#include <_EnemyHandler.h>
#include <_inventory.h>
#include <_hud.h>
#include <_capsule.h>


class _level1 : public _Scene
{
    public:
        _level1();           //constructor
        virtual ~_level1();  //Destructor

        // Level objects
        _light *myLight             = new _light();   //light settings
        _model *myModel             = new _model();   //create a model
        _inputs *myInput            = new _inputs(); // input handle
        _textureLoader *myTexture   = new _textureLoader();// for loading images
        _textureLoader *myFloor     = new _textureLoader();
        _parallax *myPrlx           = new _parallax();
        _skyBox *mySkyBox           = new _skyBox();
        _timer *myTime              = new _timer();
        _camera *myCam              = new _camera();
        _collisionCheck *myCol      = new _collisionCheck();
        _player *mdl3D              = new _player();
        _3DModelLoader *mdl3DW      = new _3DModelLoader();
        _sounds *snds               = new _sounds();
        _EnemyHandler *enemyHandler = new _EnemyHandler();
        _sprite *mySprite           = new _sprite();
        _hud *myHUD                 = new _hud();
        _inventory *myInv           = new _inventory();
        _bullets b[10];
        std::vector<_capsule*> capsules;
        std::vector<_enemy*> enemies;
        std::vector<_sprite*> items;
        _enemy* nearestEnemy;

        // Init functions - functions that run once at init
        // void reSizeScene(int width, int height) override;  // resize window
        void initTextures();
        void initGL() override;                            // initialize GL graphics
        // Draw Functions - Functions that run every frame
        void lose();
        void enemyDamagePlayer(_player* player);
        void attackHandler();
        void capsuleSpawner(int range, int add);
        void itemFromCapsule();

        void drawSceneCalc();
        void drawFloor();
        void drawScene() override;                         // render scene
        // Input functions - Functions that handle input mapping
        void mouseMapping(int,int);
        int winMsg(HWND,UINT,WPARAM,LPARAM) override;      // to get keyboard interrupts and pass it to inputs

        // Wave spawning
        int currentWave = 0;
        int enemiesPerWave;
        float waveInterval;
        float lastWaveTime;
        int maxEnemies;
        // Attack Logic
        float lastAttackTime = 0.0f;
        float lastHitTime = 0.0f;
        float minDistance = 0.0f;
        double msX,msY,msZ;
    protected:

    private:
};

#endif // _LEVEL_1_H
