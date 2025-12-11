#include "_level1.h"

_level1::_level1()
{
    //ctor
}

_level1::~_level1()
{
    //dtor
}
void _level1::reSizeScene(int width, int height)
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
void _level1::initFiles() {
    // sfx

    // Music
    files["CombatMusic"] = "sounds/music/DroneAttack.wav";
    files["bullet"] = "models/Tekk/weapon.md2";
    files["player"] = "waste";
    files["Enemy"] = "badboyblake";
    files["Floor"] = "images/tex.jpg";
}
void _level1::initGL()
{
    myHUD->addConsoleMessage("Initializing Level1...");
    scene = LEVEL1;

    glClearColor(0.15f, 0.15f, 0.22f, 1.0f);
    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambient[]  = { 0.6f, 0.6f, 0.6f, 1.0f };
    GLfloat diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightPos[] = { 0.0f, 50.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_TEXTURE_2D);

    // Fog
    glEnable(GL_FOG);
    {
        GLfloat fogColor[] = {0.1f, 0.1f, 0.12f, 1.0f};
        glFogi(GL_FOG_MODE, GL_LINEAR);
        glFogfv(GL_FOG_COLOR, fogColor);
        glFogf(GL_FOG_START, 40.0f);
        glFogf(GL_FOG_END,   200.0f);
    }

    // Assets
    myTexture->loadTexture("images/floor.jpg");
    myPrlx->parallaxInit("images/prlx.jpg");

    mySkyBox->skyBoxInit(100);
    mySkyBox->tex[0] = mySkyBox->textures->loadTexture("images/front.png");
    mySkyBox->tex[1] = mySkyBox->textures->loadTexture("images/back.png");
    mySkyBox->tex[2] = mySkyBox->textures->loadTexture("images/top.png");
    mySkyBox->tex[3] = mySkyBox->textures->loadTexture("images/bottom.png");
    mySkyBox->tex[4] = mySkyBox->textures->loadTexture("images/right.png");
    mySkyBox->tex[5] = mySkyBox->textures->loadTexture("images/left.png");
    mySkyBox->tex[6] = mySkyBox->textures->loadTexture("images/Stairs.png");

    mySprite->spriteInit("images/eg.png", 6, 4);

    mdl3D->init("waste");
    mdl3DW->initModel("models/GiJoe/weapon.md2");

    for (int i = 0; i < 10; ++i)
        b[i].iniBullet("models/UFO/UFO/weapon.md2");

    myHUD->setPlayer(mdl3D);
    myHUD->setEnemies(&enemies);
    myHUD->init();

    myInv->initInv();

    snds->initSounds();

    initTeapots();
    waveInterval  = 3.0f;
    lastWaveTime  = (float)clock() / CLOCKS_PER_SEC;

    mdl3D->applyPlayerStats();

    isInit = true;
    myHUD->addConsoleMessage("Level1 Init Complete!");
}

void _level1::initTeapots()
{
    for (int i = 0; i < MAX_TEAPOTS; ++i)
    {
        resetTeapot(i);
        teapotActive[i] = true;
    }
}
void _level1::resetTeapot(int i)
{
    fallingTeapots[i].posX = (rand() % 600 - 300) * 0.1f;
    fallingTeapots[i].posY = 20.0f + (rand() % 200) * 0.1f;
    fallingTeapots[i].posZ = (rand() % 600 - 300) * 0.1f;
}
void _level1::updateTeapots()
{
    const float floorY = -3.0f;

    for (int i = 0; i < MAX_TEAPOTS; ++i)
    {
        if (!teapotActive[i]) continue;

        fallingTeapots[i].posY -= 0.1f;

        if (fallingTeapots[i].posY < floorY)
            handleTeapotMiss(i);
    }
}

