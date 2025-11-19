#include "_level1.h"

_level1::_level1()
{
    //ctor
    myTime->startTime = clock();
    scene = LEVEL1;
    isInit = false;
    isMovingLeft = isMovingRight = isMovingForward = isMovingBack = false;
    isMoving = false;
}

_level1::~_level1()
{
    //dtor
}

void _level1::reSizeScene(int width, int height)
{
    float aspectRatio = (float)width/(float)height;// keep track of the ratio
    glViewport(0,0,width,height); // adjust my viewport

    glMatrixMode(GL_PROJECTION);  // To setup ptrojection
    glLoadIdentity();             // calling identity matrix
    gluPerspective(45, aspectRatio,0.1,1000.0); // setting perspective projection

    this->width = GetSystemMetrics(SM_CXSCREEN);
    this->height= GetSystemMetrics(SM_CYSCREEN);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();             // calling identity matrix
}

void _level1::initGL()
{
    ShowCursor(FALSE);
    glShadeModel(GL_SMOOTH); // to handle GPU shaders
    glClearColor(0.0f,0.0f,0.0f,1.0f); // black background color
    glClearDepth(2.0f);         //depth test for layers

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


    myTexture->loadTexture("images/tex.jpg");
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
    mdl3D->initModel("models/Tekk/tris.md2");
    mdl3DW->initModel("models/Tekk/weapon.md2");

    myCam->camInit();

    snds->initSounds();
    snds->playMusic("sounds/HighNoon.mp3");
    isInit = true;
}

void _level1::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//clear bits in each itteration
    glLoadIdentity();             // calling identity matrix

    // Player movement Controls
    float speed = 0.02f;
    if(isMovingForward||isMovingBack||isMovingLeft||isMovingRight)isMoving = true;
    else isMoving = false;
    if(isMovingForward){
        mdl3D->pos.z -= speed;
        std::cout << isMovingForward << std::endl;
    }
    if(isMovingBack) {
        mdl3D->pos.z += speed;
    }
    if(isMovingLeft) {
        mdl3D->pos.x -= speed;
    }
    if(isMovingRight) {
        mdl3D->pos.x += speed;
    }
    if(isMoving && mdl3D->actionTrigger != mdl3D->RUN) {
        mdl3D->actionTrigger = mdl3D->RUN;
    } else if(!isMoving && mdl3D->actionTrigger != mdl3D->STAND){
        mdl3D->actionTrigger = mdl3D->STAND;
    }
    // Camera setup
    myCam->des.x = mdl3D->pos.x;
    myCam->des.y = mdl3D->pos.y + 3.0f;
    myCam->des.z = mdl3D->pos.z;

    float height = 6.0f;

    myCam->rotateXY();
    myCam->eye.y += 5.0f;

    myCam->setUpCamera();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);

     glPushMatrix();
       myTexture->bindTexture();
       // myModel->drawModel();
     glPopMatrix();

      glPushMatrix();
      glScalef(4.33,4.33,1);
 //   myPrlx->drawParallax(width,height);
  //  myPrlx->prlxScrollAuto("left", 0.0005);
    glDepthMask(GL_FALSE);
    mySkyBox->drawSkyBox();
    glDepthMask(GL_TRUE);
    glPopMatrix();

     glPushMatrix();
       mySprite->drawSprite();
      // mySprite->actionTrigger = mySprite->WALKRIGHT;

    if(myTime->getTicks()>70)
    {
       mySprite->spriteActions();
       myTime->reset();
    }
    glPopMatrix();

    glPushMatrix();
        // Player movement

        glTranslatef(mdl3D->pos.x,mdl3D->pos.y,mdl3D->pos.z);

        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);

        glScalef(0.1,0.1,0.1);
        mdl3D->Actions();
        mdl3DW->Actions();
        mdl3DW->Draw();
        mdl3D->Draw();
    glPopMatrix();

    glPushMatrix();

       for(int i =0; i<10;i++)
       {
           if(b[i].live)
           {
               b[i].drawBullet();
               b[i].bulletActions();

        // do collision check between model and the bullets
        //       myCol->isSphereCol(myModel->p,b[i].)

           }
       }
    glPopMatrix();
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
    switch(uMsg)
    {
        case WM_KEYDOWN:
            switch(wParam) {
                case 'W':
                case 'w':
                    isMovingForward = true;
                    break;
                case 'A':
                case 'a':
                    isMovingLeft = true;
                    break;
                case 'D':
                case 'd':
                    isMovingRight = true;
                    break;
                case 'S':
                case 's':
                    isMovingBack = true;
                    break;
            }
            // Player movement

            myInput->wParam = wParam;
            myInput->keyPressed(myModel);
            myInput->keyPressed(myPrlx);
            // myInput->keyPressed(mySkyBox);
            myInput->keyPressed(mySprite);
            // myInput->keyPressed(myCam);
            myInput->keyPressed(mdl3D,mdl3DW);
        break;

        case WM_KEYUP:
            switch(wParam) {
                case 'W':
                case 'w':
                    isMovingForward = false;
                    break;
                case 'A':
                case 'a':
                    isMovingLeft = false;
                    break;
                case 'D':
                case 'd':
                    isMovingRight = false;
                    break;
                case 'S':
                case 's':
                    isMovingBack = false;
                    break;
            }
            myInput->wParam = wParam;
            myInput->keyUp(mySprite);
            // mdl3D->actionTrigger=mdl3D->STAND;
            // mdl3DW->actionTrigger=mdl3DW->STAND;
        break;

        case WM_LBUTTONDOWN:
            myInput->wParam = wParam;
            myInput->mouseEventDown(myModel,LOWORD(lParam),HIWORD(lParam));

             mouseMapping(LOWORD(lParam), HIWORD(lParam));
             clickCnt =clickCnt%10;

                 b[clickCnt].src.x = mdl3D->pos.x;
                 b[clickCnt].src.y = mdl3D->pos.y;
                 b[clickCnt].src.z = mdl3D->pos.z;

                 b[clickCnt].des.x = msX;
                 b[clickCnt].des.y = -msY;
                 b[clickCnt].des.z = msZ;

                 b[clickCnt].t =0;
                 b[clickCnt].actionTrigger = b[clickCnt].SHOOT;
                 b[clickCnt].live = true;
                   clickCnt++;
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
            int dx = LOWORD(lParam) - lastMouseX;
            int dy = HIWORD(lParam) - lastMouseY;

            lastMouseX = LOWORD(lParam);
            lastMouseY = HIWORD(lParam);

            myCam->rotAngle.x += dx * 0.2f;
            myCam->rotAngle.y -= dy * 0.2f;

            if(myCam->rotAngle.y > 85) myCam->rotAngle.y = 85;
            if(myCam->rotAngle.y < 0 ) myCam->rotAngle.y = 0;
            myInput->wParam = wParam;
            myInput->mouseMove(myModel,LOWORD(lParam),HIWORD(lParam));
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
