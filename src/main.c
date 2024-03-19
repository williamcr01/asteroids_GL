#include <stdbool.h>
#include <stdlib.h>
#include "util/dynArray.h"
#include "entity/ship.h"
#include "state.h"
// TODO implement bullet rendering, asteroid rendering, and collision detection

struct Bullet {
  Vector2 p1;
  Vector2 p2;
  float angle;
  float velocity;
  int id;
};

struct Asteroid {
  Vector2 startCenter;
  Vector2 p1;
  Vector2 p2;
  Vector2 p3;
  Vector2 p4;
  Vector2 p5;
  float angle;
  float velocity;
};

void processInput(GLFWwindow *window);

// Vertex shader source
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
    "}\0";

// Fragment shader source
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
                                   "}\n\0";

bool rotateLeft = false;
bool rotateRight = false;

const float rotationSpeed = 0.15f * PI / 180;
const float shipVelocity = 0.00048f;
const float drag = 0.00000025f;
const float bulletVelocity = 0.0004f;

float randomFLoat() {
  float randomFloat = (float)rand() / (float)RAND_MAX;
  return randomFloat * 2 - 1;
}

struct Ship ship = {
    {-0.04f, -0.04f},
    {-0.015, -0.02},
    {0.015, -0.02},
    {0.04f, -0.04f},
    {0.0f, 0.06f},
    0.5f * PI, // angle
    0.0f,      // rotationAngle
    0.0f       // velocity
};

dynArray bullets;
static int bulletId = 0;

struct State state;

int main() {
  float divisible = fmod(shipVelocity, drag);
  printf("vel mod drag %f", divisible);
  
  state.window = initWindow();
  printf("here\n");

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // delete shaders, no longer needed
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ship), &ship, GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void *)0);
  glEnableVertexAttribArray(0);

  initDynArray(&bullets, 0);
  printf("here 2\n");
  // start render loop
  while (glfwWindowShouldClose(state.window) == 0) {
    processInput(state.window);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    // Rotate the ship
    rotateShip(&ship);
    ship.angle += ship.rotationAngle;

    if (ship.velocity > 0) {
      movePoint(ship.angle, ship.velocity, &ship.p1);
      movePoint(ship.angle, ship.velocity, &ship.p2);
      movePoint(ship.angle, ship.velocity, &ship.p3);
      movePoint(ship.angle, ship.velocity, &ship.p4);
      movePoint(ship.angle, ship.velocity, &ship.p5);
      ship.velocity -= drag;
    }

    Vector2 shipBounds = checkShipOutOfBounds(&ship);
    //printf("x: %f y: %f\n", shipBounds.x, shipBounds.y);
    if(shipBounds.x != 0.0 || shipBounds.y != 0.0) {
      setPoint(-ship.p1.x, -ship.p1.y, &ship.p1); // inverse ship and coordinates
      setPoint(-ship.p2.x, -ship.p2.y, &ship.p2);
      setPoint(-ship.p3.x, -ship.p3.y, &ship.p3);
      setPoint(-ship.p4.x, -ship.p4.y, &ship.p4);
      setPoint(-ship.p5.x, -ship.p5.y, &ship.p5);
      ship.rotationAngle += PI; // rotate ship back from the inversion
      rotateShip(&ship);
    }

    // Update buffer data with rotated ship
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ship), &ship);

    glDrawArrays(GL_LINE_LOOP, 0, 5);

    glfwSwapBuffers(state.window);
    glfwPollEvents();
  }

  // de-allocate resoruces
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  printf("Application closed");
  glfwTerminate();

  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(state.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(state.window, 1);
  }
  if(glfwGetKey(state.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    Vector2 bulletSize;
    float distance = 0.01f; // Adjust the distance as needed
    // Calculate the new point coordinates
    bulletSize.x = ship.p5.x + distance * cos(ship.angle);
    bulletSize.y = ship.p5.y + distance * sin(ship.angle);
    struct Bullet bullet = {
      ship.p5,
      bulletSize,
      ship.angle,
      bulletVelocity,
      bulletId
    };
    //insertDynArray(&bullets, bulletId);
    bulletId++;
  }
  // move forward
  if (glfwGetKey(state.window, GLFW_KEY_W) == GLFW_PRESS) {
    ship.velocity = shipVelocity;
  }
  // check rotation left
  if (glfwGetKey(state.window, GLFW_KEY_A) == GLFW_PRESS) {
    rotateLeft = true;
  }
  if (glfwGetKey(state.window, GLFW_KEY_A) == GLFW_RELEASE) {
    rotateLeft = false;
  }
  // check rotation right
  if (glfwGetKey(state.window, GLFW_KEY_D) == GLFW_PRESS) {
    rotateRight = true;
  }
  if (glfwGetKey(state.window, GLFW_KEY_D) == GLFW_RELEASE) {
    rotateRight = false;
  }
  if (rotateRight == true) {
    ship.rotationAngle = -rotationSpeed;
  } else if (rotateLeft == true) {
    ship.rotationAngle = rotationSpeed;
  } else {
    ship.rotationAngle = 0.0f;
  }
}