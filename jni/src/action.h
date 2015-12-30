/* Description: Action Enumeration used to tell if a player is flying or still etc.
 * Created On: 2015-01-21 
 */

#ifndef ACTION_H
#define ACTION_H

#include <string>

namespace Action {
    typedef enum Action {
        STILL, 
        RUNNING, 
        FLYING,
        LANDING
    } Action;

    static std::string toString(Action a) {
        switch (a) {
            case STILL:
                return "STILL";
            case RUNNING:
                return "RUNNING";
            case FLYING:
                return "FLYING";
            case LANDING:
                return "LANDING";
        }
    }
}

#endif /* ACTION_H */
