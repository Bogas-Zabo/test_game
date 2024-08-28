#include <other/Square.hpp>
#include <other/Grid.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
using namespace std;

Square::Square(float side_length, int norma_pos_x, int norma_pos_y, float time, int particle, bool state) : 
square_side_length(side_length), pos_x(norma_pos_x), pos_y(norma_pos_y), t(time), type(particle) , is_stationary(state) {};

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

void Square::Set_type(int particle) {
    type = particle;
}

int Square::Get_type() {
    return type;
}

void Square::Set_is_stationary(bool state) {
    is_stationary = state;
}

int Square::Get_is_stationary() {
    return is_stationary;
}

void Square::Put_square_on_grid(int num_columns, int num_rows, int square_type, vector<vector<Square>>& given_grid_array, int coord_x, int coord_y) {

    // if ((coord_x<num_columns)&&(coord_y<num_rows)&&(coord_x>=0)&&(coord_y>=0)) // to erase squares with cursor
    if ((coord_x<num_columns-1)&&(coord_y<num_rows)&&(coord_x>0)&&(coord_y>=0)&&(given_grid_array[coord_y][coord_x].Get_type()!=2)) {
        given_grid_array[coord_y][coord_x].Set_type(square_type);
        //given_grid_array[coord_y][coord_x].Set_pos_y(coord_y);
        //given_grid_array[coord_y][coord_x].Set_pos_x(coord_x);
        //given_grid_array[coord_y][coord_x].Set_t(0.0f);
        //given_grid_array[coord_y][coord_x].Set_is_stationary(false);
    }
}


void Square::Update_falling_squares(int pos_x_limit, int pos_y_limit, int grid_cursor_x, int grid_cursor_y, vector<vector<Square>>& given_grid_array) {

    int num_columns = pos_x_limit; // columns
    int num_rows = pos_y_limit; // rows

    for (int y = num_rows-1; y>=0; y--) { // from bottom to top

        for (int x = 0; x<num_columns; x++) { // from left to right

            if ((given_grid_array[y][x].Get_type()==1)&&((x!=grid_cursor_x)||(y!=grid_cursor_y))) { // cursor square
                given_grid_array[y][x].Set_type(0);
            }

            if (given_grid_array[y][x].Get_type()==2) { // falling square

                int new_pos_x = x;
                int left_new_pos_x = new_pos_x-1;
                int right_new_pos_x = new_pos_x+1;

                int future_pos_y = y+1;

                if ((future_pos_y < pos_y_limit)&&(future_pos_y >= 0)&&(right_new_pos_x < pos_x_limit)&&(left_new_pos_x >= 0)) {

                    if (given_grid_array[future_pos_y][new_pos_x].Get_type()==2) {

                            if (given_grid_array[future_pos_y][left_new_pos_x].Get_type()==0) {
                                given_grid_array[y][new_pos_x].Set_type(0);
                                given_grid_array[future_pos_y][left_new_pos_x].Set_type(2);
                            }
                            else if (given_grid_array[future_pos_y][right_new_pos_x].Get_type()==0) {
                                given_grid_array[y][new_pos_x].Set_type(0);
                                given_grid_array[future_pos_y][right_new_pos_x].Set_type(2);
                            }                
                    }
                    else {
                        given_grid_array[y][new_pos_x].Set_type(0);
                        given_grid_array[future_pos_y][new_pos_x].Set_type(2);
                    }
                }
            }
        }
    }
}


