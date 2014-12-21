#ifndef GAME_H
#define GAME_H

#include <vector>
#include "zip.h"
#include "physics.h"
#include "player.h"
#include "planet.h"
#include "ui.h"
#include "asset_renderer.h"
#include "obj_renderer.h"
#include "screen_renderer.h"

class Game {
    private:
        static int _screen_width;
        static int _screen_height;

        static float _elapsed_time;
        static float _time_speed;

        bool _finished;
        char _package_name;

        // Used to set the elepsed time since last update
        float _previous_trail_update;

        // All players that are in the game
        // player[0] is the user
        vector<Player*> _players;
        vector<Planet*> _planets;

        UI screen_ui;
        /* Renderers */
        AssetRenderer _ass_rend;
        ObjRenderer _obj_rend;
        ScreenRenderer _scr_rend;

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
