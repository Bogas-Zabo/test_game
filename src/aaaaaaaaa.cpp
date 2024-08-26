#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

// Define the grid size and cell size
const int gridWidth = 100;
const int gridHeight = 100;
const float cellSize = 10.0f;

// Enum for particle types
enum class ParticleType { Empty, Sand, Water };

// Grid to hold the particle types
ParticleType grid[gridHeight][gridWidth];

// Function to initialize the grid with some sand particles
void initializeGrid() {
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            grid[y][x] = (y > gridHeight / 2) ? ParticleType::Sand : ParticleType::Empty;
        }
    }
}

// Render function
void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            ParticleType type = grid[y][x];

            switch (type) {
                case ParticleType::Empty:
                    glColor3f(0.0f, 0.0f, 0.0f);
                    break;
                case ParticleType::Sand:
                    glColor3f(1.0f, 1.0f, 0.0f);
                    break;
                case ParticleType::Water:
                    glColor3f(0.0f, 0.0f, 1.0f);
                    break;
            }

            glBegin(GL_QUADS);
            glVertex2f(x * cellSize, y * cellSize);
            glVertex2f((x + 1) * cellSize, y * cellSize);
            glVertex2f((x + 1) * cellSize, (y + 1) * cellSize);
            glVertex2f(x * cellSize, (y + 1) * cellSize);
            glEnd();
        }
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Falling Sand Simulation", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    initializeGrid();

    while (!glfwWindowShouldClose(window)) {
        render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
