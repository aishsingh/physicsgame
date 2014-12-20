#include <math.h>
#include "ui.h"
#include "game.h"

UI::UI() {}

void UI::draw(ScreenRenderer *rend) {
    // Render
    rend->render(getVerticeData(), getColourData(), 0, GL_LINES);
}

vector<float> UI::getVerticeData() {
    /*     [p3] [p4]          Origin Cross     
              ---             
        [p1]       [p5]          [p11]
           |   +   |     ---> [p9] + [p10]
        [p2]       [p6]          [p12]
              ---            
           [p8] [p7]                      */


    /* This is the original (x,y) that will now be transformed
     * before being passed to the vertex shader */
    float x = 150;
    float y = Game::getScreenHeight() - 200;
    float l = 200;
    float size = 60.0f;
    float offset = (l - size)/2;

    // Orgin points
    float ori_size = 10.0f;
    
    // Declare points (x,y)
    float vec[] = { x                        , y + offset               ,
                    x                        , y + size + offset        ,
                    x + offset               , y                        ,
                    x + size + offset        , y                        ,
                    x + l                    , y + offset               ,
                    x + l                    , y + size + offset        ,
                    x + offset               , y + l                    ,
                    x + size + offset        , y + l                    ,
                    x + (l/2) - (ori_size/2) , y + (l/2)                ,
                    x + (l/2) + (ori_size/2) , y + (l/2)                ,
                    x + (l/2)                , y + (l/2) - (ori_size/2) ,
                    x + (l/2)                , y + (l/2) + (ori_size/2) };

    return std::vector<float> (vec, vec + sizeof(vec) / sizeof(float));
}

vector<float> UI::getColourData() {
    Colour colour(1.0f, 0.0f, 0.0f, 1.0f);

    float clr[] = { colour.r, colour.g, colour.b, colour.a,
                    colour.r, colour.g, colour.b, colour.a,
                    colour.r, colour.g, colour.b, colour.a,
                    colour.r, colour.g, colour.b, colour.a };

    return vector<float> (clr, clr + sizeof(clr) / sizeof(float));
}