/*
void Square::Update_falling_squares(int pos_x_limit, int pos_y_limit, int grid_cursor_x, int grid_cursor_y, vector<vector<Square>>& given_grid_array) {

    int num_columns = pos_x_limit; // columns
    int num_rows = pos_y_limit; // rows

    float g = 9.81;
    float delta_t = 0.016;

    //vector<vector<int>> max_y_pos_array;
    //int max_y = 0;
    int* max_y_pos_array = static_cast<int*>(malloc(pos_x_limit*sizeof(int)));
    //int count = 0;

    for (int y = num_rows-1; y>=0; y--) { // from bottom to top

        //int count = 0;

        for (int x = 0; x<num_columns; x++) { // from left to right

            if ((given_grid_array[y][x].Get_type()==1)&&((x!=grid_cursor_x)||(y!=grid_cursor_y))) { // cursor square
                given_grid_array[y][x].Set_type(0);
            }

            max_y_pos_array[x] = pos_y_limit;

            if (given_grid_array[y][x].Get_type()==2) { // falling square

                if (given_grid_array[y][x].Get_is_stationary()) {
                    max_y_pos_array[x] = y;
                }

                //given_grid_array[y][x].Set_t(given_grid_array[y][x].Get_t()+delta_t);
                float t = given_grid_array[y][x].Get_t();
                int pos_y = given_grid_array[y][x].Get_pos_y();

                int new_pos_x = x;
                //int new_pos_x = given_grid_array[y][x].Get_pos_x();
                int left_new_pos_x = new_pos_x-1;
                int right_new_pos_x = new_pos_x+1;

                //float future_t = given_grid_array[y][x].Get_t();
                float future_t = t + delta_t;


                float v = 0 + g*future_t;

                int future_pos_y = (int)(pos_y + v*future_t + 1/2*g*future_t*future_t);
                //int future_pos_y = y+1;

                given_grid_array[y][new_pos_x].Set_t(future_t);

                if ((future_pos_y < pos_y_limit)&&(future_pos_y >= 0)&&(right_new_pos_x < pos_x_limit)&&(left_new_pos_x >= 0)) {

                    if ((given_grid_array[future_pos_y][new_pos_x].Get_type()==2)&&(future_pos_y!=pos_y)) {

                        
                        if ((given_grid_array[future_pos_y][left_new_pos_x].Get_type()==0)&&(given_grid_array[future_pos_y][right_new_pos_x].Get_type()==0)) {
                            given_grid_array[y][new_pos_x].Set_type(0);
                            given_grid_array[y][new_pos_x].Set_pos_y(0);
                            given_grid_array[future_pos_y][new_pos_x].Set_type(2);
                            given_grid_array[future_pos_y][new_pos_x].Set_t(future_t);
                            given_grid_array[future_pos_y][new_pos_x].Set_pos_y(pos_y);
                            //given_grid_array[future_pos_y][new_pos_x].Set_pos_x(new_pos_x);
                            given_grid_array[future_pos_y][new_pos_x].Set_is_stationary(false);
                        }                  
                        else if (given_grid_array[future_pos_y][left_new_pos_x].Get_type()==0) {
                            given_grid_array[y][new_pos_x].Set_type(0);
                            given_grid_array[y][new_pos_x].Set_pos_y(0);
                            given_grid_array[future_pos_y][left_new_pos_x].Set_type(2);
                            given_grid_array[future_pos_y][left_new_pos_x].Set_t(future_t);
                            given_grid_array[future_pos_y][left_new_pos_x].Set_pos_y(pos_y);
                            //given_grid_array[future_pos_y][left_new_pos_x].Set_pos_x(left_new_pos_x);
                            given_grid_array[future_pos_y][left_new_pos_x].Set_is_stationary(true);
                        } 
                        else if (given_grid_array[future_pos_y][right_new_pos_x].Get_type()==0) {
                            given_grid_array[y][new_pos_x].Set_type(0);
                            given_grid_array[y][new_pos_x].Set_pos_y(0);
                            given_grid_array[future_pos_y][right_new_pos_x].Set_type(2);
                            given_grid_array[future_pos_y][right_new_pos_x].Set_t(future_t);
                            given_grid_array[future_pos_y][right_new_pos_x].Set_pos_y(pos_y);
                            //given_grid_array[future_pos_y][right_new_pos_x].Set_pos_x(right_new_pos_x);
                            given_grid_array[future_pos_y][right_new_pos_x].Set_is_stationary(true);
                        }  
                    }
                    else {
                        //printf("max_y: %d, x: %d\n", max_y_pos_array[x], x);
                        given_grid_array[y][new_pos_x].Set_type(0);
                        given_grid_array[y][new_pos_x].Set_pos_y(0);
                        given_grid_array[future_pos_y][new_pos_x].Set_type(2);
                        given_grid_array[future_pos_y][new_pos_x].Set_t(future_t);
                        given_grid_array[future_pos_y][new_pos_x].Set_pos_y(pos_y);
                        //given_grid_array[future_pos_y][new_pos_x].Set_pos_x(new_pos_x);
                        given_grid_array[future_pos_y][new_pos_x].Set_is_stationary(false);
                    }
                }
            }
        }
        //max_y_pos_array.push_back(highest_y_pos_in_x);
    }
    free(max_y_pos_array);
}
*/

/*
// version 1 (square size = 0.02 -> ~20 fps)
void Square::Draw_square(int num_columns, int num_rows, vector<vector<int>>& given_grid_array, const float ratio, GLuint VAO, GLuint VBO) {

    float vertex_point = square_side_length/2;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    float normalized_cursor_x;
    float normalized_cursor_y;

    GLfloat vertices[12];

    for (int y = 0; y<num_rows; y++) {
        for (int x = 0; x<num_columns; x++) {
            if (given_grid_array[y][x]!=0) {

                normalized_cursor_x = ((x+1)/(static_cast<float>(num_columns)+1.0f)) * 2.0f - 1.0f;
                normalized_cursor_y = 1.0f - ((y+1)/(static_cast<float>(num_rows)+1.0f)) * 2.0f; // Invert y-axis

                // Bottom-left
                vertices[0] = -vertex_point / ratio + normalized_cursor_x;
                vertices[1] = -vertex_point + normalized_cursor_y;
                vertices[2] = 0.0f;

                // Bottom-right
                vertices[3] = vertex_point / ratio + normalized_cursor_x;
                vertices[4] = -vertex_point + normalized_cursor_y;
                vertices[5] = 0.0f;

                // Top-right
                vertices[6] = vertex_point / ratio + normalized_cursor_x;
                vertices[7] = vertex_point + normalized_cursor_y;
                vertices[8] = 0.0f;

                // Top-left
                vertices[9] = -vertex_point / ratio + normalized_cursor_x;
                vertices[10] = vertex_point + normalized_cursor_y;
                vertices[11] = 0.0f;


                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            }
        }
    }
}
*/

