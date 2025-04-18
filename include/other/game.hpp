#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Game {
    
    public:
        // game state
        GameState State;	
        bool Keys[1024];
        GLint Width, Height;
        // constructor/destructor
        Game(unsigned int width, unsigned int height);
        ~Game();
        // initialize game state (load all shaders/textures/levels)
        void Init();
        // game loop
        void ProcessInput(float dt);
        void Update(float dt);
        void Render();
};

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
}; 
