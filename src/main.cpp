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

  if (GameSaver::hasSaveGame(SAVE_GAME_FILENAME)) {
    GameSaver::getInstance().loadGame(SAVE_GAME_FILENAME, &game);
  } else {
    // if we have a save game, the snakes will be loaded from the save game
    game.createSnakes();
  }

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
