#include <stdlib.h>
#include <time.h>
#include <exception>
#include <math.h>

#include "game.h"
#include "spaceman.h"
#include "log.h"

#define INIT_TIME_SPEED 0.4f
#define TRAIL_UPDATE_INTERVAL 0.25f
#define TRAIL_PART_PER_UPDATE 1

Game::Game() : _finished(false) {
    _elapsed_time = 0; // Needs to be init here also for some reason
    _previousTrailUpdate = 0;
    
    loadResources();
    
    // Seed random numbers using the time to improve randomness
    srand(time(NULL));

    // Setup Player
    try {
        // _players.resize(_players.size() + 1, new Spaceman(0, 0, 150, 300));
        _players.push_back(new Spaceman(300, 310, Theme::GRAY,    &_ass_rend, &_obj_rend));
        _players.push_back(new Spaceman(0,     0, Theme::RAINBOW, &_ass_rend, &_obj_rend));
        _players.push_back(new Spaceman(850, 310, Theme::GRAY,    &_ass_rend, &_obj_rend));
    }
    catch (std::exception &e) {
        LOGE("Error occured while creating player: %s", e.what());
    }
}

Game::~Game() {
    for(int i=0; i<(int)_players.size(); i++) {
        // Clear boxes array
        vector<Player*>().swap(_players);
    }
}

//----
void Game::setup(int w, int h, char &package_name) {
    _package_name = package_name;
    _screen_width = w;
    _screen_height = h;

    // Log OpenGL details
    LOGI("--------------------");
    LOGI("Loading OpenGL");
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setup(%d, %d, %s)", w, h, &package_name);

    // Setup renderers
    _ass_rend.setup();
    _obj_rend.setup();
    _scr_rend.setup();

    // Put player in the center now that width and height are determined
    _players.at(1)->setX((w/2) - 25);
    _players.at(1)->setY((h/2) - 50);
}

void Game::run() {
    /* MAIN GAME LOOP */
    Renderer::clearScreen();

    // Render trails behind player
    for(int i=0; i<(int)_players.size(); i++)
        _players.at(i)->drawTrail();

    // Render players
    for(int i=0; i<(int)_players.size(); i++)
        _players.at(i)->draw();

    // Render UI
    screen_ui.draw(&_scr_rend);

    // Increment game time according to the current speed of time
    _elapsed_time += _time_speed;
}

void Game::loadResources() {
    // Load textures here
}

void Game::handleInput(float x, float y) {
    // Handle joystick input
    float angle = 0;
    bool js1InputRecieved = false;

    int js1X = 150;
    int js1Y = Game::getScreenHeight() - 200;
    int js1length = 200;
    if (x >= js1X && x <= js1X + js1length && 
            y >= js1Y && y <= js1Y + js1length) {
        js1InputRecieved = true;
        // LOGI("Joystick1 touched");

        /* Quadrants
             ---      
            1   4
          |   +   |   
            2   3
             ---   */

        float A = 0,
              O = 0;

        float originX = js1X + (js1length/2);
        float originY = js1Y + (js1length/2);
        float lengthFromOriginX = 0;
        float lengthFromOriginY = 0;
        if (x >= originX)
            lengthFromOriginX = originX - x;
        else
            lengthFromOriginX = x - originX;
        if (y >= originY)
            lengthFromOriginY = originY - y;
        else
            lengthFromOriginY = y - originY;

        // Find out the quadrants
        if (x <= js1X + (js1length/2) && y <= js1Y + (js1length/2)) {
            // 1st Quad
            A += lengthFromOriginY;
            O += lengthFromOriginX;
        }
        else if (x <= js1X + (js1length/2) && y >= js1Y + (js1length/2)) {
            // 2st Quad
            angle += 90;
            A += lengthFromOriginX;
            O += lengthFromOriginY;
        }
        else if (x >= js1X + (js1length/2) && y >= js1Y + (js1length/2)) {
            // 3st Quad
            angle += 180;
            A += lengthFromOriginY;
            O += lengthFromOriginX;
        }
        else if (x >= js1X + (js1length/2) && y <= js1Y + (js1length/2)) {
            // 4st Quad
            angle += 270;
            A += lengthFromOriginX;
            O += lengthFromOriginY;
        }

        angle += atanf(O/A) * 180/PI;
    }

    // Only build player trail after time interval
    float currentUpdate = getElapsedTime();
    float elapsedSinceUpdate = currentUpdate - _previousTrailUpdate;
    bool build_trail = false;
    if (elapsedSinceUpdate >= TRAIL_UPDATE_INTERVAL) {
        build_trail = true;
        _previousTrailUpdate = currentUpdate;
    }

    for (int i=1; i<=TRAIL_PART_PER_UPDATE; i++) {
        // TODO replace (x, y) touch pos with a position returned from a controller class
        // Player[0] is the main player (the user)
        for(int p=0; p<(int)_players.size(); p++) {
            if (js1InputRecieved)
                _players.at(p)->update(x, y, -(360-angle), build_trail);
            else
                _players.at(p)->update(x, y, _players.at(p)->getRotAngle(), build_trail);
        }
    }
}

//---
void Game::loadAPK(const char *package_name) {
    LOGI("Loading APK %s", package_name);
    APKArchive = zip_open(package_name, 0, NULL);
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

/* Static Members */
int Game::_screen_width = 0;
int Game::getScreenWidth() { 
    return _screen_width; 
}

int Game::_screen_height = 0;
int Game::getScreenHeight() { 
    return _screen_height; 
}

float Game::_elapsed_time = 0.0f;
float Game::getElapsedTime() {
    return _elapsed_time; 
}

float Game::_time_speed = INIT_TIME_SPEED;
float Game::getTimeSpeed() {
    return _time_speed; 
}
