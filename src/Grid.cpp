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

vector<vector<int>> Grid::Put_square_on_grid(int square_type, vector<vector<int>> given_grid_array, int coord_x, int coord_y) {

    int num_rows = given_grid_array.size();
    int num_columns = given_grid_array.front().size();

    // if ((coord_x<num_columns)&&(coord_y<num_rows)&&(coord_x>=0)&&(coord_y>=0)&&(given_grid_array.at(coord_y).at(coord_x)!=2))
    if ((coord_x<num_columns)&&(coord_y<num_rows)&&(coord_x>=0)&&(coord_y>=0)) {
        given_grid_array.at(coord_y).at(coord_x) = square_type;
    }

    return given_grid_array;
}

vector<vector<int>> Grid::Resize_grid(vector<vector<int>> given_grid_array, float ratio, float square_side_length) {

    int num_rows = given_grid_array.size();
    int num_columns = given_grid_array.front().size();

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

