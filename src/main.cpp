#include "raylib.h"

#include "settings.h"
#include "snake.h"

#include <game.h>
#include <memory>

int main() {
    InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "pp snake - raylib");

    SetTargetFPS(60);

    const auto initial_position = Vector2Int(10, 10);
    const auto initial_map_size = Vector2Int(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    const auto game_ptr = std::make_unique<Game>(BASE_MOVE_TIME, initial_map_size);
    const auto snake_sptr = std::make_shared<Snake>(3, initial_position, RIGHT);

    game_ptr->addSnake(snake_sptr);

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
