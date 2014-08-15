#ifndef TEXTURES_H
#define TEXTURES_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <GLES2/gl2platform.h>

#include <vector>
#include <png.h>
#include "players.h"

class Textures {
private:
    typedef struct rect {
        float x, y, w, h;
    }rect;
 
    /* Load images and handle error checking 
       TODO add/use zlib and compress all assets, then change function to load to memory
       more info on rwops here -> http://www.kekkai.org/roger/sdl/rwops/rwops.html */
    // SDL_Surface* loadFile(const char *path);

public:
    void loadTextures();
    /* Returns the rect containing the pos of the given frame in any spritesheet
       The `spritesheet_rows` is used to calulate the column that the frame is on */
    rect getFrameRect(int frame, const float frame_width, const float frame_height, const float spritesheet_rows);

    // void drawGUI(SDL_Renderer *renderer);
    //
    // /* Draw all loaded textures to the provided render */
    // void drawSpacemen(SDL_Renderer *renderer, std::vector<Spaceman> &players);
    //
    // /* Constructor
    //    Init SDL_image lib */
    // Textures();
    //
    // /* Destructor
    //    Quit SDL_image lib */
    // ~Textures();
};

#endif /* TEXTURES_H */
