#include "snake.h"

#include "raylib.h"

#include <memory>
#include <ostream>
#include <settings.h>
#include <sstream>

Snake::Snake() : Snake(0, Vector2Int{10, 10}, RIGHT, GREEN, DARKGREEN) {}

Snake::Snake(const uint32_t length, const Vector2Int &position, const Direction direction, const Color &body_color,
             const Color &tail_color)
    : body_color(body_color), tail_color(tail_color), length(length), position(position), direction(direction) {
  body = std::vector(length, position);
}

void Snake::turn(const Direction new_direction) {
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

  const Vector2Int &last = body.back();

  body.emplace_back(last.x, last.y);
}

bool Snake::hasCollidedWithItself() const {
  auto it = body.begin();
  while (it != body.end()) {
    if (it != body.begin() && *it == body.front()) {
      return true;
    }

    ++it;
  }

  return false;
}

bool Snake::hasCollidedWithSnake(const std::shared_ptr<Snake> &other) const {
  auto it = other->body.begin();
  while (it != other->body.end()) {
    if (*it == body.front()) {
      return true;
    }

    ++it;
  }

  return false;
}

void Snake::reset(const uint32_t length, const Vector2Int &position, const Direction direction) {
  this->length = length;
  this->position = position;
  this->direction = direction;

  body.clear();
  body.resize(length, position);
}

std::ostream &operator<<(std::ostream &os, const Snake &snake) {
  os << "L:" << snake.length << ",D:" << snake.direction << std::endl;
  os << "B" << std::endl;

  for (auto &&cell : snake.body) {
    os << " b:" << cell.x << "," << cell.y << std::endl;
  }

  return os;
}

std::istream &operator>>(std::istream &is, Snake &snake) {
  std::string line;
  std::getline(is, line);

  std::istringstream ss(line);

  // Read length and direction
  while (std::getline(ss, line, ':')) {
    if (line == "L") {
      std::getline(ss, line, ',');
      snake.length = std::stoi(line);
    } else if (line == "D") {
      std::getline(ss, line, ',');
      snake.direction = static_cast<Direction>(std::stoi(line));
    }
  }

  // Read body positions
  std::getline(is, line); // Skip "B" line
  snake.body.clear();
  for (uint32_t i = 0; i < snake.length; ++i) {
    std::getline(is, line);
    ss = std::istringstream(line);

    // Skip " b:"
    ss.seekg(3);

    std::string token;
    std::getline(ss, token, ',');
    int x = std::stoi(token);

    std::getline(ss, token, ',');
    int y = std::stoi(token);

    snake.body.emplace_back(x, y);
  }

  snake.position = snake.body.front();

  return is;
}

void Snake::render() {
  auto it = body.begin();
  while (it != body.end()) {
    const auto cell_x = it->x * CELL_SIZE;
    const auto cell_y = it->y * CELL_SIZE;

    DrawRectangle(cell_x, cell_y, CELL_SIZE, CELL_SIZE, body_color);

    ++it;
  }
}
