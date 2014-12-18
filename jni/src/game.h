#ifndef GAME_H
#define GAME_H

#include <vector>
#include "zip.h"
#include "physics.h"
// #include "textures.h"
#include "player.h"
#include "rend_UI.h"

class Game {
    private:
        // Textures textures;
        static int _screen_width;
        static int _screen_height;

        static float _elapsed_time;
        static float _time_speed;

        bool _finished;
        char _package_name;

        // Used to set the elepsed time since last update
        float _previousTrailUpdate;

        // All players that are in the game
        // player[0] is the user
        std::vector<Player*> _players;

        /* Renderers */
        Rend_UI _UI_renderer;

        /* All fonts, gfx, audio will is loaded here */
        void loadResources();
    public:

        /* Setup OpenGL
         - called from java */
        void setup(int w, int h, char &package_name);

        /* Main Lewp
         - called from java */
        void run();

        /* Respond to input events 
         - called from java */
        void handleInput(float x, float y);

        /* Load assets in the apk by extracting it with libzip
         - called from java */
        zip *APKArchive;
        void loadAPK(const char *package_name);

        static int getScreenWidth();
        static int getScreenHeight();
        static float getElapsedTime();
        static float getTimeSpeed();

        Game();
        ~Game();
};

#endif /* GAME_H */
