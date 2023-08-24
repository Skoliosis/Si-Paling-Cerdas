#include "MatchMenuScene.hpp"
#include "Network/ENetClient.hpp"
#include "HomeMenuScene.hpp"

static void DrawOptionBackground(int index, int x, int y)
{
    if (GetState()->AnswerReady && GetState()->CorrectAnswerIndex == index)
    {
        DrawTexture(GetTextureManager()->Get("Textures/CorrectAnswerOptionBackground.png"), x, y, WHITE);
    }
    else if (GetState()->AnswerReady && GetState()->AnswerIndex == index)
    {
        DrawTexture(GetTextureManager()->Get("Textures/WrongAnswerOptionBackground.png"), x, y, WHITE);
    }
    else
    {
        Utils::DrawHoverButton("Textures/AnswerOptionBackground.png", "Textures/AnswerOptionBackgroundHovered.png",
                               {(float)x, (float)y, 310, 99});
    }
}

void MatchMenuScene::Draw()
{
    DrawTexture(GetTextureManager()->Get("Textures/MatchMenuBackground.png"), 0, 0, WHITE);
    DrawTexture(GetTextureManager()->Get("Textures/VSLogo.png"), 636, 56, WHITE);
    DrawTexture(GetTextureManager()->Get("Textures/ProfileHover.png"), 181, 61, WHITE);
    DrawTexture(GetTextureManager()->Get("Textures/ProfileHoverReversed.png"), 830, 61, WHITE);
    DrawTexture(GetTextureManager()->Get("Textures/RoundProfilePicture.png"), 181, 53, WHITE);
    DrawTexture(GetTextureManager()->Get("Textures/EnemyRoundProfilePicture.png"), 1124, 53, WHITE);

    auto name = GetState()->Name + " (" + std::to_string(GetState()->Points) + ")";
    auto enemyName = "(" + std::to_string(GetState()->EnemyPoints) + ") " + GetState()->EnemyName;
    int length = MeasureText(enemyName.data(), 32);

    DrawText(name.data(), 315, 98, 32, BLACK);
    DrawText(enemyName.data(), 1110 - length, 98, 32, BLACK);

    if (GetState()->Winner.empty())
    {
        int seconds =
            15 - (int)std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - GetState()->QuestionTimer).count();

        /// SLIDING ANIMATION
        // int seconds =
        //     15000 - std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() -
        //     GetState()->QuestionTimer).count();
        // int width = (int)((1126.0f - 153.0f) * ((float)seconds / 15000.0f));

        // DrawRectangle(151, 260, 1130, 56, BLACK);
        // DrawRectangle(153, 262, 153 + width, 52, YELLOW);

        Utils::DrawTextWithNewLinesCentered(std::to_string(seconds), 151, 262, 40, 1130, BLACK);
        Utils::DrawTextWithNewLinesCentered(GetState()->Question, 151, 374, 40, 1130, BLACK);

        DrawOptionBackground(0, 295, 563);
        DrawOptionBackground(1, 827, 563);
        DrawOptionBackground(2, 295, 731);
        DrawOptionBackground(3, 827, 731);

        Utils::DrawTextCentered(GetState()->AnswerOption1, 295, 563, 310, 99, 30, BLACK);
        Utils::DrawTextCentered(GetState()->AnswerOption2, 827, 563, 310, 99, 30, BLACK);
        Utils::DrawTextCentered(GetState()->AnswerOption3, 295, 731, 310, 99, 30, BLACK);
        Utils::DrawTextCentered(GetState()->AnswerOption4, 827, 731, 310, 99, 30, BLACK);
    }
    else
    {
        DrawTexture(GetTextureManager()->Get("Textures/Trophy.png"), 900, 576, WHITE);

        if (GetState()->Winner != "-")
        {
            DrawTexture(GetTextureManager()->Get("Textures/ProfileHover.png"), 448, 572, WHITE);
            DrawText(GetState()->Winner.data(), 584, 597, 32, BLACK);

            if (GetState()->Winner == GetState()->EnemyName)
            {
                DrawTexture(GetTextureManager()->Get("Textures/EnemyRoundProfilePicture.png"), 448, 564, WHITE);
                Utils::DrawTextCentered("Point: " + std::to_string(GetState()->EnemyPoints), 700, 40, BLACK);
            }
            else
            {
                DrawTexture(GetTextureManager()->Get("Textures/RoundProfilePicture.png"), 448, 564, WHITE);
                Utils::DrawTextCentered("Point: " + std::to_string(GetState()->Points), 700, 40, BLACK);
            }

            Utils::DrawTextCentered("Winner!!!", 452, 40, BLACK);
        }
        else
        {
            DrawText("DRAW!", 584, 597, 32, BLACK);
            Utils::DrawTextCentered("Point: " + std::to_string(GetState()->Points), 700, 40, BLACK);
        }

        int seconds =
            5 - (int)std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - GetState()->GameEndedSince).count();
        Utils::DrawTextCentered("Going back to home menu in " + std::to_string(seconds) + " seconds..", 800, 35, BLACK);
    }
}

void MatchMenuScene::Update()
{
    if (!GetState()->Winner.empty())
    {
        int seconds =
            (int)std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - GetState()->GameEndedSince).count();
        if (seconds >= 5)
        {
            GetState()->Winner = "";
            GetState()->AlreadyAnswered = false;
            GetState()->HomeMenuSceneState = HomeMenuSceneState::Idle;
            SetNextScene<HomeMenuScene>();
        }
    }
    else
    {
        if (GetState()->AlreadyAnswered)
        {
            return;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            auto mousePosition = GetMousePosition();
            int index;

            if (CheckCollisionPointRec(mousePosition, {295, 563, 310, 99}))
            {
                index = 0;
            }
            else if (CheckCollisionPointRec(mousePosition, {827, 563, 310, 99}))
            {
                index = 1;
            }
            else if (CheckCollisionPointRec(mousePosition, {295, 731, 310, 99}))
            {
                index = 2;
            }
            else if (CheckCollisionPointRec(mousePosition, {827, 731, 310, 99}))
            {
                index = 3;
            }
            else
            {
                return;
            }

            GetState()->AnswerIndex = index;
            GetState()->AlreadyAnswered = true;
            GetENetClient()->SendAnswer(index);
        }
    }
}
