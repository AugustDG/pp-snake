#include "game.h"

#include "raylib.h"
#include <random>
#include <settings.h>
#include <sstream>

void Game::addSnake(const std::shared_ptr<Snake> &snake) { this->snake = snake; }

void Game::update() {
  // we want it static so that if the user pressed a key in a non-moving frame, once the snake moves, it will move in
  // the direction the user pressed
  static auto new_direction = NONE;

  if (IsKeyPressed(KEY_RIGHT))
    new_direction = RIGHT;
  else if (IsKeyPressed(KEY_LEFT))
    new_direction = LEFT;
  else if (IsKeyPressed(KEY_UP))
    new_direction = UP;
  else if (IsKeyPressed(KEY_DOWN))
    new_direction = DOWN;

  if (IsKeyPressed(KEY_ENTER) && is_game_over) {
    snake->reset(3, Vector2Int{map_size.x / 2 / CELL_SIZE, map_size.y / 2 / CELL_SIZE}, RIGHT);
    apples.clear();

    score = 0;
    time = 0;
    move_time = BASE_MOVE_TIME;
    is_game_over = false;
  }

  if (time % move_time == 0) {
    updateSnake(new_direction);
    updateGameOver();
    new_direction = NONE;
  }

  if (time % (move_time * 15) == 0) {
    spawnApples();
  }

  updateMoveTime();

  time++;
}

void Game::render() const {
  if (is_game_over) {
    DrawText("Game Over", map_size.x / 2 - 50, map_size.y / 2 - 10, 20, BLACK);
    DrawText("Press Enter to restart", map_size.x / 2 - 100, map_size.y / 2 + 10, 20, BLACK);
    return;
  }

  DrawRectangleLines(0, 0, map_size.x, map_size.y, BLACK);

  for (const auto [x, y] : apples) {
    DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);
  }

  std::stringstream ss;
  ss << "Score: " << score;

  DrawText(ss.str().c_str(), 10, 10, 20, BLACK);

  snake->render();
}

void Game::updateSnake(const Direction new_direction) {

  snake->turn(new_direction);
  snake->move();

  // Check if snake ate an apple
  auto it = apples.begin();
  while (it != apples.end()) {
    if (snake->position == *it) {
      score++;
      snake->grow();

      apples.erase(it);

      break;
    }

    ++it;
  }
}

void Game::updateGameOver() {
  // self-collision
  if (snake->hasCollided()) {
    is_game_over = true;
  }

  // wall collision
  if (snake->position.x < 0 || snake->position.x >= map_size.x / CELL_SIZE || snake->position.y < 0 ||
      snake->position.y >= map_size.y / CELL_SIZE) {
    is_game_over = true;
  }
}

void Game::updateMoveTime() {
  if ((move_time == 30 && time > 10) || (move_time == 25 && time > 20)) {
    move_time -= 5;
  }
}

void Game::spawnApples() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution x_dist(0, (map_size.x - 1) / CELL_SIZE);
  std::uniform_int_distribution y_dist(0, (map_size.y - 1) / CELL_SIZE);

  auto apple_pos = Vector2Int(x_dist(rng), y_dist(rng));

  apples.emplace_back(apple_pos);
}