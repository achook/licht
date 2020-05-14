#ifndef COLORS_H
#define COLORS_H

#define COLORS_LENGTH 7

typedef struct {
    unsigned int red;
    unsigned int green;
    unsigned int blue;
} color_t;

color_t COLORS[COLORS_LENGTH] = {
    {255, 0, 0},
    {255, 255, 0},
    {0, 255, 0},
    {0, 255, 255},
    {0, 0, 255},
    {255, 0, 255},
    {255, 255, 255}
};

#endif