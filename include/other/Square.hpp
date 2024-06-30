#include <vector>
#include <glad/glad.h>

class Square {

    public:
        Square(float square_side_length, float pos_x, float pos_y, float initial_t);
        float Get_pos_x();
        float Get_pos_y();
        void Set_t(float time);
        float Get_t();
        void Draw_Square_Falling(const float ratio, GLuint VAO, GLuint VBO,float& pos_x, float& pos_y, float& t);
        void Draw_Square_on_cursor(const float ratio, GLuint VAO, GLuint VBO, float normalized_cursor_x, float normalized_cursor_y);

    private:
        float square_side_length;
        float pos_x;
        float pos_y;
        float t;
};
