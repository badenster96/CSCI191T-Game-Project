#include "_level1.h"

_level1::_level1()
{
    //ctor
    myTime->startTime = clock();
    isInit = false;
    nearestEnemy = nullptr;
}

_level1::~_level1()
{
    //dtor
}
void _level1::initGL()
{
    scene = LEVEL1;
    std::cout << "Initializing Level1..." << std::endl;
    lockCursor();
    glShadeModel(GL_SMOOTH); // to handle GPU shaders
    glClearColor(0.0f,0.0f,0.0f,1.0f); // black background color
    glClearDepth(1.0f);         //depth test for layers

    glEnable(GL_DEPTH_TEST);    //activate depth test
    glDepthFunc(GL_LEQUAL);     // depth function type

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)width / (float)height;
    gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    enemyHandler->setup(100);
    myHUD->setPlayer(mdl3D);
    myHUD->setEnemies(&enemyHandler->enemies);

    myTexture->loadTexture("images/tex.jpg");
    // myFloor -> loadTexture("images/");
    myPrlx->parallaxInit("images/prlx.jpg");

    mySkyBox->skyBoxInit();
    mySkyBox->tex[0] = mySkyBox->textures->loadTexture("images/front.png");
    mySkyBox->tex[1] = mySkyBox->textures->loadTexture("images/back.png");
    mySkyBox->tex[2] = mySkyBox->textures->loadTexture("images/top.png");
    mySkyBox->tex[3] = mySkyBox->textures->loadTexture("images/bottom.png");
    mySkyBox->tex[4] = mySkyBox->textures->loadTexture("images/right.png");
    mySkyBox->tex[5] = mySkyBox->textures->loadTexture("images/left.png");
    mySkyBox->tex[6] = mySkyBox->textures->loadTexture("images/Stairs.png");

    mySprite->spriteInit("images/eg.png",6,4);
    mdl3D->init("models/GiJoe/tris.md2");
    mdl3DW->initModel("models/Tekk/weapon.md2");
    enemyHandler->initModels("models/badboyblake/tris.MD2");

    snds->initSounds();
    snds->playMusic("sounds/mainTheme.wav");

    // Level stats setup
    waveInterval = 3.0f;
    lastWaveTime = 0.0f;
    isInit = true;
}

void _level1::drawSceneCalc(){
    if(mdl3D && mdl3D->currHealth <= 0.0f){
        scene = MAIN;
        isInit = false;
        return;
    }
    // Enemy stats
    int rangeEnemiesPerWave = 40;
    float minEnemiesPerWave = 50;

    mdl3D->update();
    mySprite->face(mdl3D->getPos());


    // Collision Check to Sprites, to pick up items
    // std::cout << myCol->isSphereCol(mdl3D->pos,mySprite->pos,1.0f,1.0f,0.1f) << std::endl;

    //Set up enemy waves
    enemyHandler->calc(rangeEnemiesPerWave, minEnemiesPerWave, waveInterval, mdl3D->pos);
    nearestEnemy = nullptr;
    mdl3D->setTarget(enemyHandler->nearest(mdl3D->pos)->pos);
    nearestEnemy = enemyHandler->nearest(mdl3D->pos);
    float currentTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;

    // Hit enemies with attacks



    // Enemy hit handler
    if(currentTime - lastHitTime >= mdl3D->iFrames){
        for(const auto& e : enemyHandler->enemies){
            if(myCol->isSphereCol(mdl3D->pos,e->pos, 1.0f, 1.0f, 2.0f)){
                lastHitTime = currentTime;
                std::cout << "At:" << lastHitTime << " | Player hit for " << nearestEnemy->damage << " damage!" << std::endl;
                mdl3D->hit(e->damage);
            }
        }
    }
    for(int i = 0; i < 10; i++){
        if(nearestEnemy && !b[i].live && currentTime - lastAttackTime >= 1/mdl3D->attackSpeed){
            if(nearestEnemy->isAlive) b[i].shootBullet(mdl3D->pos, nearestEnemy->pos);
            lastAttackTime = currentTime;
        }
        if(b[i].live){
            for(const auto& e : enemyHandler->enemies){
                if(myCol->isSphereCol(b[i].pos,e->pos,1.0f,1.0f,0.1f)
                   && currentTime - e->lastTimeHit >= e->iFrames
                   && e->health > 0){
                    e->health -= mdl3D->damage;
                    e->lastTimeHit = currentTime;
                    if(e->health <= 0) e->isAlive = e->isSpawned = false;
                }
            }
        }
    }
}

