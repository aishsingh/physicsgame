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
    _boxes_length = 15;
    _elapsed_time = 0.0f;
    _time_speed = 0.48f;
    _colour_theme = RAINBOW;
    
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
    // Clear boxes array
    std::vector<Box>().swap(boxes);
}
//----
void Game::setup(int w, int h) {
    _screen_width = w;
    _screen_height = h;
    renderer.setupGraphics(w,h);
}

void Game::run() {
    // if (!boxes.size())
    //     createBox(0, 0, 0.1f);
    // --- Main loop -----------------------------------------------------------------------------------------
    // while(!_finished) {
        // Clear screen to black
        // SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
        // SDL_RenderClear(_renderer);
        
        renderer.renderFrame();
        // Draw every box in boxes array&
        for (int i = 0; i < (int)boxes.size(); i++) {
            drawBox(boxes.at(i));
            // Update physics attributes only if box in not moving
            if (boxes[i].vert_motion.getVel() != 0.0f || boxes[i].hori_motion.getVel() != 0.0f)
                physics.updatePhysics(boxes[i], _elapsed_time, _screen_width, _screen_height); // Last 2 values are 1 as opengl positions the (x,y) axis from -1 - 1
        }

        /* TESTING
        // Max texture size = 4096 x 4096
        SDL_RendererInfo info;
        SDL_GetRendererInfo(_renderer, &info);
        SDL_Log("%i x %i", info.max_texture_width, info.max_texture_height);
        */

        // Draw textures
        // textures.drawSpacemen(_renderer, players);

        // Refresh Surface
        // SDL_RenderPresent(_renderer);

        _elapsed_time += _time_speed;
        // LOGI("time %.5f", _elapsed_time);
    // }
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
        float box_x = x - (_boxes_length/2); // + box_offset_x;
        float box_y = y - (_boxes_length/2); // + box_offset_y;

        createBox(box_x, box_y, _boxes_length);
    }
}

void Game::drawBox(Box &box) {
    // fade the box when it has stopped moving vertically
    float fade_decr = 0.04f;    // Larger = faster fade
    if (box.vert_motion.getVel() == 0 && box.getAlpha() >= fade_decr)
        box.setAlpha(box.getAlpha() - fade_decr);
        LOGI("alpha:%.2f", box.getAlpha());
    if (box.getAlpha() < fade_decr && box.getAlpha() > 0.0f)
        box.setAlpha(0.0f);

    // boxRGBA(_renderer, box.getX(), box.getY(), box.getX() + box.getWidth(), box.getY() + box.getHeight(), box.getColour().r, box.getColour().g, box.getColour().b, box.getAlpha());
    renderer.renderBox(box);

    if (box.getAlpha() <= 0 && (int)boxes.size() > 0 && box.vert_motion.getVel() == 0)
        removeBox(box.getIndex());
}

void Game::changeTheme(Theme &old_theme) {
    Theme new_theme = (Theme) 0;
    do {
        int rand_val = rand() % 250;
        if (rand_val >= 0 && rand_val < 50)
            new_theme = GRAY;
        else if (rand_val >= 50  && rand_val < 100)
            new_theme = RED;
        else if (rand_val >= 100 && rand_val < 150)
            new_theme = BLUE;
        else if (rand_val >= 150 && rand_val < 200)
            new_theme = PURPLE;
        else if (rand_val >= 200 && rand_val < 250)
            new_theme = RAINBOW;
    } while (new_theme == 0 || new_theme == old_theme);

    old_theme = new_theme;
}

void Game::createBox(const float x, const float y, const float diameter) {
    /* Increase size of array and check for exceptions */
    try {
        boxes.resize(boxes.size() + 1, Box(x, y, diameter, (int)boxes.size(), _elapsed_time, _colour_theme));
    }
    catch (std::exception &e) {
        LOGE("Error occured while creating box: %s", e.what());
    }

    LOGI("create_box() ->created %i", (int)boxes.size());

    /* Setup new box */
    Box *box = &boxes[(int)boxes.size() - 1];

    // Create the fountain effect
    physics.generateInitVelocity(*box);

    // Just for testing
    // box->hori_motion.setAccel(-1.3f);

    // Allow gravity to affect the box
    box->vert_motion.setAccel(physics.getGravity());
    // box->vert_motion.setAccel(-0.1f);
}

void Game::removeBox(const int index) {
    // Shift box positions
    if (index < (int)boxes.size() - 1) {
        for (int i = index; i < (int)boxes.size(); i++) {
            boxes[i] = boxes[i + 1];
            boxes[i].setIndex(i);
        }
    }

    // Decrease size of array & realloc with less mem
    boxes.resize((int)boxes.size() - 1);
    LOGI("remove_box() -> removed %i", index);

}
