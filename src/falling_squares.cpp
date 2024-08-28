#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <other/Square.hpp>
#include <other/Grid.hpp>
#include <math.h>
#include <string>

using namespace std;

static double cursor_x;
static double cursor_y;

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red color
}
)";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

GLuint compile_Shader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << endl;
    }
    return shader;
}

GLuint create_Shader_Program() {
    GLuint vertexShader = compile_Shader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compile_Shader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

double lastTime = 0.0;
int frameCount = 0;

void calculateAndDisplayFPS(GLFWwindow* window) {
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - lastTime;
    frameCount++;

    if (deltaTime >= 0.05) { // Update every 0.05 seconds
        int fps = (int)(frameCount/deltaTime);
        string title = "Falling particles game | FPS: " + to_string(fps);
        glfwSetWindowTitle(window, title.c_str());

        frameCount = 0;
        lastTime = currentTime;
    }
}

int main() {
    if (!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "window", NULL, NULL);

    if (window == NULL) {
        cout << "Failed to open GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    GLuint shaderProgram = create_Shader_Program();

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    float left_button_normalized_x;
    float left_button_normalized_y;

    bool click = false;
    int prevMouseButtonState = GLFW_RELEASE;
    float initial_t = 0.0f;

    float square_side_length = 0.02;
    float gridSize = square_side_length;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float window_ratio = width / (float)height;

    Square sq(square_side_length, 0, 0, 0.0f, 0, false);
    Grid gr(gridSize);

    vector<vector<Square>> grid_array = gr.Initialize_grid(window_ratio, square_side_length);

    pair<int, int> grid_dimentions;

    grid_dimentions.first = grid_array.front().size(); // columns
    grid_dimentions.second = grid_array.size(); // rows
    float previous_grid_ratio = grid_dimentions.first/(float)grid_dimentions.second;

    int falling_square = 2;
    int cursor_square = 1;

    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        calculateAndDisplayFPS(window);      

        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);

        const float actual_grid_ratio = grid_dimentions.first/(float)grid_dimentions.second;

        if (previous_grid_ratio!=actual_grid_ratio) {
            grid_array = gr.Resize_grid(grid_dimentions.first, grid_dimentions.second, grid_array, actual_grid_ratio, square_side_length);
            grid_dimentions.first = grid_array.front().size(); // columns
            grid_dimentions.second = grid_array.size(); // rows
            previous_grid_ratio=actual_grid_ratio;
        }

        glfwGetCursorPos(window, &cursor_x, &cursor_y);

        int grid_cursor_x = (int)(grid_dimentions.first*cursor_x/width);
        int grid_cursor_y = (int)(grid_dimentions.second*cursor_y/height);

        int leftMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

        //if (leftMouseButtonState == GLFW_PRESS && prevMouseButtonState == GLFW_RELEASE) {... // to create individual little squares

        if (leftMouseButtonState == GLFW_PRESS) {
        //if (leftMouseButtonState == GLFW_PRESS && prevMouseButtonState == GLFW_RELEASE) {
            sq.Put_square_on_grid(grid_dimentions.first, grid_dimentions.second, falling_square, grid_array, grid_cursor_x, grid_cursor_y);
            click = true;
        }
        else {
            sq.Put_square_on_grid(grid_dimentions.first, grid_dimentions.second, cursor_square, grid_array, grid_cursor_x, grid_cursor_y);
        }

        prevMouseButtonState = leftMouseButtonState;
        
        sq.Update_falling_squares(grid_dimentions.first, grid_dimentions.second, grid_cursor_x, grid_cursor_y, grid_array);

        sq.Draw_square(grid_dimentions.first, grid_dimentions.second, grid_array, actual_grid_ratio, VAO, VBO);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}