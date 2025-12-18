#ifndef _HUD_H
#define _HUD_H

#include <Player/_player.h>
#include <Combat/_enemy.h>


class _hud
{
    public:
        struct dmgNumber {
            float x, y, z;
            float value;
            float time;
            float duration;
        };
        _hud();
        virtual ~_hud();
        // Setters
        void setPlayer(_player* p) {player = p;}
        void setEnemies(std::vector<_enemy*>* e) {enemies = e;}
        // Init
        void init();
        // Helper Functions
        void renderText(int x, int y, const std::string& text);
        void addConsoleMessage(const std::string& message);
        void addGameInfo(const std::string& message);
        void addDamageNumber(vec3 pos, float amount);
        // Draw Functions
        void drawHealthBar();
        void drawStats();
        void drawGameInfo();
        void drawConsole();
        void drawDamageNumbers(float dTime);
        // Main draw function
        void drawHandler();
        void draw(int width, int height);

        bool debug = false;
        int screenWidth, screenHeight;
        std::vector<std::string> gameConsole, gameInfo;
        std::vector<dmgNumber> damageNumbers;

    protected:

    private:
        _player* player = nullptr;
        std::vector<_enemy*>* enemies = nullptr;
};

#endif // _HUD_H
