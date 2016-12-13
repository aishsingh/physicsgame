#include <stdlib.h>
#include <time.h>
#include <exception>
#include <cstring>
#include <cmath>

#include "game.h"
#include "config.h"
#include "spaceman.h"
#include "collision.h"
#include "log.h"
#include "math.h"

int Game::_screen_height(0);
int Game::_screen_width(0);
float Game::_elapsed_time(0.0f);
float Game::_time_speed(TIME_DEFAULT_SPEED);

Game::Game(std::string pkg_name, int screen_w, int screen_h) : _galaxy(GALAXY_STARS, GALAXY_PLANETS),
                                                               _user((screen_w/2) - 25, (screen_h/3) + 750, USER_THEME),
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
    _player_rend = NULL;
    _back_rend = NULL;

    // Unzip apk using libzip
    LOGI("Unziping assets");
    _tex.loadAPK(_package_name.c_str());

    setupObjs();
}

Game::~Game() {
    // Clear all vectors
    for(int i=0; i<(int)_players.size(); i++)
        vector<Player*>().swap(_players);
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
    if (OUT_OPENGL_VER) {
        printGLString("Version", GL_VERSION);
        printGLString("Vendor", GL_VENDOR);
        printGLString("Renderer", GL_RENDERER);
        printGLString("Extensions", GL_EXTENSIONS);
    }

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
    if (_player_rend != NULL) {
        delete _player_rend;
        _player_rend = NULL;
    }
    if (_back_rend != NULL) {
        delete _back_rend;
        _back_rend = NULL;
    }
    _ass_rend = new AssetRenderer(&_cam);
    _obj_rend = new ObjRenderer(&_cam);
    _scr_rend = new ScreenRenderer();
    _player_rend = new PlayerRenderer(&_cam);
    _back_rend = new BackgroundRenderer(&_cam);

    // Disable depth rendering
    glDisable(GL_DEPTH_TEST);
    checkGlError("glDisable(GL_DEPTH_TEST)");

    glDisable(GL_DITHER);
    checkGlError("glDisable(GL_DITHER)");

    glDepthMask(GL_FALSE);
    checkGlError("glDepthMask()");

    // Use tightly packed data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    checkGlError("glPixelStorei()");

    // Enable Alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    checkGlError("glBlendFunc");

    // Lines used by planet gravity rings
    glLineWidth(3.0f);
}

void Game::draw() {
    /* MAIN GAME LOOP */ 
    Renderer::clearScreen();
    applyGravity();

    //-- RENDER ---------------------------------------
    if (!STATS_DISABLE) {
        _back_rend->render();
        _back_rend->disableAttributes();

        drawStats();
    }

    // draw galaxy full of stars
    _galaxy.draw(_obj_rend);

    // Render planets gravity area
    for(int i=0; i<(int)_galaxy.getGravObjs()->size(); i++)
        _galaxy.getGravObjs()->at(i)->drawGrav(_obj_rend);

    // Render player trails
    for(int i=0; i<(int)_players.size(); i++)
        _players.at(i)->drawTrail(_obj_rend, _galaxy.getGravObjs());

    _obj_rend->disableAttributes();

    // Render players
    for(int i=0; i<(int)_players.size(); i++) {
        _players.at(i)->updatePhysics(_galaxy.getGravObjs());
        _players.at(i)->draw(_player_rend, &_tex);
    }

    _player_rend->disableAttributes();

    // Render planets
    for(int i=0; i<(int)_galaxy.getGravObjs()->size(); i++)
        _galaxy.getGravObjs()->at(i)->draw(_obj_rend);

    _obj_rend->disableAttributes();

    drawGUI();
    //-------------------------------------------------

    // Recentre player by slowly decresing offset
    if (_user.getOrbitingPlanetsCount() == 1 && !input.isNavActive()) {
        float decr = _user.getRotAngleOffset()/15.f;
        if (_user.getRotAngleOffset() > decr ||
                _user.getRotAngleOffset() < -decr)
            _user.setRotAngleOffset(_user.getRotAngleOffset() -decr);
    }

    // Scale camera
    if (_user.getAction() == Action::FLYING || _user.getAction() == Action::LANDING)
        _cam.setScaleFromDisp(_user.getClosestPlanetDisp());

    // Update all players (eg build player trail)
    respondToInput();

    // Increment game time according to the current speed of time
    _elapsed_time += _time_speed;
}

