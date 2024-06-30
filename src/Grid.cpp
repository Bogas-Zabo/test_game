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

float Grid::Snap_To_Grid(float value, float gridSize) {
    return round(value / gridSize) * gridSize;
}

