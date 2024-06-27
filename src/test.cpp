#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

static void cursor_position_callback(GLFWwindow* window, double x, double y)
{
    printf("%0.3f: Cursor position: %f %f (%+f %+f)\n",
           glfwGetTime(),
           x, y, x - cursor_x, y - cursor_y);

    cursor_x = x;
    cursor_y = y;
}

void Draw_Square_center(const float ratio, GLuint VAO, GLuint VBO, float square_side_length) {

    float vertex_point = square_side_length/2;
    
    GLfloat vertices[] = {
        -vertex_point/ratio, -vertex_point, 0.0f, // Bottom-left
        vertex_point/ratio, -vertex_point, 0.0f,  // Bottom-right
        vertex_point/ratio, vertex_point, 0.0f,   // Top-right
        -vertex_point/ratio, vertex_point, 0.0f   // Top-left
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void Draw_Square_edge(const float ratio, GLuint VAO, GLuint VBO, float square_side_length) {

    float vertex_point = square_side_length/2;
    float vertex_limit_point_x = 1-(vertex_point/ratio);
    float vertex_limit_point_y = 1-vertex_point;
    
    GLfloat vertices[] = {
        -vertex_point/ratio-vertex_limit_point_x, -vertex_point-vertex_limit_point_y, 0.0f, // Bottom-left
        vertex_point/ratio-vertex_limit_point_x, -vertex_point-vertex_limit_point_y, 0.0f,  // Bottom-right
        vertex_point/ratio-vertex_limit_point_x, vertex_point-vertex_limit_point_y, 0.0f,   // Top-right
        -vertex_point/ratio-vertex_limit_point_x, vertex_point-vertex_limit_point_y, 0.0f   // Top-left
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

float v0 = 0;

void Draw_Square_Falling(const float ratio, GLuint VAO, GLuint VBO, float square_side_length) {

    float g = -0.15;
    float t = (float)glfwGetTime();
    //printf("time: %.3f\n",t);

    float v = v0 + g*t;

    float vertex_point = square_side_length/2;
    float vertex_limit_point_x = 1-(vertex_point/ratio);
    float vertex_limit_point_y = 1-vertex_point;

    float bottom_vertex_y = -vertex_point+v*t+1/2*g*t*t;
    float top_vertex_y = vertex_point+v*t+1/2*g*t*t;

    if (bottom_vertex_y<=-vertex_point-vertex_limit_point_y) {
        bottom_vertex_y=-vertex_point-vertex_limit_point_y;
        top_vertex_y=bottom_vertex_y+square_side_length;
    }
    
    GLfloat vertices[] = {
        -vertex_point/ratio, bottom_vertex_y, 0.0f, // Bottom-left
        vertex_point/ratio, bottom_vertex_y, 0.0f,  // Bottom-right
        vertex_point/ratio, top_vertex_y, 0.0f,   // Top-right
        -vertex_point/ratio, top_vertex_y, 0.0f   // Top-left
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}


void Draw_Square_on_cursor(const float ratio, GLuint VAO, GLuint VBO, float square_side_length, float cursor_x, float cursor_y, int window_width, int window_height) {

    float vertex_point = square_side_length/2;
    float normalized_cursor_x = (cursor_x / window_width) * 2.0f - 1.0f;
    float normalized_cursor_y = 1.0f - (cursor_y / window_height) * 2.0f; // Invert y-axis

    printf("%0.3f: Cursor position: %f %f\n",
        glfwGetTime(),
        normalized_cursor_x, normalized_cursor_y);

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

    //glfwSetCursorPosCallback(window, cursor_position_callback);

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

    while (!glfwWindowShouldClose(window)) {

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = width / (float)height;

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glfwGetCursorPos(window, &cursor_x, &cursor_y);

        //Draw_Square_center(ratio, VAO, VBO, square_side_length);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        //Draw_Square_edge(ratio, VAO, VBO, square_side_length);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        //Draw_Square_Falling(ratio, VAO, VBO, square_side_length);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        Draw_Square_on_cursor(ratio, VAO, VBO, square_side_length, (float)cursor_x, (float)cursor_y, width, height);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

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