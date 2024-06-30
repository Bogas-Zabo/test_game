#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Grid {

    public:
        Grid(float grid_size);
        void Set_Grid_size(float grid_size);
        float Get_Grid_size();
        float Snap_To_Grid(float value, float gridSize);

    private:
        float grid_size;
};
