#include "window.h"

const unsigned int SCR_WIDTH = 1024U;
const unsigned int SCR_HEIGHT = 1024U;

GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLFWwindow* initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    GLFWwindow* window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Asteroids", NULL, NULL);
    if (window == NULL) {
    printf("Failed to create a GLFW window");
    glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // load OpenGL function pointers
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
    printf("Failed to initialize GLAD");
    }
    return window;
}