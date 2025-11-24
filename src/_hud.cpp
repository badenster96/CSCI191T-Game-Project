#include "_hud.h"

_hud::_hud()
{
    //ctor
}

_hud::~_hud()
{
    //dtor
}
void _hud::renderText(int x, int y, const char* text) {
    // Needs implementation
}

void _hud::drawHealthBar(int screenWidth, int screenHeight) {
    if (!player) return;

    float padding = 10.0f;
    float barWidth = screenWidth - 2* padding;
    float barHeight = 20.0f;

    float xStart = screenWidth - padding - barWidth;
    float yStart = screenHeight - padding - barHeight;

    float healthPercent = player->currHealth / player->maxHealth;

    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(xStart, yStart);
        glVertex2f(xStart + barWidth, yStart);
        glVertex2f(xStart + barWidth, yStart + barHeight);
        glVertex2f(xStart, yStart + barHeight);
    glEnd();

    glColor3f(1.0f - healthPercent, healthPercent, 0.0f);
    if(healthPercent < 0) healthPercent = 0.0f;
    glBegin(GL_QUADS);
        glVertex2f(xStart, yStart);
        glVertex2f(xStart + barWidth * healthPercent, yStart);
        glVertex2f(xStart + barWidth * healthPercent, yStart + barHeight);
        glVertex2f(xStart, yStart + barHeight);
    glEnd();
}
void _hud::drawStats(int screenWidth, int screenHeight) {
    if (!player) return; // safety check

    float padding = 10.0f;
    float barHeight = 20.0f; // same as health bar height
    float spacing = 5.0f;    // space between text lines

    // Start below health bar
    float xStart = screenWidth - padding - 200.0f; // align with health bar
    float yStart = screenHeight - padding - barHeight - spacing;

    glColor3f(1.0f, 1.0f, 1.0f); // white text

    std::string statsText[] = {
        "Speed: " + std::to_string(player->movementSpeed),
        "Attack Speed: " + std::to_string(player->attackSpeed),
        "Damage: " + std::to_string(player->damage),
        "Crit Chance: " + std::to_string(player->critChance) + "%",
        "Armor: " + std::to_string(player->armor),
        "Armor Piercing: " + std::to_string(player->armorPiercing)
    };

    for (const std::string& stat : statsText) {
        renderText(xStart, yStart, stat.c_str());
        yStart -= (barHeight + spacing); // move down for next stat
    }
}
void _hud::draw(int screenWidth, int screenHeight) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    drawHealthBar(screenWidth, screenHeight);
    drawStats(screenWidth, screenHeight);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}
