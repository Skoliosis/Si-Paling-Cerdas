#include "BurgerMenu.hpp"
#include "HomeMenuScene.hpp"
#include "FriendListMenuScene.hpp"
#include "LeaderboardMenuScene.hpp"
#include "Network/ENetClient.hpp"
#include "Utils/FileDialogUtils.hpp"

const int AnimationVelocity = 1500;

static void DrawButton(BurgerMenuType type, BurgerMenuType expectedType, const std::string &texture, int x, int y)
{
    if (type == expectedType)
    {
        DrawTexture(GetTextureManager()->Get("Textures/" + texture + "Chosen.png"), x, y, WHITE);
    }
    else
    {
        Utils::DrawHoverButton("Textures/" + texture + ".png", "Textures/" + texture + "Hovered.png",
                               {(float)x, (float)y, 418, 102});
    }
}

void BurgerMenuScene::Draw()
{
    if (GetENetClient()->State != ClientState::Ready)
    {
        return;
    }

    Utils::DrawHoverButton("Textures/ProfileHover.png", "Textures/ProfileHover.png",
                           {X + 418.0f + 953.0f, 53.0f, 420.0f, 111.0f});

    DrawTexture(GetTextureManager()->Get("Textures/RoundProfilePicture.png"), X + 418 + 950, 45, WHITE);
    DrawText(GetState()->Name.data(), X + 418 + 1100, 98, 32, BLACK);

    DrawTexture(GetTextureManager()->Get("Textures/BurgerBarBackground.png"), X, 0, WHITE);

    Utils::DrawHoverButton("Textures/Bar.png", "Textures/HoveredBar.png", {X + 31.0f + 418.0f, 78, 89, 78});
    Utils::DrawHoverButton("Textures/BurgerBarExit.png", "Textures/BurgerBarExitHovered.png", {X + 318.0f, 18, 99, 98});

    DrawButton(Type, BurgerMenuType::Home, "BurgerBarHome", X, 117);
    DrawButton(Type, BurgerMenuType::Leaderboard, "BurgerBarLeaderboard", X, 219);
    DrawButton(Type, BurgerMenuType::FriendList, "BurgerBarFriendList", X, 328);
    DrawButton(Type, BurgerMenuType::Option, "BurgerBarOption", X, 431);    
}

void BurgerMenuScene::Update()
{
    if (GetENetClient()->State != ClientState::Ready)
    {
        return;
    }

    switch (AnimationState)
    {
        case AnimationState::Running:
        {
            if (X < 0)
            {
                X += (int)((float)AnimationVelocity * GetFrameTime());

                if (X >= 0)
                {
                    X = 0;
                    AnimationState = AnimationState::Done;
                }
            }

            break;
        }

        case AnimationState::Done:
        {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                if (CheckCollisionPointRec(GetMousePosition(), {X + 318.0f, 18.0f, 99.0f, 98.0f}) ||
                    CheckCollisionPointRec(GetMousePosition(), {X + 31.0f + 418.0f, 78.0f, 89.0f, 78.0f}))
                {
                    AnimationState = AnimationState::Closing;
                }

                if (Type !=
                    BurgerMenuType::Home && CheckCollisionPointRec(GetMousePosition(), {(float)X, 117.0f, 418.0f, 102.0f}))
                {
                    SetNextScene<HomeMenuScene>();
                    AnimationState = AnimationState::Closing;
                }
                else if (Type != BurgerMenuType::Leaderboard &&
                         CheckCollisionPointRec(GetMousePosition(), {(float)X, 219.0f, 418.0f, 102.0f}))
                {
                    SetNextScene<LeaderboardMenuScene>();
                    AnimationState = AnimationState::Closing;
                }
                else if (Type != BurgerMenuType::FriendList &&
                         CheckCollisionPointRec(GetMousePosition(), {(float)X, 328.0f, 418.0f, 102.0f}))
                {
                    SetNextScene<FriendListMenuScene>();
                    AnimationState = AnimationState::Closing;
                }
                else if (Type != BurgerMenuType::Option &&
                         CheckCollisionPointRec(GetMousePosition(), {(float)X, 431.0f, 418.0f, 102.0f}))
                {
                    AnimationState = AnimationState::Closing;
                }
            }

            break;
        }

        case AnimationState::Idle:
        {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                if (CheckCollisionPointRec(GetMousePosition(), {X + 31.0f + 418.0f, 78, 89, 78}))
                {
                    AnimationState = AnimationState::Running;
                }
            }

            break;
        }
        
        case AnimationState::Closing:
        {
            if (X > -418)
            {
                X -= (int)((float)AnimationVelocity * GetFrameTime());

                if (X <= -418)
                {
                    X = -418;
                    AnimationState = AnimationState::Idle;
                }
            }

            break;
        }
    }
}
