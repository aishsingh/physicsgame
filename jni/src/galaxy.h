#ifndef GALAXY_H
#define GALAXY_H

#include <vector>
#include "star.h"
#include "obj_renderer.h"

class Galaxy {
    private:
        std::vector<Star*> _stars;

        void populate_stars(int count);

    public:
        /* Ctor */
        Galaxy();
        
        /* Draw all stars */
        void draw(ObjRenderer *rend);
};

#endif /* GALAXY_H */
