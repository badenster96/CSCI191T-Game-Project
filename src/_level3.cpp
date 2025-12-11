#include "_level3.h"

_level3::_level3()
{
    //ctor
    myTime->startTime = clock();
    isInit = false;
    nearestEnemy = nullptr;

}

_level3::~_level3()
{
    //dtor
}

void _level3::initFiles() {
    // sfx
    files["M16"] = "sounds/sfx/bullets/M16/M16_Shoot_Auto_001.wav";
    files["EnemyHit"] = "sounds/sfx/blood/Blood_Splash_A_001.mp3";
    files["ZombieDie"] = "sounds/sfx/zombiedie/Zombie001_Die_A_001.mp3";
    // Music
    files["CombatMusic"] = "sounds/music/DroneAttack.wav";
    files["bullet"] = "models/Tekk/weapon.md2";
    files["player"] = "waste";
    files["Enemy"] = "badboyblake";
    files["Floor"] = "images/tex.jpg";
}
void _level3::initTextures() {
    myHUD->addConsoleMessage("Loading Textures...");
    myTexture->loadTexture(files["Floor"]);
    //myPrlx->parallaxInit("images/prlx.jpg");

    boundarySize = 300.0f;
    mySkyBox->skyBoxInit(boundarySize);
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
    //mySprite->spriteInit("images/eg.png",6,4);
    player->init(files["player"]);
    for(int i = 0; i < 10; i++){
        b[i].iniBullet(files["bullet"]);
    }
    enemyHandler->initModels(files["Enemy"]);
    // capsuleHandler->init();
    myHUD->addConsoleMessage("Textures loaded.");
}

void _level3::initGL() {
    myHUD->addConsoleMessage("Initializing Level3...");
    scene = LEVEL3;
    initFiles();
    lockCursor();
    glShadeModel(GL_SMOOTH); // to handle GPU shaders
    glClearColor(0.01f,0.02f,0.05f,1.0f); // black background color
    glClearDepth(1.0f);         //depth test for layers

    glEnable(GL_DEPTH_TEST);    //activate depth test
    glDepthFunc(GL_LEQUAL);     // depth function type

    //glEnable(GL_LIGHTING);
    GLfloat ambientLight[]  = {0.02f, 0.02f, 0.05f, 1.0f}; // very dim blue
    GLfloat diffuseLight[]  = {0.1f, 0.1f, 0.2f, 1.0f};    // soft bluish light
    GLfloat specularLight[] = {0.1f, 0.1f, 0.2f, 1.0f};    // subtle specular highlights
    GLfloat lightPos[]      = {50.0f, 100.0f, 50.0f, 1.0f};
    //glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Fog
    glEnable(GL_FOG);
    GLfloat fogColor[] = {0.01f, 0.02f, 0.05f, 0.8f};

    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_START, 20.0f);
    glFogf(GL_FOG_END, 200.0f);
    GLfloat lampDiffuse[] = {0.8f, 0.5f, 0.2f, 1.0f}; // warm orange
    GLfloat lampPos[]     = {10.0f, 2.0f, 10.0f, 1.0f};

    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lampDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, lampPos);

    // Set matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = (float)width / (float)height;
    gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    enemyHandler->setup(200);
    capsuleHandler->setup(100);

    myHUD->setPlayer(player);
    myHUD->setEnemies(&enemyHandler->enemies);

    initTextures();

    snds->initSounds();
    snds->playMusic(files["CombatMusic"]);
    myHUD->init();
    myInv->initInv();

    // Level stats setup
    player->applyPlayerStats();
    isInit = true;
    myHUD->addConsoleMessage("_level3 initialized");
}
void _level3::lose() {
    if(player && player->currHealth <= 0.0f){
        player->resetPlayer();
        myInv->resetItems();
        scene = MAIN;
        isInit = false;
        return;
    }
}
void _level3::enemyDamagePlayer(_player* p){
    float currentTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    if(currentTime - lastHitTime >= p->iFrames){
        for(const auto& e : enemyHandler->enemies){
            if(e && myCol->isSphereCol(p->pos,e->pos, 1.0f, 1.0f, 1.0f) && e->isAlive){
                lastHitTime = currentTime;
                std::string message = "At:" + to_string(std::round(lastHitTime * 100.0f) / 100.0f) + " | Player hit for " + to_string((int)nearestEnemy->damage) + " damage!";
                myHUD->addConsoleMessage(message);
                p->hit(e->damage);
            }
        }
    }
}

