#include "_level3.h"

_level3::_level3()
{
    //ctor
    myTime->startTime = clock();
    isInit = false;
    nearestEnemy = nullptr;
    isPickupMenuOpen = false;
}

_level3::~_level3()
{
    //dtor
}

void _level3::initTextures() {
    myHUD->addConsoleMessage("Loading Textures...");
    myTexture->loadTexture("images/tex.jpg");
    myPrlx->parallaxInit("images/prlx.jpg");

    mySkyBox->skyBoxInit();
    mySkyBox->tex[0] = mySkyBox->textures->loadTexture("images/front.jpg");
    mySkyBox->tex[1] = mySkyBox->textures->loadTexture("images/back.jpg");
    mySkyBox->tex[2] = mySkyBox->textures->loadTexture("images/top.jpg");
    mySkyBox->tex[3] = mySkyBox->textures->loadTexture("images/bottom.jpg");
    mySkyBox->tex[4] = mySkyBox->textures->loadTexture("images/right.jpg");
    mySkyBox->tex[5] = mySkyBox->textures->loadTexture("images/left.jpg");
    mySkyBox->tex[6] = mySkyBox->textures->loadTexture("images/Stairs.jpg");
    for (int i = 0; i < 6; i++) {
        myHUD->addConsoleMessage("Skybox tex[" + std::to_string(i) + "] = " + std::to_string(mySkyBox->tex[i]));
    }
    mySprite->spriteInit("images/eg.png",6,4);
    mdl3D->init("waste");
    //mdl3DW->initModel("models/Tekk/weapon.md2");
    for(int i = 0; i < 10; i++){
        b[i].iniBullet("models/Tekk/weapon.md2");
    }
    enemyHandler->initModels("badboyblake");
    // capsuleHandler->init();
    myHUD->addConsoleMessage("Textures loaded.");
}
void _level3::initGL() {
    myHUD->addConsoleMessage("Initializing Level3...");
    scene = LEVEL3;
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

    enemyHandler->setup(200);
    capsuleHandler->setup(100);

    myHUD->setPlayer(mdl3D);
    myHUD->setEnemies(&enemyHandler->enemies);

    initTextures();


    snds->initSounds();
    // snds->playMusic("sounds/mainTheme.wav");
    myHUD->init();
    myInv->initInv();

    // Level stats setup
    mdl3D->applyPlayerStats();
    isInit = true;
    myHUD->addConsoleMessage("_level3 initialized");
}
void _level3::lose() {
    if(mdl3D && mdl3D->currHealth <= 0.0f){
        mdl3D->resetPlayer();
        myInv->resetItems();
        scene = MAIN;
        isInit = false;
        return;
    }
}
void _level3::enemyDamagePlayer(_player* player){
    float currentTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    if(currentTime - lastHitTime >= player->iFrames){
        for(const auto& e : enemyHandler->enemies){
            if(e && myCol->isSphereCol(player->pos,e->pos, 1.0f, 1.0f, 1.0f) && e->isAlive){
                lastHitTime = currentTime;
                std::string message = "At:" + to_string(std::round(lastHitTime * 100.0f) / 100.0f) + " | Player hit for " + to_string((int)nearestEnemy->damage) + " damage!";
                myHUD->addConsoleMessage(message);
                player->hit(e->damage);
            }
        }
    }
}
void _level3::fireBullet() {

}
void _level3::attackHandler(vec3 nearestE, vec3 player) {
    float currentTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    if(nearestEnemy){
        mdl3D->setTarget(nearestEnemy->pos);
        for(int i = 0; i < 10; i++){
            if(nearestEnemy && !b[i].live && currentTime - lastAttackTime >= 1/mdl3D->attackSpeed){
                if(nearestEnemy->isAlive) b[i].shootBullet(player, nearestE);
                lastAttackTime = currentTime;
            }
            // fires the bullets
            if(b[i].live){
                for(const auto& e : enemyHandler->enemies){
                    if(myCol->isSphereCol(b[i].pos,e->pos,1.0f,1.0f,0.1f)
                       && currentTime - e->lastTimeHit >= e->iFrames
                       && e->health > 0){
                        e->health -= mdl3D->damage;
                        e->pain();
                        e->lastTimeHit = currentTime;
                        //myHUD->addConsoleMessage("Enemy Shot!");
                        if(e->health <= 0) {
                            e->isAlive = e->isSpawned = false;
                            enemiesKilled++;
                        }
                    }
                }
            }
        }
        // Enemy hit handler
        // function in enemyHandler that takes in
        enemyDamagePlayer(mdl3D);
    }
}
void _level3::waveSpawn() {
    int enemiesPerWave = 12 * wave;
    int enemiesPerCapsule = 4 * wave;
    if(!waveSpawned){
        myHUD->addConsoleMessage("Wave " + std::to_string(wave) + " Spawned");
        int capsulesPerWave = enemiesPerWave / enemiesPerCapsule;
        enemiesKilled = 0;
        enemyHandler->totalEnemiesSpawned = 0;
        capsuleHandler->capsuleSpawner(capsulesPerWave, mdl3D->pos);
        waveSpawned = true;
        return;
    }
    for(const auto& c : capsuleHandler->capsules){
        if(c->state == ONGROUND
           && !c->hasSpawnedEnemies
           && enemyHandler->totalEnemiesSpawned < enemiesPerWave){
            enemyHandler->spawn(enemiesPerCapsule, c->pos);
            c->hasSpawnedEnemies = true;
        }
    }
    if(enemiesKilled >= enemyHandler->totalEnemiesSpawned
       && enemyHandler->totalEnemiesSpawned > 0){
        waveSpawned = false;
        wave++;
        myHUD->addConsoleMessage("Wave Ended");
    }
    //myHUD->addConsoleMessage("Enemies Killed:" + std::to_string(enemiesKilled));
    //myHUD->addConsoleMessage("Enemies Left:" + std::to_string(enemyHandler->totalEnemiesSpawned));

}

