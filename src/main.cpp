#include "src/GUI.hpp"
#include "Board.hpp"
#include "Cell.hpp"
#include <iostream>


int main(int, char**) {
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context = NULL;
    RunProgram(window, gl_context);
    CleanUpProgram(window, gl_context);
    
    return 0;
}