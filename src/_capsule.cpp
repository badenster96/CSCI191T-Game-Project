#include "_capsule.h"

_capsule::_capsule()
{
    //ctor
    state = DESPAWNED;
    myTex = new _textureLoader();
}

_capsule::~_capsule()
{
    //dtor
}
void _capsule::init() {

}
// First, spawn the capsule in the level loop
void _capsule::spawn(vec3 center) {
    if(state != DESPAWNED) return;
    spawnTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    state = SPAWNED;
    radius = 1.0f;
    height = 5.0f;
    float radius = rand()%100/1.0f + 100;
    float angle = rand()%360 / 180.0f * PI;

    // Calculate the random point
    pos.x = center.x + radius * cos(angle);
    pos.y = 100.0f;
    pos.z = center.z + radius * sin(angle);
    hasSpawnedEnemies = false;
    state = FALLING;
}
// The falling animation for the capsule, and the handler for collection
void _capsule::update() {
    float currentTime = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    float deltaTime = currentTime - spawnTime;
    if(state == FALLING) {
        float gravity = -9.8f;
        dy += gravity * deltaTime * 0.00001;
        pos.y += dy * deltaTime;
        if((pos.y - height / 2) <= -3.5f) {
            pos.y = height / 2 - 3.5f;
            dy = 0.0f;
            state = ONGROUND;
            //isSpawned = false;
        }
    }
    if(state == COLLECTED){
        state = DESPAWNED;
    }
}
void _capsule::draw() {
    if(state == DESPAWNED | state == COLLECTED) return;
    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z);

    myTex->bindTexture();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(0.4f, 0.1f, 0.1f);
    if(myTex){
        myTex->bindTexture();
        glEnable(GL_TEXTURE_2D);
    }

    // Simple capsule approximation: cylinder + hemispheres
    GLUquadric* quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);

    // Cylinder
    glPushMatrix();
        glTranslatef(0, height/2, 0);
        glRotatef(90, 1, 0, 0);
        gluCylinder(quad, radius, radius, height, 16, 16);
    glPopMatrix();

    // Bottom hemisphere
    glPushMatrix();
        glTranslatef(0, -height/2, 0);
        glutSolidSphere(radius, 16, 16);
    glPopMatrix();

    // Top hemisphere
    glPushMatrix();
        glTranslatef(0, height/2, 0);
        glutSolidSphere(radius, 16, 16);
    glPopMatrix();

    gluDeleteQuadric(quad);

    //glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
