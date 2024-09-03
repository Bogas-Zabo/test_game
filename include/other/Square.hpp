#ifndef SQUARE_HPP
#define SQUARE_HPP

#include <vector>
#include <glad/glad.h>
using namespace std;

class Square {

    public:
        Square(float square_side_length, int falling_pos_y, float initial_t, int type);
        void Set_pos_y(int pos_y);
        int Get_pos_y();
        void Set_t(float time);
        float Get_t();
        void Set_type(int particle);
        int Get_type();
        void Put_square_on_grid(int num_columns, int num_rows, int square_type, vector<vector<Square>>& given_grid_array, int coord_x, int coord_y);        
        void Erase_square_from_grid(int num_columns, int num_rows, vector<vector<Square>>& given_grid_array, int coord_x, int coord_y);
        void Update_falling_squares(int pos_x_limit, int pos_y_limit, int grid_cursor_x, int grid_cursor_y, vector<vector<Square>>& given_grid_array);
        void Draw_square(int num_columns, int num_rows, vector<vector<Square>>& given_grid_array, const float ratio, GLuint VAO, GLuint VBO);
        

    private:
        float square_side_length;
        int falling_pos_y;
        float t;
        int type;
};

#endif // SQUARE_HPP