void _level1::handleTeapotHit(int i)
{
    if (!teapotActive[i]) return;

    teapotActive[i] = false;
    fallingTeapots[i].posY = -500.0f;

    mdl3D->currHealth  = std::min(mdl3D->maxHealth, mdl3D->currHealth + 10.0f);
    mdl3D->damage     += 0.3f;
    mdl3D->attackSpeed += 0.04f;
    mdl3D->applyPlayerStats();

    myHUD->addConsoleMessage("Teapot Shot! Stats Increased!");
}

void _level1::handleTeapotMiss(int i)
{
    if (!teapotActive[i]) return;

    teapotActive[i] = false;
    fallingTeapots[i].posY = -500.0f;

    mdl3D->currHealth -= 8.0f;
    if (mdl3D->currHealth < 1.0f)
        mdl3D->currHealth = 1.0f;

    mdl3D->damage      = std::max(0.4f, mdl3D->damage - 0.1f);
    mdl3D->attackSpeed = std::max(0.2f, mdl3D->attackSpeed - 0.03f);
    mdl3D->applyPlayerStats();

    myHUD->addConsoleMessage("You missed! Stats Decreased!");
}
void _level1::spawnWaveIfNeeded()
{
    float now = (float)clock() / CLOCKS_PER_SEC;

    if (now - lastWaveTime < waveInterval)
        return;

    lastWaveTime = now;

    int spawned = 0;
    int amount = 10;

    for (int i = 0; i < MAX_TEAPOTS && spawned < amount; ++i)
    {
        if (!teapotActive[i])
        {
            resetTeapot(i);
            teapotActive[i] = true;
            spawned++;
        }
    }

    if (spawned > 0)
        myHUD->addConsoleMessage("Wave Incoming!");
}

int _level1::findNearestTeapot()
{
    int best = -1;
    float bestDist = 0;

    vec3 p = mdl3D->pos;

    for (int i = 0; i < MAX_TEAPOTS; ++i)
    {
        if (!teapotActive[i]) continue;

        vec3 t = { fallingTeapots[i].posX, fallingTeapots[i].posY, fallingTeapots[i].posZ };

        float dx = t.x - p.x;
        float dy = t.y - p.y;
        float dz = t.z - p.z;
        float d2 = dx*dx + dy*dy + dz*dz;

        if (best == -1 || d2 < bestDist)
        {
            best = i;
            bestDist = d2;
        }
    }
    return best;
}
void _level1::drawFloor()
{
    glPushMatrix();
        glDisable(GL_LIGHTING);
        myTexture->bindTexture();

        float size = 500;
        float y = -3;

        glBegin(GL_QUADS);
            glTexCoord2f(0,0); glVertex3f(-size, y, -size);
            glTexCoord2f(50,0); glVertex3f(size, y, -size);
            glTexCoord2f(50,50); glVertex3f(size, y, size);
            glTexCoord2f(0,50); glVertex3f(-size, y, size);
        glEnd();

        glEnable(GL_LIGHTING);
    glPopMatrix();
}

void _level1::drawSceneCalc()
{
    float deltaTime = myTime->getTickSeconds();
    mdl3D->update(deltaTime);
    mySprite->face(mdl3D->getPos());

    updateTeapots();
    spawnWaveIfNeeded();

    // Bullet physics
    for (int i = 0; i < 10; ++i)
    {
        if (!b[i].live) continue;

        b[i].bulletActions(deltaTime);

        for (int t = 0; t < MAX_TEAPOTS; ++t)
        {
            if (!teapotActive[t]) continue;

            vec3 tea = { fallingTeapots[t].posX, fallingTeapots[t].posY, fallingTeapots[t].posZ };
            vec3 bp  = b[i].pos;

            // BIGGER collision (teapot radius=3, bullet radius=1)
            if (myCol->isSphereCol(tea, bp, 3.0f, 1.0f, 0.0f))
            {
                handleTeapotHit(t);
                b[i].live = false;
                break;
            }
        }
    }

    if (mdl3D->currHealth < 2.0f)
    {
        myHUD->addConsoleMessage("Health Critical! Returning to Menu...");
        scene  = MAIN;
        isInit = false;
    }
}

