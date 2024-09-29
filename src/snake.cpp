#include "snake.h"

#include "raylib.h"

#include <settings.h>

Snake::Snake() : Snake(1, Vector2Int{10, 10}, RIGHT) {}

Snake::Snake(const uint32_t length, const Vector2Int position, const Direction direction)
    : length(length), position(position), direction(direction) {
  body = std::vector(length, position);
};

void Snake::turn(Direction new_direction) {
  if (new_direction == NONE)
    return;

  // avoids going back on itself
  if (!((new_direction ^ direction) >> 1)) {
    return;
  }

  direction = new_direction;
}

void Snake::move() {
  switch (direction) {
  case UP:
    position.y -= 1;
    break;
  case DOWN:
    position.y += 1;
    break;
  case LEFT:
    position.x -= 1;
    break;
  case RIGHT:
    position.x += 1;
    break;
  default:
    break;
  }

  for (uint32_t i = length - 1; i > 0; i--) {
    body[i] = body[i - 1];
  }

  body[0] = position;
}

void Snake::grow() {
  length++;

  body.emplace_back(body.back());
}
bool Snake::hasCollided() {
  auto it = body.begin();
  while (it != body.end()) {
    if (it != body.begin() && *it == body.front()) {
      return true;
    }

    ++it;
  }

  return false;
}

void Snake::reset(uint32_t length, const Vector2Int& position, Direction direction) {
  this->length = length;
  this->position = position;
  this->direction = direction;
  body = std::vector(length, position);
}

void Snake::render() {
  auto it = body.begin();
  while (it != body.end()) {
    const auto cell_x = it->x * CELL_SIZE;
    const auto cell_y = it->y * CELL_SIZE;

    // head
    if (it == body.begin()) {
      DrawRectangle(cell_x, cell_y, CELL_SIZE, CELL_SIZE, GREEN);

      auto eyes_l_x = cell_x;
      auto eyes_l_y = cell_y;
      auto eyes_r_x = cell_x;
      auto eyes_r_y = cell_y;

      switch (direction) {
      case UP:
        eyes_l_x += CELL_SIZE;
        break;
      case DOWN:
        eyes_r_x += CELL_SIZE;
        eyes_r_y += CELL_SIZE;
        eyes_l_y += CELL_SIZE;
        break;
      case LEFT:
        eyes_l_y += CELL_SIZE;
        break;
      case RIGHT:
        eyes_l_x += CELL_SIZE;
        eyes_r_x += CELL_SIZE;
        eyes_r_y += CELL_SIZE;
        break;
      default:
        break;
      }

      DrawCircle(eyes_l_x, eyes_l_y, static_cast<float>(CELL_SIZE) / 4, BLACK);
      DrawCircle(eyes_r_x, eyes_r_y, static_cast<float>(CELL_SIZE) / 4, BLACK);
    }
    // tail
    else if (it == body.end() - 1) {
      Vector2Int v1, v2, v3;

      // we calculate the direction of the tail relative to the before last cell
      switch (relativeDirection(*it, *(it - 1))) {
      case UP:
        v1 = Vector2Int{cell_x, cell_y};
        v2 = Vector2Int{cell_x + CELL_SIZE / 2, cell_y + CELL_SIZE};
        v3 = Vector2Int{cell_x + CELL_SIZE, cell_y};
        break;
      case DOWN:
        v1 = Vector2Int{cell_x, cell_y + CELL_SIZE};
        v2 = Vector2Int{cell_x + CELL_SIZE, cell_y + CELL_SIZE};
        v3 = Vector2Int{cell_x + CELL_SIZE / 2, cell_y};
        break;
      case LEFT:
        v1 = Vector2Int{cell_x, cell_y};
        v2 = Vector2Int{cell_x, cell_y + CELL_SIZE};
        v3 = Vector2Int{cell_x + CELL_SIZE, cell_y + CELL_SIZE / 2};
        break;
      case RIGHT:
        v1 = Vector2Int{cell_x + CELL_SIZE, cell_y};
        v2 = Vector2Int{cell_x, cell_y + CELL_SIZE / 2};
        v3 = Vector2Int{cell_x + CELL_SIZE, cell_y + CELL_SIZE};
        break;
      default:;
      }

      DrawTriangle(static_cast<Vector2>(v1), static_cast<Vector2>(v2), static_cast<Vector2>(v3), DARKGREEN);
    } else if (it == body.end() - 2) {
      // we calculate the direction of the tail relative to the before last cell, to correctly place the gradient
      if (const Direction d1 = relativeDirection(*it, *(it + 1)); d1 > DOWN)
        DrawRectangleGradientH(cell_x, cell_y, CELL_SIZE, CELL_SIZE, d1 == LEFT ? DARKGREEN : GREEN,
                               d1 == LEFT ? GREEN : DARKGREEN);
      else
        DrawRectangleGradientV(cell_x, cell_y, CELL_SIZE, CELL_SIZE, d1 == UP ? DARKGREEN : GREEN,
                               d1 == UP ? GREEN : DARKGREEN);

      // how would you make it so that the gradient is always in the direction of the tail & the body?
    }
    // body
    else {
      DrawRectangle(cell_x, cell_y, CELL_SIZE, CELL_SIZE, GREEN);
    }

    ++it;
  }
}
