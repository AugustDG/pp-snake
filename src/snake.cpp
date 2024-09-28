#include "snake.h"


Snake::Snake(uint32_t length, Vector2 position, Direction direction) : length(length), position(position), direction(direction) {
    body = std::vector<Vector2>(length, position);
};

void Snake::move(Direction dir) {
    for (uint32_t i = length - 1; i > 0; i--) {
        body[i] = body[i - 1];
    }

    switch (dir) {
        case Direction::UP:
            position.y -= 1;
            break;
        case Direction::DOWN:
            position.y += 1;
            break;
        case Direction::LEFT:
            position.x -= 1;
            break;
        case Direction::RIGHT:
            position.x += 1;
            break;
    }

    body[0] = position;
}

void Snake::grow() {
    length++;

    body.push_back(body.back());
}

void Snake::render() {
    for (const auto &part : body) {
        DrawRectangle((int)part.x * 20, (int)part.y * 20, 20, 20, RED);
    }
}
