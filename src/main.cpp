#include "GUI.hpp"
#include "Board.hpp"
#include "Cell.hpp"
#include <iostream>
// Helper to draw the board as a grid inside an ImGui window and handle clicks


int main(int, char**) {
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = NULL;
    RunProgram(window, gl_context);
    CleanUpProgram(window, gl_context);
    
    return 0;
}