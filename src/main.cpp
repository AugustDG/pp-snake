#include "raylib.h"

#include "settings.h"
#include "snake.h"

#include <game.h>
#include <saving.h>

int main() {
  InitWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, "pp snake - raylib");

  SetTargetFPS(60);

  const auto initial_map_size = Vector2Int(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
  Game game{BASE_MOVE_TIME, initial_map_size};

  // load the game if it exists

  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(RAYWHITE);

    game.update();
    game.render();

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
