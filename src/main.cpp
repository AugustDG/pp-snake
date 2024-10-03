#include "raylib.h"

#include "settings.hpp"
#include "snake.hpp"

#include <game.hpp>
#include <memory>

int main() {
    InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "pp snake - raylib");

    SetTargetFPS(60);

    const auto initial_map_size = Vector2Int(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
    const auto game_ptr = std::make_unique<Game>(BASE_MOVE_TIME, initial_map_size);

    game_ptr->init();

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        game_ptr->update();
        game_ptr->render();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
