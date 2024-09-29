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

  uint64_t score;
  bool is_game_over = false;

  const Vector2Int map_size;

  explicit Game(const uint32_t base_move_time, const Vector2Int initial_map_size)
      : time(0), move_time(base_move_time), score(0), map_size(initial_map_size) {}

  void addSnake(const std::shared_ptr<Snake> &snake);

  void update();
  void render() const;

private:
  std::shared_ptr<Snake> snake;
  std::vector<Vector2Int> apples;

  void updateMoveTime();
  void updateSnake(Direction new_direction);
  void updateGameOver();

  void spawnApples();
};