#ifndef GAME_H
#define GAME_H

#include <vector>
#include "zip.h"
#include "physics.h"
#include "player.h"
#include "planet.h"
#include "spaceman.h"
#include "joystick.h"
#include "asset_renderer.h"
#include "obj_renderer.h"
#include "screen_renderer.h"
#include "player_renderer.h"
#include "background_renderer.h"
#include "camera.h"
#include "input_handler.h"
#include "texture_handler.h"
#include "galaxy.h"

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

        Spaceman _user;
        Galaxy _galaxy;
        Camera _cam;
        TextureHandler _tex;

        /* Renderers */
        AssetRenderer *_ass_rend;
        ObjRenderer *_obj_rend;
        ScreenRenderer *_scr_rend;
        PlayerRenderer *_player_rend;
        BackgroundRenderer *_back_rend;

        void respondToInput();
        void applyGravity();
        void resetTime();
        void setupObjs();
        void drawStats();

    public:
        /* Respond to input events 
         - called from JNI */
        InputHandler input;

        /* Setup OpenGL
         - called from JNI */
        void setupGLContext(int screen_w, int screen_h);

        /* Main Lewp
         - called from JNI */
        void draw();

        static int getScreenWidth();
        static int getScreenHeight();
        static float getElapsedTime();
        static float getTimeSpeed();

        Game(std::string pkg_name, int screen_w, int screen_h);
        ~Game();
};

#endif /* GAME_H */