void _level1::drawScene()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawSceneCalc();


    // |====================================================|
    // |----------------------DRAW SECTION------------------|
    // |====================================================|

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

    glPushMatrix();

        // Calculate horizontal offset based on camera rotation
        float lookOffset = fmod(mdl3D->cam.rotAngle.x, 360.0f);  // 0-360
        if(lookOffset < 0) lookOffset += 360.0f;             // make positive
        lookOffset /= 360.0f;                                // 0-1 range
        lookOffset = lookOffset * 2.0f - 1.0f;              // -1 to 1

        float parallaxFactor = 0.5f; // adjust intensity of movement
        myPrlx->xMin = -lookOffset * parallaxFactor;
        myPrlx->xMax = myPrlx->xMin + 1.0f;

        // Draw parallax
        myPrlx->drawParallax(width, height);

        // Draw skybox on top (optional)
        glDepthMask(GL_FALSE); // don't write depth for skybox
        // mySkyBox->drawSkyBox();
        glDepthMask(GL_TRUE);
        // Floor
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glDisable(GL_LIGHTING);
            myTexture->bindTexture(); // your floor texture

            glColor3f(1.0f, 1.0f, 1.0f);

            float floorSize = 500.0f;  // how big the floor is
            float repeat = 100.0f;     // how many times the texture repeats
            float floorHeight = -2.8f;

            glBegin(GL_QUADS);
                glNormal3f(0.0f, 1.0f, 0.0f); // point up

                glTexCoord2f(0.0f, 0.0f); glVertex3f(-floorSize, floorHeight, -floorSize);
                glTexCoord2f(repeat, 0.0f); glVertex3f(floorSize, floorHeight, -floorSize);
                glTexCoord2f(repeat, repeat); glVertex3f(floorSize, floorHeight, floorSize);
                glTexCoord2f(0.0f, repeat); glVertex3f(-floorSize, floorHeight, floorSize);
            glEnd();
            glEnable(GL_LIGHTING);
        glPopMatrix();
    glPopMatrix();
    glPushMatrix();
        for(int i = 0; i < 10; i++){
            if(b[i].live){
                b[i].bulletActions();
                b[i].drawBullet();
            }
        }
    glPopMatrix();

    mdl3D->draw();
    mySprite->drawSprite();
    // Enemy Render
    enemyHandler->draw();
    myHUD->draw(width, height);

}

void _level1::mouseMapping(int x, int y)

{
    GLint viewPort[4];
    GLdouble ModelViewM[16];
    GLdouble projectionM[16];
    GLfloat winX,winY,winZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, ModelViewM);
    glGetDoublev(GL_PROJECTION_MATRIX,projectionM);
    glGetIntegerv(GL_VIEWPORT,viewPort);

    winX =(GLfloat)x;
    winY = (GLfloat)y;

    glReadPixels(x,(int)winY,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winZ);
    gluUnProject(winX,winY,winZ,ModelViewM,projectionM,viewPort,&msX,&msY,&msZ);
}

int _level1::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(mdl3D){
        mdl3D->handleInput(uMsg, wParam, lParam, hWnd);
    }
    switch(uMsg)
    {
        case WM_KEYDOWN:
            if(wParam == VK_ESCAPE){
                scene = MAIN;
                isInit = false;
                return 0;
            }
            myInput->wParam = wParam;
            myInput->keyPressed(myModel);
            myInput->keyPressed(myPrlx);
            // myInput->keyPressed(mySkyBox);
            myInput->keyPressed(mySprite);
            // myInput->keyPressed(myCam);
            myInput->keyPressed(mdl3D,mdl3DW);
            if(wParam == 'm' || wParam == 'M') scene = LEVEL2;
        break;

        case WM_KEYUP:
            myInput->wParam = wParam;
            myInput->keyUp(mySprite);
        break;

        case WM_LBUTTONDOWN:
            myInput->wParam = wParam;
            myInput->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));

             mouseMapping(LOWORD(lParam), HIWORD(lParam));
        break;

        case WM_RBUTTONDOWN:
            myInput->wParam = wParam;
            myInput->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));
        break;

         case WM_MBUTTONDOWN:
             myInput->wParam = wParam;
             myInput->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));

            break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            myInput->wParam = wParam;
            myInput->mouseEventUp();
            break;

        case WM_MOUSEMOVE: {
            if(scene == LEVEL1){
                lockCursor();
            }
        }
            break;
        case WM_MOUSEWHEEL:
              myInput->wParam = wParam;
              myInput->mouseWheel(myModel,(double)GET_WHEEL_DELTA_WPARAM(wParam));
            break;

        default:
            break;

    }
    return 0;
}
