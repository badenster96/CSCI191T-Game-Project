#include "_hud.h"

_hud::_hud()
{
    //ctor
}

_hud::~_hud()
{
    //dtor
}
void _hud::init() {
    HDC hDC = wglGetCurrentDC();
    HFONT font = CreateFontA(
        -16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, FF_DONTCARE | DEFAULT_PITCH,
        "Terminal"
    );

    SelectObject(hDC, font);
    wglUseFontBitmapsA(hDC, 0, 256, 1000);
}
void _hud::renderText(int x, int y, const std::string& text) {
    // Needs implementation
    glRasterPos2f(x, y);
    glListBase(1000);
    glCallLists((GLsizei)text.length(), GL_UNSIGNED_BYTE, text.c_str());
}
void _hud::addConsoleMessage(const std::string& message){
    gameConsole.push_back(message);
    if(gameConsole.size() > 20)
        gameConsole.erase(gameConsole.begin());
        std::cout << message << std::endl;
}
void _hud::addGameInfo(const std::string& message){
    gameInfo.push_back(message);
    if(gameInfo.size() > 20)
        gameInfo.erase(gameInfo.begin());
        std::cout << message << std::endl;
}
void _hud::addDamageNumber(vec3 pos, float amount){
    dmgNumber damNum;
    damNum.x = pos.x;
    damNum.y = pos.y;
    damNum.z = pos.z;
    damNum.value = amount;
    damNum.time = 0.0f;
    damNum.duration = 1.0f;
    damageNumbers.push_back(damNum);
}
// Draw functions
void _hud::drawHealthBar() {
    if (!player) return;

    float padding = 10.0f;
    float barWidth = screenWidth - 2* padding;
    float barHeight = 40.0f;

    float xStart = screenWidth - padding - barWidth;
    float yStart = padding;

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
void _hud::drawStats() {
    if (!player) return; // safety check

    float padding = 10.0f;
    float barHeight = 40.0f; // same as health bar height
    float spacing = 5.0f;    // space between text lines
    float fontHeight = 24.0f;

    std::string hpOverlay = std::to_string((int)player->currHealth) + "  /  " + std::to_string((int)player->maxHealth);
    std::vector<std::string> statsNames = {
        "Speed: ",
        "Attack Speed: ",
        "Base Damage: ",
        "Crit Chance: ",
        "Crit Damage: ",
        "Armor: ",
        "Piercing: ",
        "Range: "
    };
    int largestStatNameLength = 0;
    for(int i = 0; i < statsNames.size(); i++){
        if(statsNames.at(i).size() > largestStatNameLength){
            largestStatNameLength = statsNames.at(i).size();
        }
    }
    std::vector<std::string> statsToText = {
        std::to_string((int)(player->movementSpeed / player->stats["Speed"] * 100)) + "%",
        std::to_string((int)(player->attackSpeed / player->stats["AttackSpeed"] * 100)) + "%",
        std::to_string((int)(player->damage)),
        std::to_string((int)(player->critChance * 100.0f)) + "%",
        std::to_string((int)(player->critDamage * 100.0f)) + "%",
        std::to_string((int)player->armor),
        std::to_string((int)(player->piercing)),
        std::to_string((int)(player->range)) + "ft"
    };
    int largestStatLength = 0;
    for(int i = 0; i < statsToText.at(i).size(); i++){
        if(statsToText.at(i).size() > largestStatLength){
            largestStatLength = statsToText.at(i).size();
        }
    }
    int statWidth = largestStatNameLength + largestStatLength;
    // Start below health bar, set box coords
    float xBoxStart = padding;
    float yBoxStart = padding*2 + barHeight;
    float boxWidth  = xBoxStart + statWidth * 15.0f;
    float boxHeight = yBoxStart + 2*padding + (barHeight * statsToText.size()); // 6 lines of stats

    float xStart = xBoxStart + padding; // align with health bar
    float yStart = yBoxStart + fontHeight;

    // Render Box
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // black with 50% transparency
    glBegin(GL_QUADS);
        glVertex2f(xBoxStart, yBoxStart);           // add small padding
        glVertex2f(boxWidth, yBoxStart);
        glVertex2f(boxWidth, boxHeight);
        glVertex2f(xBoxStart, boxHeight);
    glEnd();
    // Render Text
    glColor3f(1.0f, 1.0f, 1.0f); // white text
    renderText(screenWidth / 2 - 50.0f, padding + 30.0f, hpOverlay.c_str());
    for (const std::string& stat : statsNames) {
        renderText(xStart, yStart, stat.c_str());
        yStart += (barHeight + spacing); // move down for next stat
    }
    yStart = yBoxStart + fontHeight;
    for(const auto& stat : statsToText){
        renderText(xStart + (statWidth * 10.0f), yStart, stat.c_str());
        yStart += (barHeight + spacing);
    }
}
void _hud::drawConsole() {
    float y = 100;
    if(debug){
        for(int i = 0; i < gameConsole.size(); i++){
            renderText(320, y, gameConsole[i].c_str());
            y += 25;
        }
    }
}
void _hud::drawGameInfo() {
    float y = 50;
    for(int i = 0; i < gameInfo.size(); i++){
        renderText(screenWidth - 300, screenHeight - y, gameInfo[i].c_str());
        y += 25;
    }
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // black with 50% transparency
}
void _hud::drawDamageNumbers(float dTime){
    for(int i = 0; i < damageNumbers.size(); i++){
        dmgNumber &dmgNum = damageNumbers[i];
        dmgNum.time += dTime;

        float alpha = 1.0f - (dmgNum.time / dmgNum.duration);
        if(alpha < 0.0f) alpha = 0.0f;

        float screenX = dmgNum.x;
        float screenY = dmgNum.y - dmgNum.time * 30.0f;

        std::string text = std::to_string((int)dmgNum.value);

        glColor4f(1.0f, 0.2f, 0.2f, alpha);
        renderText(screenX, screenY, text);

        if(dmgNum.time >- dmgNum.duration){
            damageNumbers.erase(damageNumbers.begin() + i);
        } else i++;
    }
}
void _hud::drawHandler() {
    drawHealthBar();
    drawStats();
    drawConsole();
    drawGameInfo();
}
void _hud::draw(int screenWidth, int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight= screenHeight;
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);   // SAVE VIEWPORT

    glViewport(0, 0, screenWidth, screenHeight);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, screenWidth, screenHeight, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    drawHandler();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);

    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]); // RESTORE VIEWPORT
}
