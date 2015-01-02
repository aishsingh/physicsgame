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
        std::string _package_name;

        // Used to set the elepsed time since last update
        float _previous_trail_update;

        // All players that are in the game
        // player[0] is the user
        vector<Player*> _players;
        vector<Planet*> _planets;

        UI screen_ui;
        /* Renderers */
        AssetRenderer *_ass_rend;
        ObjRenderer *_obj_rend;
        ScreenRenderer *_scr_rend;

        void applyGravity();

        void resetTime();

        /* Load assets in the apk by extracting it with libzip
         - called from java */
        zip *APKArchive;
        void loadAPK(const char *package_name);

    public:
        /* Setup OpenGL
         - called from java */
        void setupGLContext(int screen_w, int screen_h);

        /* Setup game objects */
        void setupObjs();

        /* Main Lewp
         - called from java */
        void draw();

        /* Respond to input events 
         - called from java */
        void handleInput(float x, float y);

        static int getScreenWidth();
        static int getScreenHeight();
        static float getElapsedTime();
        static float getTimeSpeed();

        Game(std::string package_name);
        ~Game();
};

#endif /* GAME_H */
