#include <raylib.h>

int main() {
    const int screenWidth = 1600;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "IEEE C++ Workshop!");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        // Where our game logic will go 
        DrawRectangle(350, 250, 100, 100, RED);
        DrawText("Hello, Snake!", 10, 10, 20, BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}