void Game::respondToInput() {
    // Check if there is a touch that should cause an player action
    int touch_count = input.getCount();
    bool nav_active = input.isNavActive();
    bool is_input = ((!nav_active && touch_count > 0) || (nav_active && touch_count > 1));
    if (is_input) {
        // Only build player trail after time interval
        float cur_update = getElapsedTime();
        float elapsedSinceUpdate = cur_update - _previous_trail_update;
        if (elapsedSinceUpdate >= USER_UPDATE_INTERVAL) {
            // Build the trail 
            for (int i=1; i<=USER_UPDATE_PER_INTERVAL; i++)
                _user.updateAction(_galaxy.getGravObjs());

            // Update values for next input
            _previous_trail_update = cur_update;
        }
    }
    else {
        if (_user.getAction() == Action::RUNNING && !_user.vert_motion.getVel() && !_user.hori_motion.getVel())
            _user.setAction(Action::STILL);
    }

    // Update user's facing direction
    if (nav_active)
        _user.updateDir();
}

void Game::applyGravity() {
    for (int i=0; i<(int)_players.size(); i++)
        _players.at(i)->applyGravity();
}

void Game::drawGUI() {
    if (!_cam.isPtInCam(_galaxy.getCheckpoint(), NAV_ICON_SIZE*2)) {
        // Calc nav icon pos
        float o = _user.getCentreX() - _galaxy.getCheckpoint().getX();
        float a = _user.getCentreY() - _galaxy.getCheckpoint().getY();
        float angle = std::atan(o/a);
        a = Game::getScreenHeight()/2;
        o = std::tan(angle)*a;

        bool belowCheckpoint = false;
        if (_galaxy.getCheckpoint().getY() < _user.getCentreY()) {
            o *= -1;
            belowCheckpoint = true;
        }

        // Set pos
        Point2D nav;
        if ((Game::getScreenWidth()/2) + o > Game::getScreenWidth()) {
            a = (Game::getScreenWidth()/2) / (std::tan(angle));
            nav = Point2D(Game::getScreenWidth() - NAV_ICON_SIZE, (Game::getScreenHeight()/2) + a);
        }
        else if ((Game::getScreenWidth()/2) + o < 0) {
            a = (Game::getScreenWidth()/2) / (std::tan(angle));
            nav = Point2D(NAV_ICON_SIZE, (Game::getScreenHeight()/2) - a);
        }
        else
            nav = Point2D((Game::getScreenWidth()/2) + o, (belowCheckpoint) ? NAV_ICON_SIZE : Game::getScreenHeight() - NAV_ICON_SIZE);

        // Filter pos value
        if (nav.getX() < NAV_ICON_SIZE) nav.setX(NAV_ICON_SIZE);
        else if (nav.getX() > Game::getScreenWidth() - NAV_ICON_SIZE) nav.setX(Game::getScreenWidth() - NAV_ICON_SIZE);
        if (nav.getY() < NAV_ICON_SIZE) nav.setY(NAV_ICON_SIZE);
        else if (nav.getY() > Game::getScreenHeight() - NAV_ICON_SIZE) nav.setY(Game::getScreenHeight() - NAV_ICON_SIZE);

        // Show nav pos icon
        _scr_rend->render(Shape::genCircleVertices(nav, NAV_ICON_SIZE, 0.0f, 10),
                Colour::getColourData(10, Colour::getColour(GREY)),
                0.0f,
                GL_TRIANGLE_FAN);


        _scr_rend->disableAttributes();
    }
}

void Game::drawStats() {
    // Player collision stats
    for(int i=0; i<(int)_players.size(); i++)
        _players.at(i)->drawStats(_obj_rend);

    // Planet stats
    for(int i=0; i<(int)_galaxy.getGravObjs()->size(); i++)
        _galaxy.getGravObjs()->at(i)->drawStats(_obj_rend, (_galaxy.getGravObjs()->at(i) == _user.getOnPlanet()), _user.getOnPlanetRegion()); // 2 even showing as 4 even

    // // Show checkpoint direction
    // vector<float> vertices;
    // vertices.push_back(_galaxy.getCheckpoint().getX());
    // vertices.push_back(_galaxy.getCheckpoint().getY());
    // vertices.push_back(_user.getCentreX());
    // vertices.push_back(_user.getCentreY());
    //
    // _obj_rend->render(vertices,
    //         Colour::getColour(BLUE),
    //         0.0f,
    //         GL_LINES);
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
