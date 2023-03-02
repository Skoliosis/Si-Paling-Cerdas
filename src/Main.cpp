#include <iostream>
#include <raylib.h>

#include "Utils/Utils.hpp"
#include "Scene/Scene.hpp"
#include "Graphics/TextureManager.hpp"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800
#define TEAM_WATERMARK_NAME "Made by Skoliosis"

int main()
{
    TraceLog(LOG_INFO, "Si Paling Cerdas");
    TraceLog(LOG_INFO, "Made by Skoliosis Team");
    TraceLog(LOG_INFO, "- Ricky Jiunardi (2540133436) (Leader)");
    TraceLog(LOG_INFO, "- Catherine Kwee (2540124841)");
    TraceLog(LOG_INFO, "- Marco Anthonio Samkakai (2501960442)");
    TraceLog(LOG_INFO, "- Devani Marcellina Calista (2540124570)");
    TraceLog(LOG_INFO, "- Oey, Kevin Andrian Santoso (2540131084)");
    printf("\n");

#ifdef _DEBUG
    SetTraceLogLevel(LOG_DEBUG);
#else
    SetTraceLogLevel(LOG_NONE);
#endif

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Si Paling Cerdas");
    SetTargetFPS(60);

    GetCurrentScene()->Init();
    GetTextureManager()->Init();

    int teamNameFontSize = 15;
    int teamNamePadding = 10;
    int teamNameTextLength = MeasureText(TEAM_WATERMARK_NAME, teamNameFontSize);
    int teamNamePositionX = WINDOW_WIDTH - teamNameTextLength - teamNamePadding;
    int teamNamePositionY = WINDOW_HEIGHT - teamNameFontSize - teamNamePadding;

    while (!WindowShouldClose())
    {
        GetCurrentScene()->Update();

        BeginDrawing();

#ifdef _DEBUG
        DrawFPS(0, 0);
#endif

        ClearBackground(BLACK);
        GetCurrentScene()->Draw();
        DrawText(TEAM_WATERMARK_NAME, teamNamePositionX, teamNamePositionY, teamNameFontSize, WHITE);
        EndDrawing();
    }

    GetTextureManager()->Destroy();
    CloseWindow();
}
