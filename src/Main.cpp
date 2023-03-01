#include <iostream>
#include <raylib.h>

#include "Utils/Utils.hpp"
#include "Graphics/TextureManager.hpp"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define APP_NAME "Si Paling Cerdas"

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Si Paling Cerdas");
    SetTargetFPS(60);

    GetTextureManager()->Init();

    auto length = MeasureText(APP_NAME, 20);
    std::string longText =
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Mauris et leo vel urna suscipit varius. Pellentesque "
        "habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Nulla facilisi. Duis id augue "
        "auctor, venenatis magna in, convallis eros. Praesent scelerisque eros ac purus pharetra imperdiet. Vivamus sed "
        "urna lacus. Nulla facilisi. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia "
        "Curae; Suspendisse eget magna non mauris ornare dictum id id mauris. Donec id enim vel lacus ornare vehicula. "
        "Etiam vel odio nec arcu pellentesque ultricies vitae eget velit. Nullam et ex vel sapien tempor scelerisque vel "
        "vel magna. Duis convallis tincidunt interdum. Pellentesque in lacus blandit, rutrum ipsum vel, vestibulum elit. "
        "Sed eget turpis enim. Aliquam at ipsum ullamcorper, hendrerit magna non, euismod arcu.";

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText(TextFormat("FPS: %d", GetFPS()), 0, 0, 30, GREEN);
        DrawText(APP_NAME, WINDOW_WIDTH / 2 - length / 2, WINDOW_HEIGHT / 2, 20, WHITE);
        EndDrawing();
    }

    GetTextureManager()->Destroy();
    CloseWindow();
}
