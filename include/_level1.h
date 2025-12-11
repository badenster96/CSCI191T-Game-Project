#ifndef _LEVEL_1_H
#define _LEVEL_1_H

#include <vector>

#include <_common.h>
#include <_light.h>
#include <_model.h>
#include <_player.h>
#include <_inputs.h>
#include <_textureloader.h>
#include <_parallax.h>
#include <_skybox.h>
#include <_sprite.h>
#include <_timer.h>
#include <_3dmodelloader.h>
#include <_camera.h>
#include <_bullets.h>
#include <_collisioncheck.h>
#include <_sounds.h>
#include <_scene.h>
#include <_enemy.h>
#include <_hud.h>
#include <_inventory.h>

class _level1 : public _Scene
{
public:
    _level1();           // constructor
    virtual ~_level1();  // destructor

    // Core objects (mirroring level3 style)
    _light          *myLight   = new _light();
    _model          *myModel   = new _model();
    _inputs         *myInput   = new _inputs();
    _textureLoader  *myTexture = new _textureLoader();
    _parallax       *myPrlx    = new _parallax();
    _skyBox         *mySkyBox  = new _skyBox();
    _timer          *myTime    = new _timer();
    _camera         *myCam     = new _camera();
    _collisionCheck *myCol     = new _collisionCheck();
    _player         *mdl3D     = new _player();
    _3DModelLoader  *mdl3DW    = new _3DModelLoader();
    _sounds         *snds      = new _sounds();
    _sprite         *mySprite  = new _sprite();
    _hud            *myHUD     = new _hud();
    _inventory      *myInv     = new _inventory();
    _bullets         b[10];

    // Dummy enemy list so HUD can accept a valid pointer
    std::vector<_enemy*> enemies;

    // Teapots
    static const int MAX_TEAPOTS = 30;
    _model fallingTeapots[MAX_TEAPOTS];
    bool   teapotActive[MAX_TEAPOTS];

    // Bullet firing
    int clickCnt = 0;

    // Wave spawning for teapots
    float waveInterval = 3.0f;      // seconds between waves
    float lastWaveTime = 0.0f;

    // Window size
    int width  = 800;
    int height = 600;

    // Overrides from _Scene
    void reSizeScene(int width, int height) override;
    void initFiles();
    void initGL() override;
    void drawScene() override;
    int  winMsg(HWND, UINT, WPARAM, LPARAM) override;

    // Helpers
    void mouseMapping(int x, int y);
    double msX, msY, msZ;

    void initTeapots();
    void resetTeapot(int index);
    void updateTeapots();
    void handleTeapotHit(int index);
    void handleTeapotMiss(int index);
    void spawnWaveIfNeeded();
    void drawFloor();
    void drawSceneCalc();
    int  findNearestTeapot();   // for simple auto-aim

    std::unordered_map<std::string, char*> files;

protected:

private:
};

#endif // _LEVEL_1_H
