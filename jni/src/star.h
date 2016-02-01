#ifndef STAR_H
#define STAR_H

#include "object.h"
#include "obj_renderer.h"
#include "theme.h"

class Star: public Object {
    private:
        typedef enum StarType {
            NORMAL = 0,
            SHINING
        } StarType;

        StarType _type;
        float _scale;
        float _scale_speed;
        bool _growing;

    public:
        /* Ctor */
        Star(float x, float y, float angle, float diameter);
        StarType getType() const;
        float getScale() const;
        void draw(ObjRenderer *rend);
        void growAndShrink();
};

#endif /* STAR_H */
