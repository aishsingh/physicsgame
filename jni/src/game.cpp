#include <stdlib.h>
#include <time.h>
#include <exception>
#include <cstring>

#include "game.h"
#include "spaceman.h"
#include "collision.h"
#include "log.h"
#include "math.h"

#define TRAIL_UPDATE_INTERVAL 0.6f
#define TRAIL_PART_PER_UPDATE 1
#define OUT_OpenGL_Ver false

int Game::_screen_height(0);
int Game::_screen_width(0);
float Game::_elapsed_time(0.0f);
float Game::_time_speed(0.4f);

Game::Game(std::string pkg_name, int screen_w, int screen_h) : _user((screen_w/2) - 25, (screen_h/2) + 700, RAINBOW),
                                                               _cam(&_user),
                                                               input(true, VERT, &_user, &_cam) {
    // Init values
    _finished = false;
    _package_name = pkg_name;
    _screen_height = screen_h;
    _screen_width = screen_w;

    LOGI(" ");
    LOGI("--------------------");
    LOGI("Created game object");
    LOGI("-> APK: '%s'", pkg_name.c_str());

    // Seed random numbers using the time to improve randomness
    srand(time(NULL));

    // Init ptrs
    _ass_rend = NULL;
    _obj_rend = NULL;
    _scr_rend = NULL;

    setupObjs();
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
        _players.at(i)->resetTime(0);
}

void Game::setupObjs() {
    const int w = getScreenWidth();
    const int h = getScreenHeight();

    // Setup Player
    _players.push_back(&_user);
    // _players.push_back(new Spaceman((w/3), (h/2) + 700, RAINBOW));

    // Setup planets
    try {
        _planets.push_back(new Planet((w/2) + 100, (h/2) - 150, 400));
        _planets.push_back(new Planet((w/2) - 800, (h/2) - 150, 300));
    }
    catch (std::exception &e) {
        LOGE("Error creating planets: %s", e.what());
    }

    glViewport(0, 0, w, h);
    checkGlError("glViewport");

    glDisable(GL_DEPTH_TEST);
    checkGlError("glDisable(GL_DEPTH_TEST)");
}

void Game::setupGLContext(int screen_w, int screen_h) {
    /* NOTE: Static vars get destroyed when the app goes out of context, resulting 
             in random data. Therefore static vars need need to be reset here. */
    resetTime();

    // Log OpenGL details
    LOGI("Loading OpenGL context");
    LOGI("-> Screen Res [%d x %d]", screen_w, screen_h);

    // Show OpenGL details of device
    if (OUT_OpenGL_Ver) {
        printGLString("Version", GL_VERSION);
        printGLString("Vendor", GL_VENDOR);
        printGLString("Renderer", GL_RENDERER);
        printGLString("Extensions", GL_EXTENSIONS);
    }

    // Unzip apk using libzip
    LOGI("Unziping assets");
    _tex.loadAPK(_package_name.c_str());

    // Load textures here
    _tex.loadTextures();

    // Update device size
    _screen_width = screen_w;
    _screen_height = screen_h;

    // Setup renderers
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
    _ass_rend = new AssetRenderer(&_cam);
    _obj_rend = new ObjRenderer(&_cam);
    _scr_rend = new ScreenRenderer();

    glLineWidth(1.0f);
}

void Game::draw() {
    /* MAIN GAME LOOP */ 
    Renderer::clearScreen();

    applyGravity();

    // Render planets gravity area
    for(int i=0; i<(int)_planets.size(); i++)
        _planets.at(i)->drawGrav(_obj_rend);

    // Render player trails
    for(int i=0; i<(int)_players.size(); i++)
        _players.at(i)->drawTrail(_obj_rend, &_planets);

    _obj_rend->disableAttributes();

    // Render players
    for(int i=0; i<(int)_players.size(); i++)
        _players.at(i)->draw(_ass_rend, &_planets, &_tex);

    _ass_rend->disableAttributes();

    // Render planets
    for(int i=0; i<(int)_planets.size(); i++)
        _planets.at(i)->draw(_obj_rend);

    _obj_rend->disableAttributes();

    // Scale camera
    if (_user.getAction() == FLYING)
        _cam.setScaleFromDisp(_user.getClosestPlanetDisp());

    // Update all players (eg build player trail)
    respondToInput();

    // Increment game time according to the current speed of time
    _elapsed_time += _time_speed;
}

void Game::respondToInput() {
    // Check if there is a touch that should cause an action
    int touch_count = input.getCount();
    bool nav_active = input.isNavActive();
    if ((!nav_active && touch_count > 0) || (nav_active && touch_count > 1)) {
        // Only build player trail after time interval
        float cur_update = getElapsedTime();
        float elapsedSinceUpdate = cur_update - _previous_trail_update;
        if (elapsedSinceUpdate >= TRAIL_UPDATE_INTERVAL) {
            // build all trails 
            for (int i=1; i<=TRAIL_PART_PER_UPDATE; i++) {
                for(int p=0; p<(int)_players.size(); p++)
                    _players.at(p)->update();
            }

            // update values for next input
            _previous_trail_update = cur_update;
        }
    }
}

void Game::applyGravity() {
    for (int i=0; i<(int)_players.size(); i++)
        _players.at(i)->applyGravity(&_planets);
}

/* Static Members */
int Game::getScreenWidth() { 
    return _screen_width; 
}

int Game::getScreenHeight() { 
    return _screen_height; 
}

float Game::getElapsedTime() {
    return _elapsed_time; 
}

float Game::getTimeSpeed() {
    return _time_speed; 
}
