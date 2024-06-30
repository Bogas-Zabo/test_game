#include <other/Square.hpp>
#include <other/Grid.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <math.h>

Square::Square(float side_length, float norma_pos_x, float norma_pos_y, float time) : square_side_length(side_length), pos_x(norma_pos_x), pos_y(norma_pos_y), t(time) {};

float Square::Get_pos_x() {
    return pos_x;
}

float Square::Get_pos_y() {
    return pos_y;
}

void Square::Set_t(float time) {
    t = time;
}

float Square::Get_t() {
    return t;
}

void Square::Draw_Square_Falling(const float ratio, GLuint VAO, GLuint VBO, float& pos_x, float& pos_y, float& t) {
    float g = -0.01;
    float v = 0 + g*t;
    Grid gr(square_side_length/2);

    float vertex_point = square_side_length/2;
    float vertex_limit_point_x = 1 - (vertex_point/ratio);
    float vertex_limit_point_y = 1 - vertex_point;

    float bottom_vertex_y = -vertex_point + gr.Snap_To_Grid(+ v*t + 1/2*g*t*t + pos_y, square_side_length/2);
    float top_vertex_y = vertex_point + gr.Snap_To_Grid(v*t + 1/2*g*t*t + pos_y, square_side_length/2);

    float left_vertex_x = -vertex_point/ratio + gr.Snap_To_Grid(pos_x, square_side_length/2);
    float right_vertex_x = vertex_point/ratio + gr.Snap_To_Grid(pos_x, square_side_length/2);

    if (bottom_vertex_y <= -vertex_point-vertex_limit_point_y) {
        bottom_vertex_y = -vertex_point-vertex_limit_point_y;
        top_vertex_y = bottom_vertex_y+square_side_length;
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

void Square::Draw_Square_on_cursor(const float ratio, GLuint VAO, GLuint VBO, float normalized_cursor_x, float normalized_cursor_y) {
    float vertex_point = square_side_length/2;

    GLfloat vertices[] = {
        -vertex_point/ratio + normalized_cursor_x, -vertex_point+normalized_cursor_y, 0.0f, // Bottom-left
        vertex_point/ratio + normalized_cursor_x, -vertex_point+normalized_cursor_y, 0.0f,  // Bottom-right
        vertex_point/ratio + normalized_cursor_x, vertex_point+normalized_cursor_y, 0.0f,   // Top-right
        -vertex_point/ratio + normalized_cursor_x, vertex_point+normalized_cursor_y, 0.0f   // Top-left
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}