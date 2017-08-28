#ifndef GALAXY_H
#define GALAXY_H

#include <vector>
#include "grav_object.h"
#include "star.h"
#include "obj_renderer.h"

class Galaxy {
    private:
        std::vector<Star*> _stars;
        std::vector<GravObject*> _g_objs;
        Point2D _checkpoint;

        void populateStars(int count);
        void populateGravObjects(int count);

    public:
        Galaxy(int star_count, int gobj_count);
        ~Galaxy();
        
        /* Draw all stars */
        void drawStars(ObjRenderer *rend);

        const std::vector<GravObject*>* getGravObjs() const;
        Point2D getCheckpoint() const;
};

#endif /* GALAXY_H */
