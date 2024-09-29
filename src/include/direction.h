#pragma once

#include "vector_int.h"

enum Direction { NONE = 0b100, UP = 0b00, DOWN = 0b01, LEFT = 0b10, RIGHT = 0b11 };

static Direction relativeDirection(const Vector2Int& a, const Vector2Int& b) {
  if (a.x == b.x) {
    if (a.y < b.y) {
      return DOWN;
    }

    return UP;
  }

  if (a.x < b.x) {
    return RIGHT;
  }

  return LEFT;
}