void _level3::pickupMenu(_capsule* c){
    _item pickupItem = myInv->randomItem();
    int iteminput;
    isPickupMenuOpen = true;
    pickupChoices.clear();
    for(int i = 0; i < 3; i++){
        pickupChoices.push_back(myInv->randomItem());
    }
    myInv->addItem(pickupItem);
    std::string message = "Item \'" + pickupItem.name + "\' added to inventory. Stats: ";
    for(const auto& stat : pickupItem.stats){
        std::string statText = stat.first + ": ";
        statText += stat.second;
        message += statText;
    }
    myHUD->addConsoleMessage(message);
    isPickupMenuOpen = false;
    c->state = COLLECTED;
}
void _level3::itemFromCapsule() {
    // Check if the player is colliding with a capsule, and give the player an item if they are
    for(const auto& c : capsuleHandler->capsules){
        if(myCol->isSphereCol(mdl3D->pos, c->pos, 2.0f, 2.0f, 1.0f) && c->state == ONGROUND){
            pickupMenu(c);
        }
    }
}

void _level3::drawSceneCalc(){
    lose();
    mdl3D->update();
    mySprite->face(mdl3D->getPos());
    // Collision Check to Sprites, to pick up items
    // std::cout << myCol->isSphereCol(mdl3D->pos,mySprite->pos,1.0f,1.0f,0.1f) << std::endl;
    nearestEnemy = enemyHandler->nearest(mdl3D->pos);
    if(nearestEnemy) attackHandler(nearestEnemy->pos, mdl3D->pos);
    waveSpawn();
    itemFromCapsule();
    enemyHandler->update(mdl3D->pos);
    capsuleHandler->update();
    myInv->setPlayerStats(mdl3D->itemStats);
    mdl3D->applyPlayerStats();
}
void _level3::drawFloor(){
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
}

void _level3::drawScene()
{
    // OpenGL draw
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    // Draw calculations
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
    glPushMatrix();
        int sbSize = 10.0f;
        glScalef(sbSize, sbSize, sbSize);

        glDisable(GL_LIGHTING);
        //glDisable(GL_FOG);
        glDepthMask(GL_FALSE);
        glEnable(GL_TEXTURE_2D);

        mySkyBox->drawSkyBox();

        glDepthMask(GL_TRUE);
        glEnable(GL_LIGHTING);
        //glEnable(GL_FOG);

    glPopMatrix();
    glPopMatrix();
    drawFloor();
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

    glPushMatrix();

        // Move to player
        glTranslatef(mdl3D->pos.x, mdl3D->pos.y, mdl3D->pos.z);

        // Apply player orientation
        glRotatef(mdl3D->currentAngle-90, 0, 1, 0);  // yaw
        glScalef(0.1f, 0.1f, 0.1f);   // <<< adjust until correct size


        // Hand offset (adjust these)
        glTranslatef(0.0f, 0.0f, 0.0f);

        // Orient spear
        glRotatef(90, 1, 0, 0);
        glRotatef(180, 0, 1, 0);

        //mdl3DW->actionTrigger = mdl3D->playerModel->actionTrigger;
        //mdl3DW->Actions();
        //mdl3DW->Draw();

    glPopMatrix();
    // Enemy Render
    enemyHandler->draw();
    capsuleHandler->draw();
    glDisable(GL_FOG);
    myHUD->draw(width, height);
    glEnable(GL_FOG);
}

void _level3::mouseMapping(int x, int y)

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

int _level3::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
            if(wParam == '='){
                scene = LEVEL2;
                isInit = false;
                return 0;
            }
            if(wParam == VK_OEM_3){
                if(myHUD->debug == false) {
                    myHUD->debug = true;
                    myHUD->addConsoleMessage("Debug ON");
                } else if(myHUD->debug == true){
                    myHUD->debug = false;
                    myHUD->addConsoleMessage("Debug OFF");
                }
            }
            myInput->wParam = wParam;
            myInput->keyPressed(myModel);
            myInput->keyPressed(myPrlx);
            // myInput->keyPressed(mySkyBox);
            myInput->keyPressed(mySprite);
            // myInput->keyPressed(myCam);
            myInput->keyPressed(mdl3D->playerModel,mdl3DW);
            break;
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
            if(scene == LEVEL3){
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
