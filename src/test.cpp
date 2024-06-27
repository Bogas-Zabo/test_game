#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

struct Square {
    float pos_x;
    float pos_y;
    float t;
};

static double cursor_x;
static double cursor_y;
static vector<Square> squares;

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

float v0 = 0;

void Draw_Square_Falling(const float ratio, GLuint VAO, GLuint VBO, float square_side_length, float& pos_x, float& pos_y, float& t) {

    float g = -0.2;

    float v = v0 + g*t;

    float vertex_point = square_side_length/2;
    float vertex_limit_point_x = 1-(vertex_point/ratio);
    float vertex_limit_point_y = 1-vertex_point;

    float bottom_vertex_y = -vertex_point+v*t+1/2*g*t*t+pos_y;
    float top_vertex_y = vertex_point+v*t+1/2*g*t*t+pos_y;

    float left_vertex_x = -vertex_point/ratio+pos_x;
    float right_vertex_x = vertex_point/ratio+pos_x;

    if (bottom_vertex_y<=-vertex_point-vertex_limit_point_y) {
        bottom_vertex_y=-vertex_point-vertex_limit_point_y;
        top_vertex_y=bottom_vertex_y+square_side_length;
    }
    
    GLfloat vertices[] = {
        left_vertex_x, bottom_vertex_y, 0.0f, // Bottom-left
        right_vertex_x, bottom_vertex_y, 0.0f,  // Bottom-right
        right_vertex_x, top_vertex_y, 0.0f,   // Top-right
        left_vertex_x, top_vertex_y, 0.0f   // Top-left
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Draw_Square_on_cursor(const float ratio, GLuint VAO, GLuint VBO, float square_side_length, float normalized_cursor_x, float normalized_cursor_y) {

    float vertex_point = square_side_length/2;

    GLfloat vertices[] = {
        -vertex_point/ratio+normalized_cursor_x, -vertex_point+normalized_cursor_y, 0.0f, // Bottom-left
        vertex_point/ratio+normalized_cursor_x, -vertex_point+normalized_cursor_y, 0.0f,  // Bottom-right
        vertex_point/ratio+normalized_cursor_x, vertex_point+normalized_cursor_y, 0.0f,   // Top-right
        -vertex_point/ratio+normalized_cursor_x, vertex_point+normalized_cursor_y, 0.0f   // Top-left
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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

    while (!glfwWindowShouldClose(window)) {

        double currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width/(float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glfwGetCursorPos(window, &cursor_x, &cursor_y);

        float normalized_cursor_x = (cursor_x/width) * 2.0f - 1.0f;
        float normalized_cursor_y = 1.0f - (cursor_y/height) * 2.0f; // Invert y-axis

        int leftMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

        //if (leftMouseButtonState == GLFW_PRESS) {... // to create continuous curves with little squares

        if (leftMouseButtonState == GLFW_PRESS  && prevMouseButtonState == GLFW_RELEASE) {
            left_button_normalized_x = normalized_cursor_x;
            left_button_normalized_y = normalized_cursor_y;
            squares.push_back({left_button_normalized_x, left_button_normalized_y, initial_t});
            click = true;
        }

        prevMouseButtonState = leftMouseButtonState;

        Draw_Square_on_cursor(ratio, VAO, VBO, square_side_length, normalized_cursor_x, normalized_cursor_y);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        for (auto& square : squares) {
            square.t += deltaTime;
            Draw_Square_Falling(ratio, VAO, VBO, square_side_length, square.pos_x, square.pos_y, square.t);
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