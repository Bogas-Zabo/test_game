#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <other/Square.hpp>

using namespace std;

static double cursor_x;
static double cursor_y;
vector<Square> squares_array;

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

GLuint compileShader(GLenum type, const char* source) {
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

GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

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

    GLuint shaderProgram = createShaderProgram();

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    float square_side_length = 0.02;

    float left_button_normalized_x;
    float left_button_normalized_y;

    bool click = false;
    int prevMouseButtonState = GLFW_RELEASE;
    float initial_t = 0.0;

    double lastTime = glfwGetTime();

    vector<Square> squares_array;
    Square sq(0.0f,0.0f,0.0f);

    while (!glfwWindowShouldClose(window)) {

        double currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glfwGetCursorPos(window, &cursor_x, &cursor_y);

        float normalized_cursor_x = (cursor_x / width) * 2.0f - 1.0f;
        float normalized_cursor_y = 1.0f - (cursor_y / height) * 2.0f; // Invert y-axis

        int leftMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

        //if (leftMouseButtonState == GLFW_PRESS && prevMouseButtonState == GLFW_RELEASE) {... // to create continuous curves with little squares

        if (leftMouseButtonState == GLFW_PRESS) {
            left_button_normalized_x = normalized_cursor_x;
            left_button_normalized_y = normalized_cursor_y;
            Square sq(left_button_normalized_x, left_button_normalized_y, initial_t);
            squares_array.push_back({sq.Get_pos_x(), sq.Get_pos_y(), sq.Get_t()});
            click = true;
        }

        prevMouseButtonState = leftMouseButtonState;

        sq.Draw_Square_on_cursor(ratio, VAO, VBO, square_side_length, normalized_cursor_x, normalized_cursor_y);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        for (auto& square : squares_array) {
            square.Set_t(square.Get_t()+deltaTime);
            float norma_pos_x = square.Get_pos_x();
            float norma_pos_y = square.Get_pos_y();
            float t = square.Get_t();
            sq.Draw_Square_Falling(ratio, VAO, VBO, square_side_length, norma_pos_x, norma_pos_y, t);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }

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