#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <other/Square.hpp>

using namespace std;


class Grid {

    public:
        Grid(float grid_size);
        void Set_Grid_size(float grid_size);
        float Get_Grid_size();        
        vector<vector<Square>> Initialize_grid(float ratio, float square_side_length);
        vector<vector<Square>> Resize_grid(int num_columns, int num_rows, vector<vector<Square>> given_grid_array, float ratio, float square_side_length);

    private:
        float grid_size;
};
