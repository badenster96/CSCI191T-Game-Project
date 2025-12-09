#ifndef _HUD_H
#define _HUD_H

#include <_player.h>
#include <_enemy.h>


class _hud
{
    public:
        _hud();
        virtual ~_hud();
        void setPlayer(_player* p) {player = p;}
        void setEnemies(std::vector<_enemy*>* e) {enemies = e;}
        void init();
        void renderText(int x, int y, const std::string& text);
        void drawHealthBar(int screenWidth, int screenHeight);
        void drawStats(int screenWidth, int screenHeight);
        void addConsoleMessage(const std::string& message);
        void drawConsole(int screenWidth, int screenHeight);
        void draw(int width, int height);

        bool debug = false;
        std::vector<std::string> gameConsole;

    protected:

    private:
        _player* player = nullptr;
        std::vector<_enemy*>* enemies = nullptr;
};

#endif // _HUD_H
