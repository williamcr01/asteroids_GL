#include "gfx/shaders.h"

struct State {
    GLFWwindow* window;
    int shaderProgram;
    struct Ship ship;
    dynArray bullets;
};