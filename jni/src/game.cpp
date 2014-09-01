#include <stdlib.h>    // Needed for rand()
#include <time.h>      // Needed for seeding rand numbers
#include <vector>
#include <exception>

#include "common.h"     // TODO remove duplicate imports
#include "game.h"
#include "physics.h"
#include "shapes.h"    // only used for sizeof(Box); better to remove later
#include "log.h"

Game::Game() {
    // Initilise 
    _finished = false;
    _elapsed_time = 0.0f;
    _time_speed = 0.48f;
    
    // loadSDL();
    loadResources();
    
    // Seed random numbers using the time to improve randomness
    srand(time(NULL));

    // Setup Player
    try {
        players.resize(players.size() + 1, Spaceman(0, 0, 385/2, 460/2));
    }
    catch (std::exception &e) {
        LOGE("Error occured while creating player: %s", e.what());
    }
}
Game::~Game() {

    for(int i=0; i<(int)players.size(); i++) {
        // Clear boxes array
        std::vector<Box>().swap(players.at(i).trail.shapes);
    }
} 
//----
void Game::setup(int w, int h, const char &packageName) {
    _packageName = packageName;
    _screen_width = w;
    _screen_height = h;
    renderer.setupGraphics(w,h);
}

void Game::run() {
    /* MAIN GAME LOOP */

    // Update renderer frame
    renderer.renderFrame();

    // Render players
    for(int i=0; i<(int)players.size(); i++) {
        players.at(i).Draw(renderer, _elapsed_time, _screen_width, _screen_height);
    }

    // Increment game time according to the current speed of time
    _elapsed_time += _time_speed;
}

void Game::loadResources() {
    // textures.loadTextures(_renderer);
}

void Game::handleInput(float x, float y) {
    // Normalise input values
    // float touch_x = (x/(_screen_width/2));
    // float touch_y = (y/(_screen_height/2));


    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    // glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for an ortho camera :

    LOGI("Touch at (%.2f, %.2f)", x, y);

    for (int i=1; i<=_BOXES_PER_PRESS; i++) {

        // TODO replace (x, y) touch pos with a position returned from a controller class
        // Player[0] is the main player (the user)
        players.at(0).update(x, y, _elapsed_time);
    }
}
