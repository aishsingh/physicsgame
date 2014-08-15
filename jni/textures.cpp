#include <assert.h>
#include <SOIL.h>
#include "textures.h"

// Textures::Textures() {
//     // Init with png image support
// }
// Textures::~Textures() {
//     // Delete textures
// }

void Textures::loadTextures() {
/* load an image file directly as a new OpenGL texture */
    GLuint tex_2d = SOIL_load_OGL_texture
        (
         "sprite.png",
         SOIL_LOAD_AUTO,
         SOIL_CREATE_NEW_ID,
         SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
        );
    glGenTextures(1, &tex_2d);
}

// SDL_Surface* Textures::loadFile(const char *path) {
//     // Load File
//     SDL_RWops *file = SDL_RWFromFile(path, "rb");
//
//     // Check file loaded
//     if (!file) {
//         SDL_Log("Could not load file %s, Error:%s", path, IMG_GetError());
//         exit(EXIT_FAILURE);
//     }
//
//     // Set surface
//     SDL_Surface *surf = IMG_Load_RW(file, 1);
//
//     if (!surf) {
//         SDL_Log("Could not set surface from file path %s, Error:%s", path, IMG_GetError());
//         exit(EXIT_FAILURE);
//     }
//
//     return surf;
// }
//
// void Textures::loadTextures(SDL_Renderer *renderer) {
//     // Load spritesheets
//     SDL_Surface *spaceman_run_left = loadFile("spritesheets/spaceman_run_left.png");
//     SDL_Surface *spaceman_run_right = loadFile("spritesheets/spaceman_run_right.png");
//
//     // Create textures
//     _tex_spaceman_run_left = SDL_CreateTextureFromSurface(renderer, spaceman_run_left);
//     _tex_spaceman_run_right = SDL_CreateTextureFromSurface(renderer, spaceman_run_right);
//
//     // Surface is no longer needed
//     SDL_FreeSurface(spaceman_run_left);
//     SDL_FreeSurface(spaceman_run_right);
// }
//
// void Textures::drawGUI(SDL_Renderer *renderer) {
//     // Joystick
//     SDL_Rect src_rect = {0, 0, 100, 100};
//     SDL_Rect dest_rect = {0, 0, 100, 100};
//     SDL_RenderCopy(renderer, _tex_spaceman_run_left, &src_rect, &dest_rect);
// }
//
// void Textures::drawSpacemen(SDL_Renderer *renderer, std::vector<Spaceman> &players) {
//     for (int i = 0; i < (int)players.size(); i++) {
//         SDL_Rect src_rect = getFrameRect(players.at(i).getFrame(), 385, 460, 15);
//         SDL_Rect dest_rect = {(int)players.at(i).getX(), (int)players.at(i).getY(), players.at(i).getWidth(), players.at(i).getHeight()};
//
//         switch (players.at(i).getFacing()) {
//             case LEFT:
//                 SDL_RenderCopy(renderer, _tex_spaceman_run_left, &src_rect, &dest_rect);
//                 break;
//
//             case RIGHT:
//                 SDL_RenderCopy(renderer, _tex_spaceman_run_right, &src_rect, &dest_rect);
//                 break;
//         }
//     }
// }
