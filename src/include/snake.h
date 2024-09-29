#pragma once

#include "direction.h"
#include "vector_int.h"
#include <cstdint>
#include <vector>

class Snake {
public:
  Snake();
  Snake(uint32_t length, Vector2Int position, Direction direction);

  uint32_t length;
  Vector2Int position;

  Direction direction;

  void render();

  void turn(Direction new_direction);
  void move();
  void grow();

  bool hasCollided();
  void reset(uint32_t length, const Vector2Int &position, Direction direction);

private:
  std::vector<Vector2Int> body;
};
