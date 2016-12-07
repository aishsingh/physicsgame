#ifndef GALAXY_H
#define GALAXY_H

#include <vector>
#include "star.h"
#include "obj_renderer.h"

class Galaxy {
    private:
        std::vector<Star*> _stars;
        Point2D _checkpoint;

        void populate_stars(int count);

    public:
        /* Ctor */
        Galaxy();
        
        /* Draw all stars */
        void draw(ObjRenderer *rend);

        Point2D getCheckpoint() const;
};

#endif /* GALAXY_H */
