#include "game.h"

#include "raylib.h"
#include <random>
#include <saving.h>
#include <settings.h>
#include <sstream>

void Game::createSnakes() {
  this->snake_one = std::make_shared<Snake>(
      STARTING_SNAKE_SIZE, Vector2Int{viewport_size.x / 4 / CELL_SIZE, viewport_size.y / 2 / CELL_SIZE}, RIGHT, GREEN,
      DARKGREEN);
  this->snake_two = std::make_shared<Snake>(
      STARTING_SNAKE_SIZE, Vector2Int{3 * viewport_size.x / 4 / CELL_SIZE, viewport_size.y / 2 / CELL_SIZE}, LEFT, BLUE,
      DARKBLUE);
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
    snake_one->reset(3, Vector2Int{viewport_size.x / 4 / CELL_SIZE, viewport_size.y / 2 / CELL_SIZE}, RIGHT);
    snake_two->reset(3, Vector2Int{3 * viewport_size.x / 4 / CELL_SIZE, viewport_size.y / 2 / CELL_SIZE}, LEFT);
    apples.clear();

    time = 0;
    is_game_over = false;

    updateMoveTime();

    GameSaver::getInstance().saveGame(SAVE_GAME_FILENAME, *this);
  }

  if (time % move_time == 0) {
    updateSnake(new_direction_one, false);
    updateSnake(new_direction_two, true);
    updateGameOver();

    new_direction_one = NONE;
    new_direction_two = NONE;

    GameSaver::getInstance().saveGame(SAVE_GAME_FILENAME, *this);
  }

  // apples spawn faster as the game progresses
  if (time % (move_time * 15) == 0) {
    spawnApples();

    GameSaver::getInstance().saveGame(SAVE_GAME_FILENAME, *this);
  }

  updateMoveTime();

  time++;
}

void Game::render() const {
  std::stringstream ss;

  // Game Over
  if (is_game_over) {
    ss << "P1: " << getScore(false) << ", P2: " << getScore(true);

    const std::string game_over_text = "Game Over";
    const std::string score_text = ss.str();
    const std::string restart_text = "Press Enter to restart";

    int centering_offset = MeasureText(game_over_text.c_str(), 20) / 2;

    DrawText(game_over_text.c_str(), viewport_size.x / 2 - centering_offset, viewport_size.y / 2 - 20, 20, BLACK);
    centering_offset = MeasureText(score_text.c_str(), 20) / 2;
    DrawText(score_text.c_str(), viewport_size.x / 2 - centering_offset, viewport_size.y / 2, 20, BLACK);
    centering_offset = MeasureText(restart_text.c_str(), 20) / 2;
    DrawText(restart_text.c_str(), viewport_size.x / 2 - centering_offset, viewport_size.y / 2 + 20, 20, BLACK);

    return;
  }

  // Map Grid
  for (int i = 0; i <= viewport_size.x / CELL_SIZE; i++) {
    DrawLine(i * CELL_SIZE, 0, i * CELL_SIZE, viewport_size.y, LIGHTGRAY);
  }

  for (int i = 0; i <= viewport_size.y / CELL_SIZE; i++) {
    DrawLine(0, i * CELL_SIZE, viewport_size.x, i * CELL_SIZE, LIGHTGRAY);
  }

  DrawRectangleLines(0, 0, viewport_size.x, viewport_size.y, BLACK);

  // Apples
  for (const auto [x, y] : apples) {
    DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, RED);
  }

  // Score
  ss.str("");
  ss << "P1: " << getScore(false);
  DrawText(ss.str().c_str(), 10, viewport_size.y - 40, 30, BLACK);

  ss.str("");
  ss << "P2: " << getScore(true);

  // Draws P2's score "justified" to the right
  const std::string score_text = ss.str();
  const int score_text_size = MeasureText(score_text.c_str(), 30);
  DrawText(ss.str().c_str(), viewport_size.x - score_text_size - 10, viewport_size.y - 40, 30, BLACK);

  // Snakes
  snake_one->render();
  snake_two->render();
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

  snake->turn(new_direction);
  snake->move();

  // Check if snake ate an apple
  auto it = apples.begin();
  while (it != apples.end()) {
    if (snake->position == *it) {
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
  if (snake_one->position.x < 0 || snake_one->position.x >= viewport_size.x / CELL_SIZE || snake_one->position.y < 0 ||
      snake_one->position.y >= viewport_size.y / CELL_SIZE || snake_two->position.x < 0 ||
      snake_two->position.x >= viewport_size.x / CELL_SIZE || snake_two->position.y < 0 ||
      snake_two->position.y >= viewport_size.y / CELL_SIZE) {
    is_game_over = true;
  }
}

void Game::updateMoveTime() { move_time = BASE_MOVE_TIME - (snake_one->length + snake_two->length) / 4 - time / 600; }

void Game::spawnApples() {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution x_dist(0, (viewport_size.x - 1) / CELL_SIZE);
  std::uniform_int_distribution y_dist(0, (viewport_size.y - 1) / CELL_SIZE);

  auto apple_pos = Vector2Int(x_dist(rng), y_dist(rng));

  apples.emplace_back(apple_pos);
}