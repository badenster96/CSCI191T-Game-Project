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
void _level1::enemyDamagePlayer(float currentTime, _player* player){
    if(currentTime - lastHitTime >= player->iFrames){
        for(const auto& e : enemyHandler->enemies){
            if(e && myCol->isSphereCol(player->pos,e->pos, 1.0f, 1.0f, 2.0f)){
                lastHitTime = currentTime;
                std::cout << "At:" << lastHitTime << " | Player hit for " << nearestEnemy->damage << " damage!" << std::endl;
                player->hit(e->damage);
            }
        }
    }
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

    // Fog
    glEnable(GL_FOG);
    GLfloat fogColor[] = {0.01f, 0.08f, 0.01f, 0.8f};

    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_START, 20.0f);
    glFogf(GL_FOG_END, 200.0f);

    // Set matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)width / (float)height;
    gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    enemyHandler->setup(100);
    capsules.resize(100);
    for( int i = 0; i < capsules.size(); i++){
        _capsule* capsule = new _capsule;
        capsules.at(i) = capsule;
    }
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
    myHUD->init();

    // Level stats setup
    waveInterval = 3.0f;
    lastWaveTime = 0.0f;
    mdl3D->applyPlayerStats();
    isInit = true;
}

void _level1::drawSceneCalc(){
    if(mdl3D && mdl3D->currHealth <= 0.0f){
        scene = MAIN;
        isInit = false;
        return;
    }
    // Enemy stats
    int rangeEnemiesPerCapsule = 4;
    float minEnemiesPerCapsule = 5;

    mdl3D->update();
    mySprite->face(mdl3D->getPos());


    // Collision Check to Sprites, to pick up items
    // std::cout << myCol->isSphereCol(mdl3D->pos,mySprite->pos,1.0f,1.0f,0.1f) << std::endl;

    //Set up enemy waves
    float currentTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;


    // Attack logic, if there is a nearestEnemy
    nearestEnemy = enemyHandler->nearest(mdl3D->pos);
    if(nearestEnemy){
        mdl3D->setTarget(nearestEnemy->pos);
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
        // Enemy hit handler
        // function in enemyHandler that takes in
        enemyDamagePlayer(currentTime,mdl3D);
    }

    // Spawn capsules
    int capsulesPerWave = rand()%5 + 5;
    if(currentTime - lastWaveTime >= waveInterval){
        int spawned = 0;
        for(auto& c : capsules) {
            if(!c->isSpawned && spawned < capsulesPerWave){
                if(enemyHandler->canSpawn())c->spawn(mdl3D->pos);
                spawned++;
            }
        }
        lastWaveTime = currentTime;
    }
    // Spawn random number of enemies at capsule
    for(const auto& c : capsules){
        c->update(currentTime);
        if(c->getState() == 2) {
            vec3 capsulePos(c->posX,c->posY,c->posZ);
            enemyHandler->calc(rangeEnemiesPerCapsule, minEnemiesPerCapsule, capsulePos);
            std::cout << "Wave Spawned at Capsule" <<std::endl;
            c->state = SPAWNED;
        }
    }
    mdl3D->applyPlayerStats();
}

void _level1::drawScene()
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    drawSceneCalc();

    // |====================================================|
    // |----------------------DRAW SECTION------------------|
    // |====================================================|

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
        //myPrlx->drawParallax(width, height);

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
    mySprite->drawSprite();
    mdl3D->draw();
    // Enemy Render
    enemyHandler->draw(mdl3D->pos);
    for(const auto& c : capsules) {
        c->draw();
    }
    glDisable(GL_FOG);
    myHUD->draw(width, height);
    glEnable(GL_FOG);

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
            if(wParam == 'm' || wParam == 'M')
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
