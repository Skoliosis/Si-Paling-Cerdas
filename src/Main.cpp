#include <iostream>
#include <raylib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define APP_NAME "Si Paling Cerdas"

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Si Paling Cerdas");
    SetTargetFPS(60);

    auto length = MeasureText(APP_NAME, 20);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(TextFormat("FPS: %d", GetFPS()), 0, 0, 30, GREEN);
        DrawText(APP_NAME, WINDOW_WIDTH / 2 - length / 2, WINDOW_HEIGHT / 2, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
}
