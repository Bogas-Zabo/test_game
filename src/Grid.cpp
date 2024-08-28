#include <other/Grid.hpp>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
using namespace std;

Grid::Grid(float size) : grid_size(size) {};

void Grid::Set_Grid_size(float size) {
    grid_size = size;
}

float Grid::Get_Grid_size() {
  return grid_size;
}

vector<vector<Square>> Grid::Initialize_grid(float ratio, float square_side_length) {

    vector<vector<Square>> grid_array;

    int num_columns = (int)(2.0f/square_side_length*ratio);
    int num_rows = (int)(2.0f/square_side_length);

    for (int y = 0; y<num_rows; y++) {
        vector<Square> row_array;
        for (int x = 0; x<num_columns; x++) {
            Square square(square_side_length, 0, 0, 0.0f, 0, false);
            //int x_value = 0;
            row_array.push_back(square);
        }
        grid_array.push_back(row_array);
    }

    return grid_array;
}


vector<vector<Square>> Grid::Resize_grid(int num_columns, int num_rows, vector<vector<Square>> given_grid_array, float ratio, float square_side_length) {

    int updated_num_rows = num_rows;
    int updated_num_columns = (int)(num_columns*ratio);

    vector<vector<Square>> updated_grid_array;

    for (int y = 0; y<updated_num_rows; y++) {
        vector<Square> updated_row_array;
        for (int x = 0; x<updated_num_columns; x++) {
            //int x_value = given_grid_array[y][(int)(x/ratio)];
            Square square = given_grid_array[y][(int)(x/ratio)];
            updated_row_array.push_back(square);
        }
        updated_grid_array.push_back(updated_row_array);
    }

    return updated_grid_array;
}