void _level3::attackHandler(vec3 nearestE, vec3 p) {
    float currentTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    if(nearestEnemy){
        player->setTarget(nearestEnemy->pos);
        for(int i = 0; i < 10; i++){
            if(nearestEnemy && !b[i].live && currentTime - lastAttackTime >= 1/player->attackSpeed){
                if(nearestEnemy->isAlive
                   && ((player->pos - nearestEnemy->pos).lengthSquared() <= player->stats["Range"] * player->stats["Range"])) {
                    b[i].shootBullet(p, nearestE, player->stats["Range"], player->stats["Piercing"]);
                    snds->playRandSound(files["M16"], 10, 0.4f);
                }
                lastAttackTime = currentTime;
            }
            // fires the bullets
            if(b[i].live){
                int hitsThisFrame = 0;
                for(const auto& e : enemyHandler->enemies){
                    if(myCol->isSphereCol(b[i].pos,e->pos,1.0f,1.0f,0.1f)
                       && currentTime - e->lastTimeHit >= e->iFrames
                       && e->health > 0){
                        float critChance = rand()%100 / 100.0f;
                        if(critChance <= player->critChance){
                            e->health -= player->damage * player->critDamage;
                            myHUD->addGameInfo("Crit for " + std::to_string(player->damage * player->critDamage) + " Damage!");
                        }
                        else e->health -= player->damage;
                        e->pain();
                        e->lastTimeHit = currentTime;
                        snds->playRandSound(files["EnemyHit"],8, 0.3f);
                        hitsThisFrame++;
                        b[i].pierce--;
                        //myHUD->addConsoleMessage("Enemy Shot!");
                        if(e->health <= 0) {
                            e->isAlive = e->isSpawned = false;
                            snds->playRandSound(files["ZombieDie"], 5, 0.8f);
                            enemiesKilled++;
                        }
                    }
                }
                b[i].pierce -= hitsThisFrame;
                if(b[i].pierce <= 0){
                    b[i].live = false;
                }
            }
        }
        // Enemy hit handler
        // function in enemyHandler that takes in
        enemyDamagePlayer(player);
    }
}
void _level3::waveSpawn() {
    int enemiesPerWave = 120 * wave;
    int enemiesPerCapsule = 4 * wave;
    if(!waveSpawned){
        myHUD->addConsoleMessage("Wave " + std::to_string(wave) + " Spawned");
        myHUD->addGameInfo("Wave " + std::to_string(wave) + " Spawned");
        int capsulesPerWave = enemiesPerWave / enemiesPerCapsule;
        enemiesKilled = 0;
        enemyHandler->totalEnemiesSpawned = 0;
        capsuleHandler->capsuleSpawner(capsulesPerWave, player->pos);
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

void _level3::pickupMenu(){
    if(capsuleHandler->checkPickup(player->pos, myCol)){
        _item randomItem = myInv->pickupItem();
        myHUD->addGameInfo("Picked up " + randomItem.name);
    };

    //std::string message = "Item \'" + pickupItem.name + "\' added to inventory. Stats: ";
    //myHUD->addConsoleMessage(message);
}

vec3 _level3::clampBounds(const vec3& pos){
    minBound = vec3(-boundarySize, -10.0f, -boundarySize);
    maxBound = vec3(boundarySize, 50.0f, boundarySize);
    vec3 clampedPos = pos;
    if(clampedPos.x < minBound.x) clampedPos.x = minBound.x;
    if(clampedPos.y < minBound.y) clampedPos.y = minBound.y;
    if(clampedPos.z < minBound.z) clampedPos.z = minBound.z;

    if(clampedPos.x > maxBound.x) clampedPos.x = maxBound.x;
    if(clampedPos.y > maxBound.y) clampedPos.y = maxBound.y;
    if(clampedPos.z > maxBound.z) clampedPos.z = maxBound.z;

    return clampedPos;
}
void _level3::clampLevel(){
    for(auto& c : capsuleHandler->capsules){
        c->pos = clampBounds(c->pos);
    }
    for(auto& e : enemyHandler->enemies){
        e->pos = clampBounds(e->pos);
    }
    myCam->eye = clampBounds(myCam->eye);
    player->pos = clampBounds(player->pos);
}

void _level3::update(){
    float deltaTime = myTime->getTickSeconds();
    lose();
    player->update(deltaTime);
    nearestEnemy = enemyHandler->nearest(player->pos);
    if(nearestEnemy) attackHandler(nearestEnemy->pos, player->pos);
    waveSpawn();
    pickupMenu();
    enemyHandler->update(player->pos, deltaTime);
    capsuleHandler->update();
    myInv->setPlayerStats(player->itemStats);
    player->applyPlayerStats();
    for(int i = 0; i < 10; i++){
        if(b[i].live){
            b[i].bulletActions(deltaTime);
            b[i].drawBullet();
        }
    }
    clampLevel();
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
    update();

    // |====================================================|
    // |----------------------DRAW SECTION------------------|
    // |====================================================|

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // your moonlight
    mySkyBox->drawSkyBox();
    drawFloor();

    //mySprite->drawSprite();
    player->draw();
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
    if(player){
        player->handleInput(uMsg, wParam, lParam, hWnd);
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
            // myInput->keyPressed(myPrlx);
            // myInput->keyPressed(mySkyBox);
            // myInput->keyPressed(myCam);
            myInput->keyPressed(player->playerModel,mdl3DW);
            break;
            if(wParam == 'm' || wParam == 'M')
        break;

        case WM_KEYUP:
            myInput->wParam = wParam;
        break;

        case WM_LBUTTONDOWN:
            myInput->wParam = wParam;

             mouseMapping(LOWORD(lParam), HIWORD(lParam));
        break;

        case WM_RBUTTONDOWN:
            myInput->wParam = wParam;
        break;

         case WM_MBUTTONDOWN:
             myInput->wParam = wParam;

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
            break;

        default:
            break;

    }
    return 0;
}
