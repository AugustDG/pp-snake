#pragma once

#include "raylib.h"

struct Vector2Int {
  int x;
  int y;

  Vector2Int() : Vector2Int(0, 0) {}
  Vector2Int(const int x, const int y) : x(x), y(y) {}

  explicit operator Vector2() const { return {static_cast<float>(x), static_cast<float>(y)}; }
  bool operator==(const Vector2Int &other) const { return x == other.x && y == other.y; }
};