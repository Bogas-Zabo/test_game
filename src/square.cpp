#include <other/Square.hpp>
#include <other/Grid.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
using namespace std;

Square::Square(float side_length, int norma_pos_x, int norma_pos_y, float time, int falling_pos) : 
square_side_length(side_length), pos_x(norma_pos_x), pos_y(norma_pos_y), t(time), falling_pos_y(falling_pos) {};

void Square::Set_pos_x(int new_pos_x) {
    pos_x = new_pos_x;
}

int Square::Get_pos_x() {
    return pos_x;
}

void Square::Set_pos_y(int new_pos_y) {
    pos_y = new_pos_y;
}

int Square::Get_pos_y() {
    return pos_y;
}

void Square::Set_t(float time) {
    t = time;
}

float Square::Get_t() {
    return t;
}
void Square::Set_falling_pos_y(int falling_pos) {
    falling_pos_y = falling_pos;
}

int Square::Get_falling_pos_y() {
    return falling_pos_y;
}

void Square::Put_square_on_grid(int num_columns, int num_rows, int square_type, vector<vector<int>>& given_grid_array, int coord_x, int coord_y) {

    // if ((coord_x<num_columns)&&(coord_y<num_rows)&&(coord_x>=0)&&(coord_y>=0)) // to erase squares with cursor
    if ((coord_x<num_columns-1)&&(coord_y<num_rows)&&(coord_x>=1)&&(coord_y>=0)&&(given_grid_array.at(coord_y).at(coord_x)!=2)) {
        given_grid_array.at(coord_y).at(coord_x) = square_type;
    }
}

void Square::Update_falling_squares(int pos_x_limit, int pos_y_limit, int grid_cursor_x, int grid_cursor_y, vector<vector<int>>& given_grid_array) {

    int num_columns = pos_x_limit; // columns
    int num_rows = pos_y_limit; // rows
    float g = 9.81/3;

    for (int y = num_rows-1; y>=0; y--) { // from bottom to top
        for (int x = 0; x<num_columns; x++) { // from left to right

            if ((given_grid_array.at(y).at(x)==1)&&((x!=grid_cursor_x)||(y!=grid_cursor_y))) { // cursor square
                given_grid_array.at(y).at(x) = 0;
            }

            if (given_grid_array.at(y).at(x)==2) { // falling square

                int future_falling_pos_y = y+1;
                int new_pos_x = x;

                if (future_falling_pos_y >= pos_y_limit) {
                    future_falling_pos_y = pos_y_limit-1;
                }

                if (future_falling_pos_y < 0) {
                    future_falling_pos_y = 0;
                }
                                    
                if (new_pos_x+1 >= pos_x_limit) {
                    new_pos_x = pos_x_limit-2;    
                }

                if (new_pos_x-1 <= 0) {
                    new_pos_x = 1;    
                }

                given_grid_array.at(y).at(new_pos_x) = 0;
                
                if (given_grid_array.at(future_falling_pos_y).at(new_pos_x)==2) {

                    if (given_grid_array.at(future_falling_pos_y).at(new_pos_x+1)==0) {
                        given_grid_array.at(future_falling_pos_y).at(new_pos_x+1) = 2;
                    }
                    else if (given_grid_array.at(future_falling_pos_y).at(new_pos_x-1)==0) {
                        given_grid_array.at(future_falling_pos_y).at(new_pos_x-1) = 2;
                    }
                    else if ((given_grid_array.at(future_falling_pos_y).at(new_pos_x+1)==2)&&(given_grid_array.at(future_falling_pos_y).at(new_pos_x-1)==2)) {
                        given_grid_array.at(y).at(new_pos_x) = 2;
                    }
                }
                else {
                    given_grid_array.at(future_falling_pos_y).at(new_pos_x) = 2;
                }
            }
        }
    }
}

void Square::Draw_square(int num_columns, int num_rows, vector<vector<int>>& given_grid_array, const float ratio, GLuint VAO, GLuint VBO) {

    float vertex_point = square_side_length/2;
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    for (int y = 0; y<num_rows; y++) {
        for (int x = 0; x<num_columns; x++) {
            if (given_grid_array.at(y).at(x)!=0) {

                float normalized_cursor_x = ((x+1)/((float)num_columns+1.0f)) * 2.0f - 1.0f;
                float normalized_cursor_y = 1.0f - ((y+1)/((float)num_rows+1.0f)) * 2.0f; // Invert y-axis

                GLfloat vertices[] = {
                -vertex_point/ratio + normalized_cursor_x, -vertex_point+normalized_cursor_y, 0.0f, // Bottom-left
                vertex_point/ratio + normalized_cursor_x, -vertex_point+normalized_cursor_y, 0.0f,  // Bottom-right
                vertex_point/ratio + normalized_cursor_x, vertex_point+normalized_cursor_y, 0.0f,   // Top-right
                -vertex_point/ratio + normalized_cursor_x, vertex_point+normalized_cursor_y, 0.0f   // Top-left
                };

                glBindVertexArray(VAO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            }
        }
    }
}