/*
// version 2 (square size = 0.02 -> ~100 fps) 5x fps
void Square::Draw_square(int num_columns, int num_rows, vector<vector<int>>& given_grid_array, const float ratio, GLuint VAO, GLuint VBO) {
    float vertex_point = square_side_length / 2;
    vector<GLfloat> verticesData;

    // Clear the old data from the VBO
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    for (int y = 0; y < num_rows; y++) {
        for (int x = 0; x < num_columns; x++) {
            if (given_grid_array[y][x] != 0) {
                // Calculate normalized cursor coordinates
                float normalized_cursor_x = ((x + 1) / (static_cast<float>(num_columns) + 1.0f)) * 2.0f - 1.0f;
                float normalized_cursor_y = 1.0f - ((y + 1) / (static_cast<float>(num_rows) + 1.0f)) * 2.0f; // Invert y-axis

                // Define vertices in the correct order (clockwise)
                verticesData.insert(verticesData.end(), {
                    -vertex_point / ratio + normalized_cursor_x, -vertex_point + normalized_cursor_y, 0.0f, // Bottom-left
                    vertex_point / ratio + normalized_cursor_x, -vertex_point + normalized_cursor_y, 0.0f,  // Bottom-right
                    vertex_point / ratio + normalized_cursor_x, vertex_point + normalized_cursor_y, 0.0f,   // Top-right
                    -vertex_point / ratio + normalized_cursor_x, vertex_point + normalized_cursor_y, 0.0f   // Top-left
                });
            }
        }
    }

    // Update the VBO with the new vertices data
    glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(GLfloat), verticesData.data(), GL_DYNAMIC_DRAW);

    // Set up vertex attribute pointers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Draw each square individually
    for (int i = 0; i < verticesData.size(); i += 4) {
        glDrawArrays(GL_TRIANGLE_FAN, i, 4); // Draw each square (4 vertices per square)
    }
}
*/

// version 3 (square size = 0.02 -> ~120 fps) 6x fps
void Square::Draw_square(int num_columns, int num_rows, vector<vector<Square>>& given_grid_array, const float ratio, GLuint VAO, GLuint VBO) {
    float vertex_point = square_side_length / 2;
    vector<GLfloat> verticesData;

    // Clear the old data from the VBO
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

    float normalized_cursor_x;
    float normalized_cursor_y;

    for (int y = 0; y < num_rows; y++) {

        normalized_cursor_y = 1.0f - ((y + 1) / (static_cast<float>(num_rows) + 1.0f)) * 2.0f; // Invert y-axis

        for (int x = 0; x < num_columns; x++) {
            if (given_grid_array[y][x].Get_type() != 0) {

                // Calculate normalized cursor coordinates
                normalized_cursor_x = ((x + 1) / (static_cast<float>(num_columns) + 1.0f)) * 2.0f - 1.0f;

                // Add two more sets of vertices for the next triangle in the square
                verticesData.insert(verticesData.end(), {
                    -vertex_point / ratio + normalized_cursor_x, -vertex_point + normalized_cursor_y, 0.0f, // Bottom-left
                    vertex_point / ratio + normalized_cursor_x, -vertex_point + normalized_cursor_y, 0.0f,  // Bottom-right
                    -vertex_point / ratio + normalized_cursor_x, vertex_point + normalized_cursor_y, 0.0f   // Top-left
                });

                // Add two more sets of vertices for the last triangle in the square
                verticesData.insert(verticesData.end(), {
                    vertex_point / ratio + normalized_cursor_x, -vertex_point + normalized_cursor_y, 0.0f,  // Bottom-right
                    -vertex_point / ratio + normalized_cursor_x, vertex_point + normalized_cursor_y, 0.0f,   // Top-left
                    vertex_point / ratio + normalized_cursor_x, vertex_point + normalized_cursor_y, 0.0f   // Top-right
                });
            }
        }
    }

    // Update the VBO with the new vertices data
    glBufferData(GL_ARRAY_BUFFER, verticesData.size() * sizeof(GLfloat), verticesData.data(), GL_DYNAMIC_DRAW);

    // Set up vertex attribute pointers
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    //glEnableVertexAttribArray(0);

    // Draw the squares
    // Since we're now using GL_TRIANGLES, we need to calculate the number of triangles per square
    glDrawArrays(GL_TRIANGLES, 0, verticesData.size() / 3);
}
