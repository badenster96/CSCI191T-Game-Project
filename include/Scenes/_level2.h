#ifndef _LEVEL_2_H
#define _LEVEL_2_H

#include<_common.h>
#include<Graphics/_light.h>
#include<Graphics/_model.h>
#include <Player/_player.h>
#include<UI/_inputs.h>
#include<Graphics/_textureloader.h>
#include<Graphics/_parallax.h>
#include<Graphics/_skybox.h>
#include<Graphics/_sprite.h>
#include<_timer.h>
#include<Graphics/_3dmodelloader.h>
#include<UI/_camera.h>
#include<Combat/_bullets.h>
#include<_collisioncheck.h>
#include<_sounds.h>
#include <Scenes/_scene.h>
#include <Combat/_enemy.h>


class _level2 : public _Scene
{
    public:
        _level2();           //constructor
        virtual ~_level2();  //Destructor

        _light *myLight             = new _light();   //light settings
        _model *myModel             = new _model();   //create a model
        _inputs *myInput            = new _inputs(); // input handle
        _textureLoader *myTexture   = new _textureLoader();// for loading images
        _textureLoader *myFloor     = new _textureLoader();
        _parallax *myPrlx           = new _parallax();
        _skyBox *mySkyBox           = new _skyBox();
        _sprite *mySprite           = new _sprite();
        _timer *myTime              = new _timer();
        _camera *myCam              = new _camera();
        _collisionCheck *myCol      = new _collisionCheck();

        _player *mdl3D              = new _player();
        _3DModelLoader *mdl3DW      = new _3DModelLoader();

        _sounds *snds               = new _sounds();

        _bullets b[10];
        std::vector<_enemy*> enemies;

        int clickCnt =0;

        void reSizeScene(int width, int height) override;  // resize window
        void initGL() override;                            // initialize GL graphics
        void drawScene() override;                         // render scene
        int winMsg(HWND,UINT,WPARAM,LPARAM) override;      // to get keyboard interrupts and pass it to inputs
        void updateCameraOrbit();
        void updateCameraMovement();
        void mouseMapping(int,int);
        double msX,msY,msZ;
        void playerMoveForward(float speed);
        void playerMoveBack(float speed);
        void playerMoveLeft(float speed);
        void playerMoveRight(float speed);
        void focusCameraOnRandomEnemy();

        int width, height;  // keep record of the screen size

        float currentPlayerAngle = 0.0f;
        int currentEnemy;
        // Camera Controls
        int lastMouseX, lastMouseY = 0;
        bool isDraggingOrbit;
        // Camera orbit variables
        float orbitCurrentYaw = 0.0f;    // current horizontal angle (degrees)
        float orbitPitch = 0.0f;         // current vertical angle (degrees)
        float targetOrbitPitch = 0.0f;   // target vertical angle (degrees)

        bool wKey, sKey, aKey, dKey;
        bool iKey, jKey, kKey, lKey;
        bool upKey, downKey, leftKey, rightKey;
        bool isJumping, isFalling;
    protected:


    private:
};

#endif // _LEVEL_2_H
