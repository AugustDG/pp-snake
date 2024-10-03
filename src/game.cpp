#include "game.hpp"

#include "raylib.h"
#include <random>
#include <settings.hpp>
#include <sstream>

void Game::init() {
  this->snake_one = std::make_shared<Snake>(3, Vector2Int{ map_size.x / 4 / CELL_SIZE, map_size.y / 2 / CELL_SIZE },
    RIGHT, GREEN, DARKGREEN);
  this->snake_two = std::make_shared<Snake>(3, Vector2Int{ 3 * map_size.x / 4 / CELL_SIZE, map_size.y / 2 / CELL_SIZE },
    LEFT, BLUE, DARKBLUE);
}

void Game::update() {
  // we want it static so that if the user pressed a key in a non-moving frame, once the snake moves, it will move in
  // the direction the user pressed
  static auto new_direction_one = NONE;
  static auto new_direction_two = NONE;

  if (IsKeyPressed(KEY_D))
    new_direction_one = RIGHT;
  else if (IsKeyPressed(KEY_A))
    new_direction_one = LEFT;
  else if (IsKeyPressed(KEY_W))
    new_direction_one = UP;
  else if (IsKeyPressed(KEY_S))
    new_direction_one = DOWN;

  if (IsKeyPressed(KEY_RIGHT))
    new_direction_two = RIGHT;
  else if (IsKeyPressed(KEY_LEFT))
    new_direction_two = LEFT;
  else if (IsKeyPressed(KEY_UP))
    new_direction_two = UP;
  else if (IsKeyPressed(KEY_DOWN))
    new_direction_two = DOWN;

  if (IsKeyPressed(KEY_ENTER) && is_game_over) {
    snake_one->reset(3, Vector2Int{ map_size.x / 4 / CELL_SIZE, map_size.y / 2 / CELL_SIZE }, RIGHT);
    snake_two->reset(3, Vector2Int{ 3 * map_size.x / 4 / CELL_SIZE, map_size.y / 2 / CELL_SIZE }, RIGHT);
    apples.clear();

    score_one = 0;
    score_two = 0;
    time = 0;
    move_time = BASE_MOVE_TIME;
    is_game_over = false;
  }

  if (time % move_time == 0) {
    updateSnake(new_direction_one, false);
    updateSnake(new_direction_two, true);
    updateGameOver();
    new_direction_one = NONE;
    new_direction_two = NONE;
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
  ss << "Score: " << score_one;
  DrawText(ss.str().c_str(), 10, 10, 20, BLACK);

  ss.str("");
  ss << "Score: " << score_two;
  DrawText(ss.str().c_str(), 10, 40, 20, BLACK);

  snake_one->render();
  snake_two->render();
}

void Game::updateSnake(const Direction new_direction, const bool is_two) {
  const auto snake = is_two ? snake_two : snake_one;

  snake->turn(new_direction);
  snake->move();

  // Check if snake ate an apple
  auto it = apples.begin();
  while (it != apples.end()) {
    if (snake->position == *it) {
      if (is_two)
        score_two++;
      else
        score_one++;

      snake->grow();

      apples.erase(it);

      break;
    }

    ++it;
  }
}

void Game::updateGameOver() {
  // self-collision
  if (snake_two->hasCollidedWithItself() || snake_one->hasCollidedWithItself() ||
    snake_one->hasCollidedWithSnake(snake_two)) {
    is_game_over = true;
  }

  // wall collision
  if (snake_one->position.x < 0 || snake_one->position.x >= map_size.x / CELL_SIZE || snake_one->position.y < 0 ||
    snake_one->position.y >= map_size.y / CELL_SIZE || snake_two->position.x < 0 ||
    snake_two->position.x >= map_size.x / CELL_SIZE || snake_two->position.y < 0 ||
    snake_two->position.y >= map_size.y / CELL_SIZE) {
    is_game_over = true;
  }
}

void Game::updateMoveTime() {
  move_time = BASE_MOVE_TIME - (score_one + score_two) / 4 - time / 600;
}

void Game::spawnApples() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution x_dist(0, (map_size.x - 1) / CELL_SIZE);
  std::uniform_int_distribution y_dist(0, (map_size.y - 1) / CELL_SIZE);

  auto apple_pos = Vector2Int(x_dist(rng), y_dist(rng));

  apples.emplace_back(apple_pos);
}