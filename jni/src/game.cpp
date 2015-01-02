#include <stdlib.h>
#include <time.h>
#include <exception>
#include <cstring>

#include "game.h"
#include "spaceman.h"
#include "log.h"

#define INIT_TIME_SPEED 0.4f
#define TRAIL_UPDATE_INTERVAL 0.5f
#define TRAIL_PART_PER_UPDATE 1
#define OUT_OpenGL_Ver false

Game::Game(std::string package_name) : _finished(false), _package_name(package_name), _previous_trail_update(0) {
    LOGI("--------------------");
    LOGI("Created game object");
    LOGI("pkgname: %s", _package_name.c_str());

    // Seed random numbers using the time to improve randomness
    srand(time(NULL));

    _ass_rend = NULL;
    _obj_rend = NULL;
    _scr_rend = NULL;
}

Game::~Game() {
    // Clear all vectors
    for(int i=0; i<(int)_players.size(); i++)
        vector<Player*>().swap(_players);
    for(int i=0; i<(int)_planets.size(); i++)
        vector<Planet*>().swap(_planets);
}

void Game::resetTime() {
    _elapsed_time = 0;
    _previous_trail_update = 0;

    for(int i=0; i<(int)_players.size(); i++)
        _players.at(i)->resetTime();
}

void Game::setupGLContext(int screen_w, int screen_h) {
    // NOTE: for some reason the elapsed time gets set to a random value and so all time vals needs to be reset
    resetTime();

    // Log OpenGL details
    LOGI("loading OpenGL context");
    LOGI("w: %d, h: %d, pkg: %s", screen_w, screen_h, _package_name.c_str());

    // Show OpenGL details of device
    if (OUT_OpenGL_Ver) {
        printGLString("Version", GL_VERSION);
        printGLString("Vendor", GL_VENDOR);
        printGLString("Renderer", GL_RENDERER);
        printGLString("Extensions", GL_EXTENSIONS);
    }

    // Unzip apk using libzip
    LOGI("unziping assets");
    loadAPK(_package_name.c_str());

    // Update device size
    _screen_width = screen_w;
    _screen_height = screen_h;

    // Load textures here
    // TODO

    if (_ass_rend != NULL) {
        delete _ass_rend;
        _ass_rend = NULL;
    }
    if (_obj_rend != NULL) {
        delete _obj_rend;
        _obj_rend = NULL;
    }
    if (_scr_rend != NULL) {
        delete _scr_rend;
        _scr_rend = NULL;
    }
    // Setup renderers
    _ass_rend = new AssetRenderer();
    _obj_rend = new ObjRenderer();
    _scr_rend = new ScreenRenderer();
}

void Game::setupObjs() {
    float w = getScreenWidth();
    float h = getScreenHeight();

    // Setup Player
    try {
        _players.push_back(new Spaceman((w/2) - 25, (h/2) - 50, Theme::GRAY));
    }
    catch (std::exception &e) {
        LOGE("Error creating players: %s", e.what());
    }

    // Setup planets
    try {
        _planets.push_back(new Planet((w/2) + 50, (h/2) - 150, 400, 10));
        _planets.push_back(new Planet((w/2) - 350, (h/2) - 150, 200, 10));
    }
    catch (std::exception &e) {
        LOGE("Error creating planets: %s", e.what());
    }
}

void Game::draw() {
    /* MAIN GAME LOOP */
    Renderer::clearScreen();

    applyGravity();

    // Render player trails
    for(int i=0; i<(int)_players.size(); i++)
        _players.at(i)->drawTrail(_obj_rend);

    // Render planets
    for(int i=0; i<(int)_planets.size(); i++)
        _planets.at(i)->draw(_obj_rend);

    _obj_rend->disableAttributes();

    // Render players
    for(int i=0; i<(int)_players.size(); i++)
        _players.at(i)->draw(_ass_rend);

    _ass_rend->disableAttributes();

    // Render UI
    screen_ui.draw(_scr_rend);

    // Increment game time according to the current speed of time
    _elapsed_time += _time_speed;
}

void Game::handleInput(float x, float y) {

    // Handle joystick input
    float js1Angle = screen_ui.getJoystickAngle(x, y);

    // Always rotate players whenever there is new input
    for(int p=0; p<(int)_players.size(); p++) {
        if (js1Angle != 0)
            _players.at(p)->setRotAngle(-(360-js1Angle));
        // _players.at(p)->setX(x);
        // _players.at(p)->setY(y);
    }

    // Only build player trail after time interval
    float currentUpdate = getElapsedTime();
    float elapsedSinceUpdate = currentUpdate - _previous_trail_update;
    if (elapsedSinceUpdate >= TRAIL_UPDATE_INTERVAL) {
        // build all trails 
        for (int i=1; i<=TRAIL_PART_PER_UPDATE; i++) {
            for(int p=0; p<(int)_players.size(); p++)
                _players.at(p)->update();
        }

        // update values for next input
        _previous_trail_update = currentUpdate;
    }
}

void Game::applyGravity() {
    for (int i=0; i<(int)_players.size(); i++)
        _players.at(i)->applyGravity(&_planets);
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
int Game::_screen_width(0);
int Game::getScreenWidth() { 
    return _screen_width; 
}

int Game::_screen_height(0);
int Game::getScreenHeight() { 
    return _screen_height; 
}

float Game::_elapsed_time(0);
float Game::getElapsedTime() {
    return _elapsed_time; 
}

float Game::_time_speed(INIT_TIME_SPEED);
float Game::getTimeSpeed() {
    return _time_speed; 
}
