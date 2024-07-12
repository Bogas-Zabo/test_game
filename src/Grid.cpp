#include <other/Grid.hpp>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>

Grid::Grid(float size) : grid_size(size) {};

void Grid::Set_Grid_size(float size) {
    grid_size = size;
}

float Grid::Get_Grid_size() {
  return grid_size;
}

vector<vector<int>> Grid::Initialize_grid(float ratio, float square_side_length) {

    vector<vector<int>> grid_array;

    int num_columns = (int)(2.0f/square_side_length*ratio);
    int num_rows = (int)(2.0f/square_side_length);

    for (int y = 0; y<num_rows; y++) {
        vector<int> row_array;
        for (int x = 0; x<num_columns; x++) {
            int x_value = 0;
            row_array.push_back(x_value);
        }
        grid_array.push_back(row_array);
    }

    return grid_array;
}


vector<vector<int>> Grid::Resize_grid(int num_columns, int num_rows, vector<vector<int>> given_grid_array, float ratio, float square_side_length) {

    int updated_num_rows = num_rows;
    int updated_num_columns = (int)(num_columns*ratio);

    vector<vector<int>> updated_grid_array;

    for (int y = 0; y<updated_num_rows; y++) {
        vector<int> updated_row_array;
        for (int x = 0; x<updated_num_columns; x++) {
            int x_value = given_grid_array.at(y).at((int)(x/ratio));
            updated_row_array.push_back(x_value);
        }
        updated_grid_array.push_back(updated_row_array);
    }

    return updated_grid_array;
}

