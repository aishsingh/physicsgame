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
        PhysicsEngine physics;
        // Textures textures;

        static const bool _DEBUG = false;
        static const int _BOXES_PER_PRESS = 1;

        bool _finished;

        int _screen_width;
        int _screen_height;

        // float touch_x, touch_y; 

        float _boxes_length;
        float _elapsed_time;
        float _time_speed;
        Theme _colour_theme;    // Changes the colour scheme of the boxes


        /* All fonts, gfx, audio will is loaded here */
        void loadResources();


        /* Draw the passed box to the renderer using SDL_gfx */
        void drawBox(Box &box);

        /* Generates a random theme for the GIVEN theme */
        void changeTheme(Theme &old_theme);
        
        /* Creates a new box with the given values
           1. Initializes a bew instanse  */
        void createBox(const float x, const float y, const float diameter);

        /* Removes the box at the given index
           1. Shifts boxes that are right of the index one pos left 
           2. Decreases the memory allocation of the array */
        void removeBox(const int index);

        /* Function gets called every tick of the timer
           Increments the time variable */
        // static Uint32 timerCallback(Uint32 interval, void *param);

        /* main poop hehehe.. */
        void mainLoop();

    public:
        Renderer renderer;

        std::vector<Box> boxes;
        std::vector<Spaceman> players;

        /* Setup OpenGL */
        void setup(int w, int h);

        /* Main function */
        void run();

        /* Respond to input events */
        void handleInput(float x, float y);

        /* Constructor */
        Game();

        /* Destructor */
        ~Game();
};

#endif /* GAME_H */
