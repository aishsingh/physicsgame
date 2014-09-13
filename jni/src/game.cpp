#include <stdlib.h>    // rand()
#include <time.h>      // seeding rand numbers
#include <vector>
#include <exception>

#include "common.h"
#include "game.h"
#include "physics.h"
#include "log.h"

Game::Game() {
    // Initilise 
    _finished = false;
    _elapsed_time = 0.0f;
    _time_speed = 0.7f;
    
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

    // Log OpenGL details
    LOGI("--------------------");
    LOGI("Loading OpenGL");
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setup(%d, %d, %s)", w, h, &packageName);

    players.at(0).setup(w, h);
}

void Game::run() {
    /* MAIN GAME LOOP */

    // Update _rend_boxes frame
    // _rend_boxes.renderFrame();

    // Render players
    for(int i=0; i<(int)players.size(); i++) {
        players.at(i).draw(_elapsed_time, _screen_width, _screen_height);
    }

    // Increment game time according to the current speed of time
    _elapsed_time += _time_speed;
}

void Game::loadResources() {
    // textures.loadTextures(__rend_boxes);
}

void Game::handleInput(float x, float y) {
    // LOGI("Touch at (%.2f, %.2f)", x, y);

    for (int i=1; i<=_BOXES_PER_PRESS; i++) {
        // TODO replace (x, y) touch pos with a position returned from a controller class
        // Player[0] is the main player (the user)
        players.at(0).update(x, y, _elapsed_time);
    }
}

//---
void Game::loadAPK (const char *packageName) {
    LOGI("Loading APK %s", packageName);
    APKArchive = zip_open(packageName, 0, NULL);
    if (APKArchive == NULL) {
        LOGE("Error loading APK");
        return;
    }

    //Just for debug, print APK contents
    int numFiles = zip_get_num_files(APKArchive);
    for (int i=0; i<numFiles; i++) {
        const char* name = zip_get_name(APKArchive, i, 0);
        if (name == NULL) {
            LOGE("Error reading zip file name at index %i : %s", i, zip_strerror(APKArchive));
            return;
        }
        LOGI("File %i : %s\n", i, name);
    }
}
