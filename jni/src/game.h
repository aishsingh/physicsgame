#ifndef GAME_H
#define GAME_H

#include <vector>
#include "zip.h"
#include "physics.h"
// #include "textures.h"
#include "player.h"

class Game {
    private:
        // Textures textures;
        static const bool _DEBUG = false;
        static const int _BOXES_PER_PRESS = 1;

        bool _finished;

        char _package_name;
        int _screen_width;
        int _screen_height;

        float _elapsed_time;
        float _time_speed;

        // All players that are in the game
        // player[0] is the user
        std::vector<Player*> _players;

        /* Renderers */
        // Renderer _rend_UI_controls;

        /* All fonts, gfx, audio will is loaded here */
        void loadResources();

    public:

        /* Setup OpenGL
         - called from java code */
        void setup(int w, int h, char &package_name);

        /* Main Lewp
         - called from java code */
        void run();

        /* Respond to input events */
        void handleInput(float x, float y);

        /* Load assets in the apk by extracting it with libzip */
        zip *APKArchive;
        void loadAPK(const char *package_name);

        Game();
        ~Game();
};

#endif /* GAME_H */
