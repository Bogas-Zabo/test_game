#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <other/Square.hpp>
#include <other/Grid.hpp>
#include <math.h>

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

vector<vector<int>> Initialize_grid(float ratio, float square_side_length) {

    vector<vector<int>> grid_array;

    int num_columns = (int)(2.0f/square_side_length*ratio);
    int num_rows = (int)(2.0f/square_side_length);

    for (int y = 0; y<num_rows; y++) {
        vector<int> row_array;
        for (int x = 0; x<num_columns; x++) {
            float x_value = 0;
            row_array.push_back(x_value);
        }
        grid_array.push_back(row_array);
    }

    return grid_array;
}

void Destroy_Grid(vector<vector<int>> given_grid_array) {

    int num_rows = given_grid_array.size();

    for (int y = 0; y<num_rows; y++) {
        given_grid_array.at(y).clear();
    }

    given_grid_array.clear();
}

void Draw_grid(vector<vector<int>> given_grid_array) {

    printf("\n\n");

    int num_rows = given_grid_array.size();
    int num_columns = given_grid_array.front().size();

    for (int y = 0; y<num_rows; y++) {
        for (int x = 0; x<num_columns; x++) {
            printf("%d", given_grid_array.at(y).at(x));
        }
        printf("\n");
    }
}

vector<vector<int>> Put_square_on_grid(int square_type, vector<vector<int>> grid_array, int coord_x, int coord_y) {

    int num_rows = grid_array.size();
    int num_columns = grid_array.front().size();

    if ((coord_x<num_columns)&&(coord_y<num_rows)&&(coord_x>=0)&&(coord_y>=0)) {
        grid_array.at(coord_y).at(coord_x) = square_type;
    }

    for (int y = 0; y<num_rows; y++) {
        for (int x = 0; x<num_columns; x++) {
            if ((square_type==1)&&((x!=coord_x)||(y!=coord_y))) {
                grid_array.at(y).at(x) = 0;
            }
        }
    }

    return grid_array;
}

void Draw_square(float grid_cursor_x, float grid_cursor_y, vector<vector<int>> given_grid_array, const float ratio, GLuint VAO, GLuint VBO) {

    float vertex_point = 0.02/2;

    int num_rows = given_grid_array.size();

    int num_columns = given_grid_array.front().size();

    for (int y = 0; y<num_rows; y++) {
        for (int x = 0; x<num_columns; x++) {
            if (given_grid_array.at(y).at(x)==1) {

                float normalized_cursor_x = ((x+1)/((float)num_columns+1.0f)) * 2.0f - 1.0f;
                float normalized_cursor_y = 1.0f - ((y+1)/((float)num_rows+1.0f)) * 2.0f; // Invert y-axis

                GLfloat vertices[] = {
                -vertex_point/ratio + normalized_cursor_x, -vertex_point+normalized_cursor_y, 0.0f, // Bottom-left
                vertex_point/ratio + normalized_cursor_x, -vertex_point+normalized_cursor_y, 0.0f,  // Bottom-right
                vertex_point/ratio + normalized_cursor_x, vertex_point+normalized_cursor_y, 0.0f,   // Top-right
                -vertex_point/ratio + normalized_cursor_x, vertex_point+normalized_cursor_y, 0.0f   // Top-left
                };

                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            }
        }
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

    GLuint shaderProgram = createShaderProgram();

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
    float initial_t = 0.0;

    double lastTime = glfwGetTime();

    float square_side_length = 0.02;
    float gridSize = square_side_length;

    Square sq(square_side_length, 0.0f,0.0f,0.0f);
    Grid gr(gridSize);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float previous_ratio = width / (float)height;

    vector<vector<int>> grid_array = Initialize_grid(previous_ratio, square_side_length);

    pair<int, int> grid_dimentions;

    grid_dimentions.first = grid_array.front().size(); // columns
    grid_dimentions.second = grid_array.size(); // rows

    //grid_array = Put_square_on_grid(grid_array, 0, 0);
    //grid_array = Put_square_on_grid(grid_array, 0, grid_dimentions.second-1);
    //grid_array = Put_square_on_grid(grid_array, grid_dimentions.first-1, 0);
    //grid_array = Put_square_on_grid(grid_array, 20, 20);
    

    //Draw_grid(grid_array);
    //Draw_square(grid_array, previous_ratio, VAO, VBO);

    int falling_square = 2;
    int cursor_square = 1;

    while (!glfwWindowShouldClose(window)) {;

        double currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        const float actual_ratio = width / (float)height;

        if (previous_ratio!=actual_ratio) {
            Destroy_Grid(grid_array);
            grid_array = Initialize_grid(actual_ratio, square_side_length);
            grid_dimentions.first = grid_array.front().size(); // columns
            grid_dimentions.second = grid_array.size(); // rows

            //grid_array = Put_square_on_grid(cursor_square, grid_array, 0, grid_dimentions.second-1);
            //Draw_grid(grid_array);

            //Draw_grid(grid_array);
            //printf("\ncolumns: %d, rows: %d\n", grid_dimentions.first, grid_dimentions.second);
            previous_ratio=actual_ratio;
        }

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glfwGetCursorPos(window, &cursor_x, &cursor_y);

        float grid_cursor_x = grid_dimentions.first*cursor_x/width;
        float grid_cursor_y = grid_dimentions.second*cursor_y/height;

        float normalized_cursor_x = (grid_cursor_x / grid_dimentions.first) * 2.0f - 1.0f;
        float normalized_cursor_y = 1.0f - (grid_cursor_y / grid_dimentions.second) * 2.0f; // Invert y-axis

        float snapped_cursor_x = gr.Snap_To_Grid(normalized_cursor_x, gridSize/actual_ratio);
        float snapped_cursor_y = gr.Snap_To_Grid(normalized_cursor_y, gridSize);

        int leftMouseButtonState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

        //if (leftMouseButtonState == GLFW_PRESS && prevMouseButtonState == GLFW_RELEASE) {... // to create individual little squares

        if (leftMouseButtonState == GLFW_PRESS) {
            left_button_normalized_x = snapped_cursor_x;
            left_button_normalized_y = snapped_cursor_y;
            Square sq(square_side_length, left_button_normalized_x, left_button_normalized_y, initial_t);
            squares_array.push_back(sq);
            click = true;
        }

        prevMouseButtonState = leftMouseButtonState;

        grid_array = Put_square_on_grid(cursor_square, grid_array, grid_cursor_x, grid_cursor_y);
        Draw_square(grid_cursor_x, grid_cursor_y, grid_array, actual_ratio, VAO, VBO);

        //sq.Draw_Square_on_cursor(actual_ratio, VAO, VBO, snapped_cursor_x, snapped_cursor_y);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        for (auto& square : squares_array) {
            square.Set_t(square.Get_t() + deltaTime*10);
            float norma_pos_x = square.Get_pos_x();
            float norma_pos_y = square.Get_pos_y();
            float t = square.Get_t();
            //sq.Draw_Square_Falling(actual_ratio, VAO, VBO, norma_pos_x, norma_pos_y, t);
            //glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
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