#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include <GL/gl.h>
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "Board.hpp"
#include "Cell.hpp"
#include <string>

class GameWindow {
private:
    SDL_Window* window;
    SDL_GLContext gl_context;
    Board board;
    bool running;
    float stepsPerSecond;
    float cellSize;
    Owner currentToggling;
    std::string screenText;
public:
    GameWindow(int boardRows, int boardCols);
    void runProgram();
    void drawBoardUI();
    void cleanUpProgram();
    void handleGameEnd(GameResult gameResult);
};