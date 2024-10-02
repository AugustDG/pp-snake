#pragma once

#include "direction.h"
#include "vector_int.h"
#include <cstdint>
#include <memory>
#include <vector>

class Snake {
public:
  Snake();
  Snake(uint32_t length, const Vector2Int& position, Direction direction, const Color& body_color, const Color& tail_color);

  Color body_color;
  Color tail_color;

  uint32_t length;
  Vector2Int position;

  Direction direction;

  void render();

  void turn(Direction new_direction);
  void move();
  void grow();

  bool hasCollidedWithItself() const;
  bool hasCollidedWithSnake(const std::shared_ptr<Snake> &other) const;

  void reset(uint32_t length, const Vector2Int &position, Direction direction);

private:
  std::vector<Vector2Int> body;
};
