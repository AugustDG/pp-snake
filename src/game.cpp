#include "game.h"

#include "raylib.h"
#include <random>
#include <saving.h>
#include <settings.h>
#include <sstream>

void Game::createSnakes() {
    // create the snakes
}

void Game::update() {
  // we want it static so that if the user pressed a key in a non-moving frame, once the snake moves, it will move in
  // the direction the user pressed
  static auto new_direction_one = NONE;

  if (time % move_time == 0) {
    updateSnake(new_direction_one, false);

    new_direction_one = NONE;
  }

  time++;
}

void Game::render() const {
  // Map Grid
  for (int i = 0; i <= viewport_size.x / CELL_SIZE; i++) {
    DrawLine(i * CELL_SIZE, 0, i * CELL_SIZE, viewport_size.y, LIGHTGRAY);
  }

  for (int i = 0; i <= viewport_size.y / CELL_SIZE; i++) {
    DrawLine(0, i * CELL_SIZE, viewport_size.x, i * CELL_SIZE, LIGHTGRAY);
  }

  // Draw Map Border
  DrawRectangleLines(0, 0, viewport_size.x, viewport_size.y, BLACK);

  // Snakes
}

uint64_t Game::getScore(const bool is_two) const {
  return is_two ? snake_two->length - STARTING_SNAKE_SIZE : snake_one->length - STARTING_SNAKE_SIZE;
}

std::ostream &operator<<(std::ostream &os, const Game &game) {
  os << "GO:" << game.is_game_over << ",T:" << game.time << ",MT:" << game.move_time << ",LA:" << game.apples.size()
     << std::endl;
  os << "A" << std::endl;

  for (auto &&apple : game.apples) {
    os << " a:" << apple.x << "," << apple.y << std::endl;
  }

  os << "S1" << std::endl;
  os << *game.snake_one;
  os << "S2" << std::endl;
  os << *game.snake_two;

  return os;
}

std::istream &operator>>(std::istream &is, Game &game) {
  std::string line;
  std::getline(is, line);

  std::istringstream ss(line);

  // Read game over status, time, move time, and number of apples
  while (std::getline(ss, line, ',')) {
    if (line.substr(0, 2) == "GO") {
      game.is_game_over = std::stoi(line.substr(3));
    } else if (line.substr(0, 1) == "T") {
      game.time = std::stoi(line.substr(2));
    } else if (line.substr(0, 2) == "MT") {
      game.move_time = std::stoi(line.substr(3));
    } else if (line.substr(0, 2) == "LA") {
      const int num_apples = std::stoi(line.substr(3));

      std::getline(is, line); // Skip "A" line

      game.apples.clear();
      for (int i = 0; i < num_apples; ++i) {
        std::getline(is, line);
        std::istringstream apple_ss(line.substr(3));
        int x, y;
        char comma;
        apple_ss >> x >> comma >> y;
        game.apples.emplace_back(x, y);
      }
    }
  }

  game.createSnakes();

  // Read snakes
  std::getline(is, line); // Skip "S1" line
  is >> *game.snake_one;
  std::getline(is, line); // Skip "S2" line
  is >> *game.snake_two;

  return is;
}

void Game::updateSnake(const Direction new_direction, const bool is_two) {
  const auto snake = is_two ? snake_two : snake_one;

  // Check if snake ate an apple
}

void Game::updateGameOver() {
  // self-collision
  if (snake_one->hasCollidedWithItself()) {
    is_game_over = true;
  }

  // wall collision
}

void Game::updateMoveTime() { move_time = BASE_MOVE_TIME; /* - (how much we want to increase the speed);*/ }

void Game::spawnApples() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution x_dist(0, (viewport_size.x - 1) / CELL_SIZE);
  std::uniform_int_distribution y_dist(0, (viewport_size.y - 1) / CELL_SIZE);
}