void _level1::drawScene()
{
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (float)width / height, 0.1f, 2000.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawSceneCalc();

    // Skybox
    glPushMatrix();
        glDepthMask(GL_FALSE);
        mySkyBox->drawSkyBox();
        glDepthMask(GL_TRUE);
    glPopMatrix();

    drawFloor();

    // BIGGER Teapots
    glPushMatrix();
        for (int i = 0; i < MAX_TEAPOTS; ++i)
        {
            if (!teapotActive[i]) continue;

            glPushMatrix();
                glTranslatef(fallingTeapots[i].posX,
                             fallingTeapots[i].posY,
                             fallingTeapots[i].posZ);

                glScalef(4.0f, 4.0f, 4.0f);  // BIG TEAPOTS
                fallingTeapots[i].drawModel();
            glPopMatrix();
        }
    glPopMatrix();

    // Bullets
    glPushMatrix();
        for (int i = 0; i < 10; ++i)
            if (b[i].live)
                b[i].drawBullet();  // bullet size increased inside bullet class
    glPopMatrix();

    // Player + weapon
    mdl3D->draw();

    glPushMatrix();
        glTranslatef(mdl3D->pos.x, mdl3D->pos.y, mdl3D->pos.z);
        glRotatef(mdl3D->currentAngle - 90, 0,1,0);
        glScalef(0.1f,0.1f,0.1f);

        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);

        mdl3DW->actionTrigger = mdl3D->playerModel->actionTrigger;
        mdl3DW->Actions();
        mdl3DW->Draw();
    glPopMatrix();

    glDisable(GL_FOG);
    myHUD->draw(width,height);
    glEnable(GL_FOG);
}

void _level1::mouseMapping(int x, int y)
{
    GLint vp[4];
    GLdouble mv[16], pr[16];
    GLfloat winX, winY, winZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, mv);
    glGetDoublev(GL_PROJECTION_MATRIX, pr);
    glGetIntegerv(GL_VIEWPORT, vp);

    winX = (float)x;
    winY = (float)y;

    glReadPixels(x, (int)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ,
                 mv, pr, vp,
                 &msX, &msY, &msZ);
}

int _level1::winMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (mdl3D)
        mdl3D->handleInput(uMsg, wParam, lParam, hWnd);

    switch (uMsg)
    {
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
            {
                scene  = MAIN;
                isInit = false;
                return 0;
            }

            myInput->wParam = wParam;
            myInput->keyPressed(myModel);
            myInput->keyPressed(myPrlx);
            myInput->keyPressed(mySkyBox);
            myInput->keyPressed(mySprite);
            myInput->keyPressed(myCam);
            myInput->keyPressed(mdl3D->playerModel, mdl3DW);
            break;

        case WM_KEYUP:
            myInput->wParam = wParam;
            myInput->keyUp(mySprite);
            break;

        case WM_LBUTTONDOWN:
        {
            myInput->mouseEventDown(myModel, LOWORD(lParam), HIWORD(lParam));
            mouseMapping(LOWORD(lParam), HIWORD(lParam));

            clickCnt = (clickCnt + 1) % 10;

            int n = findNearestTeapot();
            vec3 src = mdl3D->pos;
            vec3 dest;

            if (n >= 0)
            {
                dest.x = fallingTeapots[n].posX;
                dest.y = fallingTeapots[n].posY;
                dest.z = fallingTeapots[n].posZ;
            }
            else
            {
                dest.x = (float)msX;
                dest.y = (float)-msY;
                dest.z = (float)msZ;
            }

            b[clickCnt].shootBullet(src, dest);
        }
        break;

        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            myInput->mouseEventDown(myModel, LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
            myInput->mouseEventUp();
            break;

        case WM_MOUSEMOVE:
            myInput->mouseMove(myModel, LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_MOUSEWHEEL:
            myInput->mouseWheel(myModel, (double)GET_WHEEL_DELTA_WPARAM(wParam));
            break;

        default:
            break;
    }

    return 0;
}
