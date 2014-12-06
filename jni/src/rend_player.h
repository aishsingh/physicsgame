#ifndef REND_PLAYER_H
#define REND_PLAYER_H

#include <GLES2/gl2.h>
#include "renderer.h"
#include "point2d.h"

class Rend_player: public Renderer {
    private:
        GLuint gsTexHandle;
        Point2D _base;

        // PNG loading
        static void png_zip_read(png_structp png_ptr, png_bytep data, png_size_t length);
        GLubyte* getBytesFromPNG(const char* filename, zip *APKArchive, int &width, int &height);

        // Texture loading
        GLuint tex;
        GLuint createSimpleTexture2D(GLuint _textureid, GLubyte* pixels, int width, int height, int channels);

        std::vector<float> useObjectVertices(Object *obj);
        std::vector<float> useColour(Colour *colour); 
        void setShaderData(float vertices[], float colours[], float angle);

    public:
        void renderObject(Object *obj);
        void disableAttributes();
        Point2D getBasePoint(); // return pos of the bottom of the player after rotations
        bool setup();

        /* Ctor - Init Shaders */
        Rend_player();
};

#endif /* REND_PLAYER_H */
