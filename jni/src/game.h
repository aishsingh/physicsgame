#ifndef GAME_H
#define GAME_H

#include <vector>

#include "renderer.h"
#include "shapes.h"
#include "physics.h"
// #include "textures.h"
#include "players.h"
#include "common.h"

class Game {
    private:
        // Textures textures;

        static const bool _DEBUG = false;
        static const int _BOXES_PER_PRESS = 1;

        bool _finished;

        char _packageName;
        int _screen_width;
        int _screen_height;

        float _elapsed_time;
        float _time_speed;

        /* All fonts, gfx, audio will is loaded here */
        void loadResources();

        void drawBox(Box &box);

        /* Generates a random theme for the GIVEN theme */
        void changeTheme(Theme &old_theme);
        
        /* Function gets called every tick of the timer
           Increments the time variable */
        // static Uint32 timerCallback(Uint32 interval, void *param);

        /* main poop hehehe.. */
        void mainLoop();

    public:

        // Gets initilised from app_glue.cpp
        // so needs to be public
        Renderer renderer;

        // All players that are in the game
        // player[0] is the user
        std::vector<Spaceman> players;

        /* Setup OpenGL */
        void setup(int w, int h, const char &packageName);

        /* Main function */
        void run();

        /* Respond to input events */
        void handleInput(float x, float y);

        /* Ctor */
        Game();

        /* Dtor */
        ~Game();

};

#endif /* GAME_H */
