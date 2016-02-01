#ifndef GALAXY_H
#define GALAXY_H

#include <vector>
#include "star.h"
#include "obj_renderer.h"

class Galaxy {
    private:
        std::vector<Star*> _stars;

    public:
        /* Ctor */
        Galaxy();
        
        /* Draw all stars */
        void draw(ObjRenderer *rend);
        void populate();
};

#endif /* GALAXY_H */
