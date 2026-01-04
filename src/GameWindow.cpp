#include "GameWindow.hpp"
#include <thread>
#include <chrono>
#include <vector>

GameWindow::GameWindow(int boardRows, int boardCols): 
board(boardRows, boardCols), window(nullptr), gl_context(NULL), 
cellSize(20.0f), stepsPerSecond(10.0f), running(false), playingAgainstAI(false) {}

void GameWindow::drawBoardUI() {
    if (showPreGameUI) {
        ImGui::Begin("Game Setup", nullptr,
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_AlwaysAutoResize);

        if (!choosingDifficulty) {
            ImGui::Text("Choose Game Type");
            ImGui::Separator();

            int gameType = playingAgainstAI ? 1 : 0;
            ImGui::RadioButton("Player vs Player", &gameType, 0);
            ImGui::RadioButton("Player vs AI", &gameType, 1);
            playingAgainstAI = (gameType == 1);

            ImGui::Spacing();

            if (ImGui::Button("Start Game", ImVec2(200, 30))) {
                if (playingAgainstAI) {
                    choosingDifficulty = true;
                } else {
                    showPreGameUI = false;
                }
            }
        }
        else {
            ImGui::Text("Choose AI Difficulty");
            ImGui::Separator();

            if (ImGui::Button("Easy")) {
                selectedDifficulty = Difficulty::easy;
                AIOpponent = std::make_unique<AIPlayer>(
                    Owner::Blue, selectedDifficulty
                );
                showPreGameUI = false;
            }
            if (ImGui::Button("Medium")) {
                selectedDifficulty = Difficulty::medium;
                AIOpponent = std::make_unique<AIPlayer>(
                    Owner::Blue, selectedDifficulty
                );
                showPreGameUI = false;
            }
            if (ImGui::Button("Hard")) {
                selectedDifficulty = Difficulty::hard;
                AIOpponent = std::make_unique<AIPlayer>(
                    Owner::Blue, selectedDifficulty
                );
                showPreGameUI = false;
            }
        }
        ImGui::End();
        return;
    }

    ImGui::Begin("Multiplayer Game of Life");
    if (board.getGameResult() == GameResult::None && board.getPlayerTurn() == Owner::None) {
        running = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Game", ImVec2(120,25))) {
        board.reset();
        running = false;
    }
    if (board.getGameResult() == GameResult::None) {
        if(board.getPlayerTurn() == Owner::None) {
        ImGui::SameLine();
        screenText = "Game is running...";
        }
        else if(board.getPlayerTurn() == Owner::Red) {
            ImGui::SameLine();
            screenText = "Red player's turn to place cells"; 
        }
        else {
            ImGui::SameLine();
            screenText = "Blue player's turn to place cells";
        }
    }
    ImGui::Text(screenText.c_str());
    ImGui::Text("Red Cells: %zu | Blue Cells: %zu | Generations: %zu", board.getRedCells(), board.getBlueCells(), board.getNumGenerations());

    // Grid area
    ImGui::BeginChild("BoardArea", ImVec2(0, 0), true);
    ImVec2 origin = ImGui::GetCursorScreenPos();
    ImDrawList *draw_list = ImGui::GetWindowDrawList();

    const int rows = board.getRows(); 
    const int cols = board.getCol();
    const ImVec2 board_size = ImVec2(cols * cellSize, rows * cellSize);

    ImGui::InvisibleButton("board_inv", board_size);
    ImVec2 top_left = origin;
    //AI player
    if (playingAgainstAI && board.getPlayerTurn() == Owner::Blue) {
        std::vector<std::pair<int, int>> moves = AIOpponent->chooseMove(board);
        for (auto& move : moves) {
            board.toggleBlueCell(move.first, move.second);
        }
    }
    // Click handling
    if (ImGui::IsItemActive() || ImGui::IsItemHovered()) {
        // Determine which cell the mouse is over when clicked
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            ImVec2 m = ImGui::GetIO().MousePos;
            int mc = (int)((m.x - top_left.x) / cellSize);
            int mr = (int)((m.y - top_left.y) / cellSize);
            if (mr >= 0 && mr < rows && mc >= 0 && mc < cols && board.getPlayerTurn() != Owner::None && !board.getCell(mr,mc).isAlive()) {
                if (board.getPlayerTurn() == Owner::Red) {
                    board.toggleRedCell(mr, mc);
                }
                else if (!playingAgainstAI) {
                    board.toggleBlueCell(mr, mc);
                }
            }
        }
    }    
    // Draw cells
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            ImVec2 a = ImVec2(top_left.x + c * cellSize, top_left.y + r * cellSize);
            ImVec2 b = ImVec2(a.x + cellSize, a.y + cellSize);

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

void GameWindow::runProgram() {
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
    window = SDL_CreateWindow("Multiplayer Game of Life - ImGui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 800, window_flags);
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO(); (void)io;
    ImGui::GetIO().IniFilename = nullptr;

    //Load higher quality text font
    io.Fonts->AddFontFromFileTTF("imgui-master/imgui-master/misc/fonts/Cousine-Regular.ttf", 20.0f);  
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Create a board and initialize needed variables
    const int frameDelay = 1000 / 60; // 60 FPS cap
    bool done = false;
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
        float elapsed = static_cast<float>(currentTime - lastStepTime);
        if(running && elapsed >= 1000.0 / stepsPerSecond) {
            board.stepBoard();
            auto gameResult = board.checkGameResult();
            if (gameResult != GameResult::None) {
                handleGameEnd(gameResult);
            }
            lastStepTime = currentTime;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        ImGui::SetNextWindowSize(ImVec2(1000, 800), ImGuiCond_Always);
        drawBoardUI();

        //Program rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
    cleanUpProgram();
}

void GameWindow::cleanUpProgram() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void GameWindow::handleGameEnd(GameResult gameResult)  {
    running = false;
    if (gameResult == GameResult::Tie) {
        screenText = "The game was a tie!";
    }
    else if (gameResult == GameResult::Red) {
        screenText = "Red player won the game!";
    }
    else {
        screenText = "Blue player won the game!";
    }
}