#include <stdlib.h>
#include <time.h>
#include <exception>

#include "game.h"
#include "spaceman.h"
#include "log.h"

#define INIT_TIME_SPEED 0.4f
#define TRAIL_UPDATE_INTERVAL 0.5f
#define TRAIL_PART_PER_UPDATE 1

Game::Game() : _finished(false) {
    _elapsed_time = 0; // Needs to be init here also for some reason
    _previous_trail_update = 0;
    
    loadResources();
    
    // Seed random numbers using the time to improve randomness
    srand(time(NULL));

    // Setup Player
    try {
        _players.push_back(new Spaceman(300, 310, Theme::GRAY));
        _players.push_back(new Spaceman(0,     0, Theme::RAINBOW));
        _players.push_back(new Spaceman(850, 310, Theme::GRAY));
    }
    catch (std::exception &e) {
        LOGE("Error creating player: %s", e.what());
    }

    // Setup planets
    try {
        _planets.push_back(new Planet(0, 0, 400));
    }
    catch (std::exception &e) {
        LOGE("Error creating planet: %s", e.what());
    }

}

Game::~Game() {
    // Clear all vectors
    for(int i=0; i<(int)_players.size(); i++)
        vector<Player*>().swap(_players);
    for(int i=0; i<(int)_planets.size(); i++)
        vector<Planet*>().swap(_planets);
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

    loadResources();

    // Put player in the center now that width and height are determined
    _players.at(1)->setX((w/2) - 25);
    _players.at(1)->setY((h/2) - 50);

    _planets.at(0)->setX((w/2));
    _planets.at(0)->setY(h);
}

void Game::run() {
    /* MAIN GAME LOOP */
    Renderer::clearScreen();

    // Render player trails
    for(int i=0; i<(int)_players.size(); i++)
        _players.at(i)->drawTrail(&_obj_rend);

    // Render planets
    for(int i=0; i<(int)_planets.size(); i++)
        _planets.at(i)->draw(&_obj_rend);

    _obj_rend.disableAttributes();

    // Render players
    for(int i=0; i<(int)_players.size(); i++)
        _players.at(i)->draw(&_ass_rend);

    _ass_rend.disableAttributes();

    // Render UI
    screen_ui.draw(&_scr_rend);

    // Increment game time according to the current speed of time
    _elapsed_time += _time_speed;
}

void Game::loadResources() {
    // Load textures here
    // TODO

    // Setup renderers
    _ass_rend.setup();
    _obj_rend.setup();
    _scr_rend.setup();
}

void Game::handleInput(float x, float y) {
    // Handle joystick input
    float js1Angle = screen_ui.getJoystickAngle(x, y);

    // Only build player trail after time interval
    float currentUpdate = getElapsedTime();
    float elapsedSinceUpdate = currentUpdate - _previous_trail_update;
    bool build_trail = false;
    if (elapsedSinceUpdate >= TRAIL_UPDATE_INTERVAL) {
        build_trail = true;
        _previous_trail_update = currentUpdate;
    }

    for (int i=1; i<=TRAIL_PART_PER_UPDATE; i++) {
        for(int p=0; p<(int)_players.size(); p++) {
            // TODO make player rotate func coz x,y cord dont matter right now
            if (js1Angle != 0)
                _players.at(p)->update(x, y, -(360-js1Angle), build_trail);
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
