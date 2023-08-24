#include "LeaderboardMenuScene.hpp"
#include "Network/ENetClient.hpp"

void LeaderboardMenuScene::Init()
{
    Scene::Init();
}

void LeaderboardMenuScene::Draw()
{
    Scene::Draw();

    auto x = X + 418;

    DrawTexture(GetTextureManager()->Get("Textures/LeaderboardText.png"), x + 477, 197, WHITE);
    DrawTexture(GetTextureManager()->Get("Textures/LeaderboardLine.png"), x + -2, 358, WHITE);
    DrawTexture(GetTextureManager()->Get("Textures/LeaderboardInfo.png"), x + 189, 409, WHITE);

    if (!GetState()->LeaderboardFetched)
    {
        const char *text = "Fetching leaderboard..";
        int length = MeasureText(text, 30);

        DrawText(text, x + GetScreenWidth() / 2 - length / 2, 1024 / 2, 30, BLACK);
    }
    else
    {
        int max = std::min(StartIndex + 3, (int)GetState()->Leaderboard.size());

        for (int i = StartIndex; i < max; ++i)
        {
            int y = 181 * (i - StartIndex);
            auto &info = GetState()->Leaderboard[i];

            auto roundProfilePicturePath = "Textures/LeaderboardRoundProfilePicture" + std::to_string(i) + ".png";

            DrawTexture(GetTextureManager()->Get("Textures/LeaderboardBackground.png"), x + 235, 485 + y, WHITE);
            DrawTexture(GetTextureManager()->Get(roundProfilePicturePath), x + 171, 469 + y, WHITE);

            DrawText(std::to_string(i + 1).data(), x + 338, 511 + y, 30, BLACK);
            DrawText(info.Name.data(), x + 477, 511 + y, 30, BLACK);
            DrawText(std::to_string(info.Win).data(), x + 885, 511 + y, 30, BLACK);
            DrawText(std::to_string(info.Lose).data(), x + 1013, 511 + y, 30, BLACK);
            DrawText(std::to_string(info.Rating).data(), x + 1209, 511 + y, 30, BLACK);
        }
    }

    BurgerMenuScene::Draw();
}

void LeaderboardMenuScene::Update()
{
    BurgerMenuScene::Update();

    if (!GetState()->LeaderboardFetched)
    {
        GetState()->LeaderboardFetched = true;
        GetENetClient()->FetchLeaderboard();
    }
    else if (GetState()->Leaderboard.size() > 3)
    {
        if (GetMouseWheelMove() == -1.0f)
        {
            StartIndex += 1;

            if (StartIndex > GetState()->Leaderboard.size() - 3)
            {
                StartIndex = GetState()->Leaderboard.size() - 3;
            }
        }
        else if (GetMouseWheelMove() == 1.0f)
        {
            StartIndex -= 1;

            if (StartIndex < 0)
            {
                StartIndex = 0;
            }
        }
    }
}
