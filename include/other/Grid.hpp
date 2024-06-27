#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class Grid {

    public:
        void Init(unsigned int width, unsigned int height);
        void clear();
        void set(unsigned int x, unsigned int y, unsigned int color);
        void swap(unsigned int a, unsigned int b);
        bool isEmpty(unsigned int index);
};
