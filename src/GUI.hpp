#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL_opengl.h>
#include "imgui.h"
#include <GL/gl.h>
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "Board.hpp"
#include "Cell.hpp"

void DrawBoardUI(Board &board, bool &running, float &stepsPerSecond, float &zoom, Owner &currentToggle);
void RunProgram(SDL_Window*& window, SDL_GLContext gl_context);
void CleanUpProgram(SDL_Window* window, SDL_GLContext& gl_context);