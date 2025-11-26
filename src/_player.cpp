#include "_player.h"

_player::_player()
{
    //ctor
    stats["Health"] = 100.0f;
    stats["Speed"] = 0.5f;
    stats["AttackSpeed"] = 10.0f;
    stats["Damage"] = 2.0f;
    stats["CriticalChance"] = 0.1f;
    stats["Armor"] = 0.0f;
    stats["ArmorPiercing"] = 10.0f;
    camHeightOffset = 3.0f;
    currHealth = 100.0f;

}

_player::~_player()
{
    //dtor
}
// Helper Functions
void _player::applyPlayerStats() {
    maxHealth = stats["Health"] + itemStats["Health"];
    movementSpeed = stats["Speed"] + itemStats["Speed"];
    attackSpeed = stats["AttackSpeed"] + itemStats["AttackSpeed"];
    damage = stats["Damage"] + itemStats["Damage"];
    critChance = stats["CriticalChance"] + itemStats["CriticalChance"];
    armor = stats["Armor"] + itemStats["Armor"];
    armorPiercing = stats["ArmorPiercing"] + itemStats["ArmorPiercing"];
}
void _player::hit(float hitDamage) {
    currHealth -= hitDamage;
}
void _player::setTarget(vec3 point) {
    currTarget = point;
}
vec3 _player::calcMoveVector(){
    vec3 moveVec(0,0,0);

    vec3 lookDir = cam.des - cam.eye;
    lookDir.y = 0.0f;
    lookDir.normalize();

    if(isMovingForward)   { moveVec += lookDir; }
    if(isMovingBack)      { moveVec -= lookDir; }
    if(isMovingLeft)      { moveVec += vec3(lookDir.z, 0, -lookDir.x); }
    if(isMovingRight)     { moveVec += vec3(-lookDir.z, 0, lookDir.x); }
    return moveVec;
}
void _player::rotatePlayer(const vec3& normVec){
    vec3 moveVec = normVec;
    if(moveVec.normalize() < 0.0001f) return;
    float targetAngle = -atan2f(moveVec.x, -moveVec.z) * 180.0f / PI;
    float rotationSpeed = 4.0f;
    float angleDiff = targetAngle - currentAngle;

    while(angleDiff > 180.0f) angleDiff -= 360.0f;
    while(angleDiff < -180.0f) angleDiff += 360.0f;

    currentAngle += (angleDiff > 0 ? rotationSpeed : -rotationSpeed);
}
void _player::update(){
    vec3 moveVec = calcMoveVector();
    if(moveVec.normalize() > 0.0001f){
        pos += moveVec * movementSpeed;
        isMoving = true;
        rotatePlayer(moveVec);
    }
    else isMoving = false;


    cam.des = pos + vec3(0,camHeightOffset,0);
    cam.rotateXY();
    cam.setUpCamera();
}
void _player::resetPlayer() {
    stats["Health"] = 100.0f;
    stats["Speed"] = 0.5f;
    stats["AttackSpeed"] = 10.0f;
    stats["Damage"] = 2;
    stats["CriticalChance"] = 0.1f;
    stats["Armor"] = 0.0f;
    stats["ArmorPiercing"] = 10.0f;
    applyPlayerStats();
    currHealth = maxHealth;
}


void _player::init(const char* model) {
    resetPlayer();
    initModel(model);
    cam.camInit();
}
void _player::draw() {
    applyPlayerStats();
    glPushMatrix();
        glTranslatef(pos.x,pos.y,pos.z);
        glRotatef(currentAngle-90, 0, 1, 0);
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);


        if(isMoving && actionTrigger != RUN){
            actionTrigger = RUN;
            pframe = 0;
        } else if (!isMoving && actionTrigger != STAND) {
            actionTrigger = STAND;
            pframe = 0;
        }
        glScalef(0.1,0.1,0.1);
        Actions();
        Draw();
    glPopMatrix();
}

void _player::handleInput(UINT uMsg, WPARAM wParam, LPARAM lParam, HWND hWnd) {
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
                case VK_SPACE:
                    isJumping = true;
                    break;
                case 'P':
                case 'p':
                    stats["Speed"] += 0.025f;
                    stats["Damage"] += 2.0f;
                    break;
            }
            break;
        case WM_KEYUP:
            switch(wParam) {
                case 'W': case 'w':
                    isMovingForward = false;
                    break;
                case 'A': case 'a':
                    isMovingLeft = false;
                    break;
                case 'D': case 'd':
                    isMovingRight = false;
                    break;
                case 'S': case 's':
                    isMovingBack = false;
                    break;
                case VK_SPACE:
                    isJumping = false;
                    break;
            }
            break;
        case WM_MOUSEMOVE: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);

            // Calculate movement delta relative to the screen center
            int centerX = GetSystemMetrics(SM_CXSCREEN) / 2;
            int centerY = GetSystemMetrics(SM_CYSCREEN) / 2;

            int dx = x - centerX;
            int dy = y - centerY;

            cam.rotAngle.x += dx * 0.2f;
            cam.rotAngle.y -= dy * 0.2f;

            if(cam.rotAngle.y > 85) cam.rotAngle.y = 85;
            if(cam.rotAngle.y < 0 ) cam.rotAngle.y = 0;

            // Reset the mouse to the center of the window
            POINT centerPoint = { centerX, centerY };
            ClientToScreen(hWnd, &centerPoint);
            SetCursorPos(centerPoint.x, centerPoint.y);
        }
            break;
    }
}
