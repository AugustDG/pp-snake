#pragma once

#include <cstdint>
#include <vector>
#include "raylib.h"

enum class Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Snake {
public:
    Snake(uint32_t length = 1, Vector2 position = Vector2(), Direction direction = Direction::UP);

    uint32_t length;
    Vector2 position;

    Direction direction;

    void render();

    void move(Direction dir = Direction::NONE);
    void grow();

private:
    std::vector<Vector2> body;
};
