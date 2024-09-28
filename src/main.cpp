#include "raylib.h"

#include "settings.h"
#include "snake.h"

int main() {
    InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "snake - raylib");

    SetTargetFPS(60);

    Snake snake(3, {10, 10}, Direction::RIGHT);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (IsKeyPressed(KEY_RIGHT))
            snake.move(Direction::RIGHT);
        else if (IsKeyPressed(KEY_LEFT))
            snake.move(Direction::LEFT);
        else if (IsKeyPressed(KEY_UP))
            snake.move(Direction::UP);
        else if (IsKeyPressed(KEY_DOWN))
            snake.move(Direction::DOWN);

        snake.render();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
