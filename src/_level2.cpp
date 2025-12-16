#include "_level2.h"
#include <random>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

_level2::_level2()
{
    scene = LEVEL2;
    isInit = false;
    enemies.resize(100);

    for(int i = 0; i < enemies.size(); i++){
        enemies.at(i) = new _enemy;
    }
    currentEnemy = 0;
}

_level2::~_level2() {}

void _level2::reSizeScene(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    this->width = width;
    this->height = height;
}

void _level2::initGL()
{
    ShowCursor(FALSE);
    glShadeModel(GL_SMOOTH);
    glClearColor(0,0,0,1);
    glClearDepth(2.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Load textures and skybox
    myTexture->loadTexture("images/tex.jpg");
    mySkyBox->skyBoxInit(200, "regSkybox", "jpg");

    myCam->camInit();
    snds->initSounds();
    for(int i = 0; i < enemies.size(); i++)enemies.at(i)->init("models/badboyblake/tris.MD2");
    std::cout << "Enemies: " << enemies.size() << std::endl;
    snds->playMusic("sounds/mainTheme.wav");
    isInit = true;
}
void _level2::focusCameraOnRandomEnemy() {
    enemies.at(currentEnemy)->spawn(myCam->des);
    myCam->des = enemies.at(currentEnemy)->pos;
    currentEnemy++;
    if(currentEnemy >= enemies.size()) currentEnemy = 0;
}

void _level2::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Setting up the camera, checking bool
    float moveSpeed = 0.15f;
    // camera relative forward and right vectors
    vec3 forwardVec(myCam->des - myCam->eye);
    forwardVec.normalize();

    vec3 rightVec;
    vec3 moveVec(0,0,0);
    // IJKL
    if(iKey){
        moveVec.z += forwardVec.z;
        moveVec.x += forwardVec.x;
    }
    if(kKey){
        moveVec.z -= forwardVec.z;
        moveVec.x -= forwardVec.x;
    }
    if(jKey){
        moveVec.z -= forwardVec.x;
        moveVec.x += forwardVec.z;
    }
    if(lKey){
        moveVec.z += forwardVec.x;
        moveVec.x -= forwardVec.z;
    }
    if(isJumping){
        myCam->des.y += 0.1f;
        myCam->eye.y += 0.1f;
    }
    if(isFalling){
        myCam->des.y -= 0.1f;
        myCam->eye.y -= 0.1f;
        if(myCam->des.y < 0) myCam->des.y = 0.0f;
    }

    if(moveVec.normalize() > 0.001f){
        myCam->des.x += moveVec.x * moveSpeed;
        myCam->des.z += moveVec.z * moveSpeed;
    }

    //WASD

    if(wKey || upKey){
        myCam->rotAngle.y -= myCam->step;
    }
    if(sKey || downKey){
        myCam->rotAngle.y += myCam->step;
    }
    if(aKey || leftKey){
        myCam->rotAngle.x += myCam->step;
    }
    if(dKey || rightKey){
        myCam->rotAngle.x -= myCam->step;
    }
    // Arrows
    if(upKey){

    }
    if(downKey){

    }
    if(leftKey){

    }
    if(rightKey){

    }
    myCam->rotateXY();
    myCam->setUpCamera();

    for(int i = 0; i < enemies.size(); i++){
        if(enemies.at(i)->isSpawned) enemies.at(i)->draw();
    }

    glPushMatrix();
        glDepthMask(GL_FALSE); // don't write depth for skybox
        mySkyBox->drawSkyBox();
        glDepthMask(GL_TRUE);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
    glPopMatrix();
}

void _level2::mouseMapping(int x,int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winZ;
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    glReadPixels(x, viewport[3]-y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);
    gluUnProject((GLfloat)x,(GLfloat)(viewport[3]-y),winZ,modelview,projection,viewport,&msX,&msY,&msZ);
}

int _level2::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg){
        case WM_KEYDOWN:
            if(wParam == VK_ESCAPE){
                scene = MAIN;
                isInit = false;
                return 0;
            }
            switch(wParam){
                case 'W': case 'w': wKey=true; break;
                case 'S': case 's': sKey=true; break;
                case 'A': case 'a': aKey=true; break;
                case 'D': case 'd': dKey=true; break;
                case 'J': case 'j': jKey=true; break;
                case 'K': case 'k': kKey=true; break;
                case 'L': case 'l': lKey=true; break;
                case 'I': case 'i': iKey=true; break;
                case VK_LEFT:       leftKey=true; break;
                case VK_RIGHT:      rightKey=true; break;
                case VK_UP:         upKey=true; break;
                case VK_DOWN:       downKey=true; break;
                case VK_SPACE:      isJumping=true; break;
                case VK_CONTROL:    isFalling=true; break;
                case 'M': case 'm': scene = MAIN; break;
            } break;

        case WM_KEYUP:
            switch(wParam){
                case 'W': case 'w': wKey=false; break;
                case 'S': case 's': sKey=false; break;
                case 'A': case 'a': aKey=false; break;
                case 'D': case 'd': dKey=false; break;
                case 'J': case 'j': jKey=false; break;
                case 'K': case 'k': kKey=false; break;
                case 'L': case 'l': lKey=false; break;
                case 'I': case 'i': iKey=false; break;
                case VK_LEFT:       leftKey=false; break;
                case VK_RIGHT:      rightKey=false; break;
                case VK_UP:         upKey=false; break;
                case VK_DOWN:       downKey=false; break;
                case VK_SPACE:      isJumping=false; break;
                case VK_CONTROL:    isFalling=false; break;
            } break;

        case WM_LBUTTONDOWN:
        {
            mouseMapping(LOWORD(lParam),HIWORD(lParam));
            bool isClick = true;
            focusCameraOnRandomEnemy();
            vec3 clicked{(float)msX,(float)msY,(float)msZ};


            break;
        }
        case WM_MOUSEMOVE:
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            // Calculate movement delta relative to the screen center
            int centerX = width / 2;
            int centerY = height / 2;

            int dx = x - centerX;
            int dy = y - centerY;

            myCam->rotAngle.x += dx * 0.2f;
            myCam->rotAngle.y -= dy * 0.2f;

            if(myCam->rotAngle.y > 89) myCam->rotAngle.y = 89;
            if(myCam->rotAngle.y < -89 ) myCam->rotAngle.y = -89;

            // Reset the mouse to the center of the window
            POINT centerPoint = { centerX, centerY };
            ClientToScreen(hWnd, &centerPoint);
            SetCursorPos(centerPoint.x, centerPoint.y);

            lastMouseX = centerX;
            lastMouseY = centerY;

            myInput->wParam = wParam;
            myInput->mouseMove(myModel, x, y);

            // Lock cursor to window client area
            lockCursor();
        }
            break;
    }
    return 0;
}
