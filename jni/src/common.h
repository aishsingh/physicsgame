#ifndef COMMON_H
#define COMMON_H

typedef enum ColourTheme {GRAY=1, RED, PURPLE, BLUE, RAINBOW} Theme;

typedef struct Colour {
    float r, g, b, a;
} Colour;

/* Projectile motion components */
typedef enum Component {VERT, HORI, CIRC} Comp;   

/* Used by player.facing direction */
typedef enum Direction {LEFT, RIGHT} Dir;

#endif /* COMMON_H */
