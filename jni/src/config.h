/* Description: This config is created to quickly alter game mechanics
 * Created On: 2016-02-20 
 */

#ifndef CONFIG_H
#define CONFIG_H
/******************************************/

// --- GAME CONFIG ------------------------
#define STATS_DISABLE true
#define STATS_COLOUR Colour::getColour(PURPLE)
#define TIME_DEFAULT_SPEED 0.4f
#define USER_UPDATE_INTERVAL 0.6f
#define USER_UPDATE_PER_INTERVAL 1
#define USER_THEME GREY
#define OUT_OPENGL_VER false
#define NAV_ICON_SIZE 35

// --- RENDERER CONFIG --------------------
#define BG_COLOUR Colour(0.918f, 0.629f, 0.512f, 1.0f)
// #define BG_COLOUR Colour(0.41176, 0.40392f, 0.45098f, 1.0f)

// --- TEXTURE CONFIG ---------------------
#define OUT_TEX_APK_CONTENTS false
#define OUT_TEX_ANIMATION_CELL false

// --- GALAXY CONFIG ----------------------
#define GALAXY_STARS 2000
#define GALAXY_PLANETS 40
#define GALAXY_RANGE_X 10000  // 10,000x10,000 area
#define GALAXY_RANGE_Y 10000  

// --- PLAYER CONFIG ----------------------
#define STATS_PLAYER_REGION_PT true
#define STATS_PLAYER_CLOSEST_GOBJ true
#define STATS_PLAYER_RUNNING_DIR true
#define STATS_PLAYER_TRAIL false
#define PLAYER_THRUST_MAX_INIT_V 1.0f // Pushes new boxes out with some initial velocity
#define PLAYER_THRUST_MIN_INIT_V 1.5f // up is neg, down is pos, left is neg, right is pos
#define PLAYER_THRUST_OFFSET 0.0f
#define PLAYER_SHADE true
#define PLAYER_SHADE_COLOUR "-= vec4(0.1, 0.1, 0.1, 0.25);\n"
#define OUT_PLAYER_ACTION false
#define OUT_PLAYER_POS true
#define OUT_PLAYER_SURFACE_OFFSET false

// --- PLANET CONFIG ----------------------
#define STATS_PLANET_NORMALS true
#define STATS_PLANET_FIRST_NORMAL true
#define STATS_PLANET_VR true
#define PLANET_AVERAGE_SIDES 30 //30      // The more sides (or vertices) the more round the object looks
#define PLANET_RAND_SIDES false       // The sides will not be symetrical if true
#define PLANET_RAND_SIDES_OFFSET 15.0f
#define PLANET_COLOUR Colour(0.9294f, 0.898f, 0.88627f, 0.3f)

// --- GRAV OBJECT CONFIG -----------------
#define STATS_GOBJ_BORDER true
#define GOBJ_AVERAGE_SIDES 35
#define GOBJ_RING_SHAKE Math::offsetDataByRand(vert, -4.0f, 4.0f)
#define GOBJ_OPACITY 0.5f
#define GOBJ_ROTATE true
#define GOBJ_REGION_HEIGHT 150

// --- TRAIL CONFIG -----------------------
#define TRAIL_LENGTH 22
#define TRAIL_FADE_DEC 0.1f
#define TRAIL_SHRINK_DEC 0.6f
#define OUT_TRAIL_SHAPE_COUNT false

/******************************************/
#endif /* CONFIG_H */
