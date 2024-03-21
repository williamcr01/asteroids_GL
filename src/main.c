#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "entity/asteroid.h"
#include "state.h"

// TODO: implement bullet rendering, asteroid rendering, and collision detection

void processInput(GLFWwindow *window);

bool rotateLeft = false;
bool rotateRight = false;

const float rotationSpeed = 0.15f * PI / 180;
const float bulletVelocity = 0.0001f;

float randomFLoat() {
    float randomFloat = (float)rand() / (float)RAND_MAX;
    return randomFloat * 2 - 1;
}

struct State state;

clock_t timeSinceLastBullet;

dynArray bullets;
static int bulletId = 0;

int main() {
    state.ship = initShip();
    state.bullets = bullets;
    float divisible = fmod(state.ship.shipVelocity, state.ship.drag);
    printf("vel mod drag %f\n", divisible);

    initDynArrayBullet(&state.bullets, 1);

    timeSinceLastBullet = clock();

    state.window = initWindow();         // initialize window
    state.shaderProgram = initShaders(); // initialize shaders

    unsigned int VBO, VAO, bulletVBO, bulletVAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &bulletVAO);
    glGenBuffers(1, &bulletVBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(state.ship), &state.ship,
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), (void *)0);
    glEnableVertexAttribArray(0);

    // start render loop
    while (glfwWindowShouldClose(state.window) == 0) {
        processInput(state.window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(state.shaderProgram);
        glBindVertexArray(VAO);

        // Rotate the ship
        rotateShip(&state.ship);
        state.ship.angle += state.ship.rotationAngle;

        if (state.ship.currVelocity > 0) {
            movePoint(state.ship.angle, state.ship.currVelocity,
                      &state.ship.p1);
            movePoint(state.ship.angle, state.ship.currVelocity,
                      &state.ship.p2);
            movePoint(state.ship.angle, state.ship.currVelocity,
                      &state.ship.p3);
            movePoint(state.ship.angle, state.ship.currVelocity,
                      &state.ship.p4);
            movePoint(state.ship.angle, state.ship.currVelocity,
                      &state.ship.p5);
            state.ship.currVelocity -= state.ship.drag;
            if (state.ship.currVelocity < 0) {
                state.ship.currVelocity = 0;
            }
        }

        Vector2 shipBounds = checkShipOutOfBounds(&state.ship);
        if (shipBounds.x != 0.0 || shipBounds.y != 0.0) {
            setPoint(-state.ship.p1.x, -state.ship.p1.y,
                     &state.ship.p1); // inverse ship and coordinates
            setPoint(-state.ship.p2.x, -state.ship.p2.y, &state.ship.p2);
            setPoint(-state.ship.p3.x, -state.ship.p3.y, &state.ship.p3);
            setPoint(-state.ship.p4.x, -state.ship.p4.y, &state.ship.p4);
            setPoint(-state.ship.p5.x, -state.ship.p5.y, &state.ship.p5);
            state.ship.rotationAngle +=
                PI; // rotate ship back from the inversion
            rotateShip(&state.ship);
        }

        if (state.bullets.size > 0) {
            for (int i = 0; i < state.bullets.size; i++) {
                if (state.bullets.array[i].bulletId >= 0) {
                    struct Bullet *bullet = &state.bullets.array[i];
                    movePoint(bullet->angle, bullet->velocity, &bullet->p1);
                    movePoint(bullet->angle, bullet->velocity, &bullet->p2);
                    if (bullet->p1.x > 1 || bullet->p1.x < -1 ||
                        bullet->p1.y > 1 || bullet->p1.y < -1) {
                        removeElementBullet(&state.bullets, i);
                    }
                    // printf("Bullet position: %f, %f\n",
                    // bullet->p1.x,bullet->p1.y);
                    // printf("Bullet id: %d\n", bullet->bulletId);
                }
            }
        }

        // Update buffer data with rotated ship
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(state.ship), &state.ship);

        glDrawArrays(GL_LINE_LOOP, 0, 5);

        glfwSwapBuffers(state.window);
        glfwPollEvents();
    }

    // de-allocate resoruces
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(state.shaderProgram);
    freeDynArrayBullet(&state.bullets);

    printf("Application closed");
    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(state.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(state.window, 1);
    }
    if (glfwGetKey(state.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        if ((float)(clock() - timeSinceLastBullet) / CLOCKS_PER_SEC > 1.0f) {
            Vector2 bulletSize;
            float distance = 0.01f; // Adjust the distance as needed
            // Calculate the new point coordinates
            bulletSize.x = state.ship.p5.x + distance * cos(state.ship.angle);
            bulletSize.y = state.ship.p5.y + distance * sin(state.ship.angle);
            struct Bullet bullet = {state.ship.p5, bulletSize, state.ship.angle,
                                    bulletVelocity, bulletId};
            insertDynArrayBullet(&state.bullets,
                                 bullet); // insert bullet into array

            bulletId++;
            for (int i = 0; i < state.bullets.size; i++) {
                printf("Bullet id: %d\n", state.bullets.array[i].bulletId);
            }
            timeSinceLastBullet = clock();
        }
    }
    // move forward
    if (glfwGetKey(state.window, GLFW_KEY_W) == GLFW_PRESS) {
        state.ship.currVelocity = state.ship.shipVelocity;
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
        state.ship.rotationAngle = -rotationSpeed;
    } else if (rotateLeft == true) {
        state.ship.rotationAngle = rotationSpeed;
    } else {
        state.ship.rotationAngle = 0.0f;
    }
}