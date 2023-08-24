#include <iostream>
#include <raylib.h>

#include "Utils/Utils.hpp"
#include "Scene/Scene.hpp"
#include "Scene/BurgerMenu.hpp"
#include "Scene/HomeMenuScene.hpp"
#include "Graphics/TextureManager.hpp"
#include "Network/ENetClient.hpp"

#define WINDOW_WIDTH 1440
#define WINDOW_HEIGHT 1024

int main()
{
    GetState();
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

    if (!GetENetClient()->Init())
    {
        TraceLog(LOG_ERROR, "Failed to initialize ENet client");
        return EXIT_FAILURE;
    }

    if (!GetENetClient()->Connect())
    {
        TraceLog(LOG_WARNING, "ENet host connect fails");
        return EXIT_FAILURE;
    }

    while (!WindowShouldClose() && GetState()->Running)
    {
        GetCurrentScene()->Update();
        BeginDrawing();
        ClearBackground(BLACK);
        GetCurrentScene()->Draw();
        EndDrawing();
        GetENetClient()->Poll();

        auto homeMenuScene = dynamic_cast<HomeMenuScene *>(g_Scene.get());
        auto isConnected = GetENetClient()->IsConnected();
        if (!homeMenuScene && !isConnected)
        {
            SetNextScene<HomeMenuScene>();
        }

        if (g_NextScene)
        {
            auto nextBurgerMenu = dynamic_cast<BurgerMenuScene *>(g_NextScene.get());
            auto previousBurgerMenu = dynamic_cast<BurgerMenuScene *>(g_Scene.get());
            auto isBurgerBar = previousBurgerMenu != nullptr && nextBurgerMenu != nullptr;

            g_Scene = std::move(g_NextScene);
            g_NextScene = nullptr;

            if (isBurgerBar && (homeMenuScene || isConnected))
            {
                nextBurgerMenu = dynamic_cast<BurgerMenuScene *>(g_Scene.get());
                nextBurgerMenu->X = 0;
                nextBurgerMenu->AnimationState = AnimationState::Closing;
            }

            GetState()->FriendFetched = false;
            GetState()->LeaderboardFetched = false;
            GetState()->FriendRequestFetched = false;
        }
    }

    GetTextureManager()->Destroy();
    GetENetClient()->Destroy();
    CloseWindow();
}
