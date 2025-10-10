#include "GUI.hpp"
#include <thread>
#include <chrono>

void DrawBoardUI(Board &board, bool &running, float &stepsPerSecond, float &zoom, Owner &currentToggling) {
    ImGui::Begin("Multiplayer Game of Life");
    if (ImGui::Button(running ? "Pause" : "Run", ImVec2(90,25))) {
        running = !running;
    }
    ImGui::SameLine();
    if (ImGui::Button("Step", ImVec2(90,25))) {
        board.stepBoard();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear", ImVec2(90,25))) {
        board.clear();
    }
    ImGui::SameLine();
    if (ImGui::Button(currentToggling == Owner::Red ? "Red Toggle" : "Blue Toggle", ImVec2(90,25))) {
        if (currentToggling == Owner::Red) {
            currentToggling = Owner::Blue;
        }
        else {
            currentToggling = Owner::Red;
        }
    }
    
    ImGui::SliderFloat("Cell Size", &zoom, 6.0f, 48.0f);
    ImGui::SliderFloat("Run Speed", &stepsPerSecond, 2.0f, 20.0f);
    ImGui::Text("Board: %zu x %zu", board.getRows(), board.getCol());

    // Grid area
    ImGui::BeginChild("BoardArea", ImVec2(0, 0), true);
    ImVec2 origin = ImGui::GetCursorScreenPos();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    const float cell_size = zoom;
    const int rows = (int)board.getRows(); 
    const int cols = (int)board.getCol();
    const ImVec2 board_size = ImVec2(cols * cell_size, rows * cell_size);

    // Reserve space for the grid
    ImGui::InvisibleButton("board_inv", board_size);
    ImVec2 top_left = origin;
    // Click handling
    if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
        // Determine which cell the mouse is over when clicked
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            ImVec2 m = ImGui::GetIO().MousePos;
            int mc = (int)((m.x - top_left.x) / cell_size);
            int mr = (int)((m.y - top_left.y) / cell_size);
            if (mr >= 0 && mr < rows && mc >= 0 && mc < cols) {
                if (currentToggling == Owner::Red) {
                    board.toggleRedCell(mr, mc);
                }
                else {
                    board.toggleBlueCell(mr, mc);
                }
            }
        }
    }    
    // Draw cells
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            ImVec2 a = ImVec2(top_left.x + c * cell_size, top_left.y + r * cell_size);
            ImVec2 b = ImVec2(a.x + cell_size, a.y + cell_size);

            bool alive = board.getCell(r, c).getState() == CellState::Alive;
            ImU32 col;
            if(alive && board.getCell(r,c).getOwner() == Owner::Blue) {
                col = IM_COL32(0, 122, 255, 255); 
            }
            else if(alive && board.getCell(r,c).getOwner() == Owner::Red) {
                col = IM_COL32(255, 0, 0, 255);
            } 
            else {
                col = IM_COL32(30, 30, 30, 255);
            }
            draw_list->AddRectFilled(a, b, col);
            draw_list->AddRect(a, b, IM_COL32(80, 80, 80, 255));

            
        }
    }

    ImGui::EndChild();
    ImGui::End();
}
void RunProgram(SDL_Window*& window, SDL_GLContext gl_context) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return;
    }

    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    window = SDL_CreateWindow("Multiplayer Game of Life - ImGui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, window_flags);
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Create a board and initialize needed variables
    Board board(30, 40);
    bool running = false;
    float cell_size = 16.0f;
    const int frameDelay = 1000 / 60; // 60 FPS cap
    Owner currentToggling = Owner::Red;
    bool done = false;
    float stepsPerSecond = 4;
    int lastStepTime = SDL_GetTicks();

    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }
        uint32_t frameStart = SDL_GetTicks();
        uint32_t frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }

        //Board step speed logic
        int currentTime = SDL_GetTicks();
        float elapsed = (currentTime - lastStepTime);
        if(running && elapsed >= 1000.0 / stepsPerSecond) {
            board.stepBoard();
            lastStepTime = currentTime;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        DrawBoardUI(board, running, stepsPerSecond, cell_size, currentToggling);

        //Program rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
    
}

void CleanUpProgram(SDL_Window* window, SDL_GLContext& gl_context) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}