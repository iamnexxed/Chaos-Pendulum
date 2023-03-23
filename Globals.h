#ifndef GLOBALS_H
#define GLOBALS_H

#define WIDTH 720
#define HEIGHT 480
#define WINDOW_NAME "Chaos Pendulum"
#include <vector>

// Note: Please check Globals.cpp for correct comments and values
namespace Globals {
    extern const float kBG_COLOR_R;
    extern const float kBG_COLOR_G;
    extern const float kBG_COLOR_B;
    extern const float kBG_COLOR_A;

    extern const float CIRCLE_STEP;

    extern const float DEFAULT_ANGLE1;
    extern const float DEFAULT_ANGLE2;
    extern const float LENGTH1;
    extern const float LENGTH2;
    extern const float MASS1;
    extern const float MASS2;
    extern const float ROPE_THICKNESS;
    extern const float MAX_ANGULAR_VELOCITY;

    extern const float GRAVITY;
    extern const int MAX_TRAIL_POINTS;
}

#endif


