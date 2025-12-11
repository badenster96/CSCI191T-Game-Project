#include "_enemy.h"

_enemy::_enemy()
{
    //ctor
    iFrames = 0.3f;
    lastTimeHit = 0.0f;


}

_enemy::~_enemy()
{
    //dtor
}
void _enemy::init(std::string model){
    std::string pathMdl, pathTex;
    pathMdl = "models/" + model + "/tris.md2";
    pathTex = "models/" + model + "/texture.jpg";
    mdl->initModel((char*)pathMdl.c_str());
    tex->loadTexture((char*)pathTex.c_str());
    mdl->md2file.tex_id = tex->textID;
}
void _enemy::pain(){
    isFlashing = true;
    flashStartTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
}

// Spawns the enemy at a random location around a point
void _enemy::spawn(vec3 center){
    // Enemy stats
    health = 10.0f + rand()%5;
    damage = 1.0f + rand()%10;
    speed = 10.0f + rand()%40 * 1.0f;
    // Spawn logic
    float radius = rand()%50/10.0f + 10;
    angle = rand()%360 / 180.0f * PI;
    // Calculate the random point
    pos.x = center.x + radius * cos(angle);
    pos.y = 0.0f;
    pos.z = center.z + radius * sin(angle);
    // std::cout << radius << " " << angle << " " << pos.x << pos.z << std::endl;
    isSpawned = isAlive = true;
}
void _enemy::update(){
    if(isFlashing){
        float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
        if(currentTime - flashStartTime > 0.04f){
            isFlashing = false;
        }
    }
    //speed += 0.001f;

}
void _enemy::draw(){
    glPushMatrix();
        glTranslatef(pos.x, pos.y, pos.z);
        glRotatef(angle+90, 0, 1, 0);
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        glScalef(0.1,0.1,0.1);

        update();
        mdl->Actions();

        // If enemy is flashing, tint it
        if(isFlashing){
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mdl->md2file.tex_id);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  // TINT MODE
            glColor3f(1.0f, 0.3f, 0.3f); // red flash
        } else {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, mdl->md2file.tex_id);
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);   // normal texture
            glColor3f(1.0f,1.0f,1.0f);
        }

        mdl->Draw();
    glPopMatrix();
}

void _enemy::moveTowardPoint(vec3 point, float deltaTime) {
    vec3 path;
    path.x = point.x - pos.x;
    path.y = 0;
    path.z = point.z - pos.z;
    this-> angle = atan2f(path.x, path.z) * 180.0f / PI;
    // Normalize the vector, to control speed better
    float length = sqrtf(path.x*path.x + path.z*path.z);
    if(length > 0.01f){
        path.x /= length;
        path.z /= length;
        mdl->actionTrigger = RUN;
        pos.x += path.x * speed * deltaTime;
        pos.z += path.z * speed * deltaTime;
    }
}
