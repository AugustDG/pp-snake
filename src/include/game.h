#pragma once
#include "snake.h"
#include "vector_int.h"
#include <cstdint>
#include <memory>
#include <vector>

class Game {
public:
  uint64_t time;
  uint32_t move_time;

  bool is_game_over = false;

  const Vector2Int viewport_size;

  explicit Game(const uint32_t base_move_time, const Vector2Int initial_map_size)
      : time(0), move_time(base_move_time), viewport_size(initial_map_size) {}

  void init();

  void update();
  void render() const;

  uint64_t getScore(bool is_two) const;

private:
  std::shared_ptr<Snake> snake_one;
  std::shared_ptr<Snake> snake_two;
  std::vector<Vector2Int> apples;

  void updateMoveTime();
  void updateSnake(Direction new_direction, bool is_two);
  void updateGameOver();

  void spawnApples();
  bool hasSnakeEatenApple(const std::shared_ptr<Snake>& snake);
};