#include "raylib.h"

int main() {
    InitWindow(1280, 720, "Discovery One");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Discovery One", 540, 340, 30, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
