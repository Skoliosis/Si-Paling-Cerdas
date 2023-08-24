#include "FriendListMenuScene.hpp"
#include "Network/ENetClient.hpp"

void FriendListMenuScene::Draw()
{
    Scene::Draw();

    auto x = X + 418;

    DrawTexture(GetTextureManager()->Get("Textures/FriendListText.png"), x + 110, 200, WHITE);

    Utils::DrawHoverButton("Textures/FriendAddLogo.png", "Textures/FriendAddLogoHovered.png", {x + 1168.0f, 226, 70, 70});
    Utils::DrawHoverButton("Textures/FriendRequestLogo.png", "Textures/FriendRequestLogoHovered.png", {x + 1260.0f, 226, 70, 70});

    if (!GetState()->FriendFetched)
    {
        const char *text = "Fetching friends..";
        int length = MeasureText(text, 30);

        DrawText(text, x + GetScreenWidth() / 2 - length / 2, 1024 / 2, 30, BLACK);
    }
    else if (GetState()->Friends.empty())
    {
        const char *text = "You don't have any friends!";
        int length = MeasureText(text, 30);

        DrawText(text, x + GetScreenWidth() / 2 - length / 2, 1024 / 2, 30, BLACK);
    }
    else
    {
        int max = std::min(StartIndex + 5, (int)GetState()->Friends.size());

        for (int i = StartIndex; i < max; ++i)
        {
            int y = 132 * (i - StartIndex);
            auto &info = GetState()->Friends[i];
            auto profilePicturePath = "Textures/FriendProfilePicture" + std::to_string(i) + ".png";

            if (info.Online)
            {
                DrawTexture(GetTextureManager()->Get("Textures/FriendBackground.png"), x + 110, 313 + y, WHITE);
                DrawTexture(GetTextureManager()->Get("Textures/ChallengeLogo.png"), x + 1200, 335 + y, WHITE);
            }
            else
            {
                DrawTexture(GetTextureManager()->Get("Textures/FriendBackgroundOffline.png"), x + 110, 313 + y, WHITE);
            }

            DrawTexture(GetTextureManager()->Get(profilePicturePath), x + 135, 324 + y, WHITE);
            DrawText(info.Name.data(), x + 252, 330 + y, 20, BLACK);

            int width = MeasureText(info.Name.data(), 30);
            if (info.Online)
            {
                DrawTexture(GetTextureManager()->Get("Textures/OnlineLogo.png"), x + 252 + width, 330 + y, WHITE);
            }
            else
            {
                DrawTexture(GetTextureManager()->Get("Textures/OfflineLogo.png"), x + 260 + width, 330 + y, WHITE);
            }
        }
    }

    if (State == FriendListMenuState::FriendRequest)
    {
        DrawTexture(GetTextureManager()->Get("Textures/FriendRequestBackground.png"), x + 425, 341, WHITE);
        Utils::DrawHoverButton("Textures/BurgerBarExit.png", "Textures/BurgerBarExitHovered.png", {x + 920.0f, 341, 99, 98});

        if (!GetState()->FriendRequestFetched)
        {
            Utils::DrawTextCentered("Fetching...", 667, 25, BLACK);
        }
        else if (GetState()->FriendRequests.empty())
        {
            Utils::DrawTextCentered("No Request.", 667, 25, BLACK);
        }
        else
        {
            int max = std::min(RequestStartIndex + 3, (int)GetState()->FriendRequests.size());

            for (int i = RequestStartIndex; i < max; ++i)
            {
                int y = 116 * (i - RequestStartIndex);
                auto &info = GetState()->FriendRequests[i];

                auto profilePicturePath = "Textures/FriendRequestProfilePicture" + std::to_string(i) + ".png";

                DrawTexture(GetTextureManager()->Get("Textures/FriendRequestOffer.png"), x + 461, 522 + y, WHITE);
                DrawTexture(GetTextureManager()->Get(profilePicturePath), x + 477, 533 + y, WHITE);
                DrawText(info.Name.data(), x + 550, 550 + y, 30, BLACK);
            }
        }
    }
    else if (State == FriendListMenuState::FriendAdd)
    {
        DrawTexture(GetTextureManager()->Get("Textures/FriendAddBackground.png"), x + 425, 338, WHITE);
        DrawTexture(GetTextureManager()->Get("Textures/NameSearch.png"), x + 474, 467, WHITE);
        DrawText(GetState()->NameInput.data(), 477, 483, 30, BLACK);

        Utils::DrawHoverButton("Textures/BurgerBarExit.png", "Textures/BurgerBarExitHovered.png", {x + 926.0f, 338, 99, 98});

        if (GetState()->SearchNameFetched)
        {
            if (GetState()->SearchNameFound)
            {
                DrawTexture(GetTextureManager()->Get("Textures/NameFoundBackground.png"), x + 466, 567, WHITE);
                DrawTexture(GetTextureManager()->Get("Textures/NameFoundProfilePicture.png"), x + 482, 578, WHITE);
                DrawText(GetState()->NameFound.data(), 555, 595, 30, BLACK);
            }
            else
            {
                Utils::DrawTextCentered("Not Found.", 595, 30, BLACK);
            }
        }

        if (Writing)
        {
            int width = MeasureText(GetState()->NameInput.data(), 30);
            DrawRectangle(480 + width, 480, 10, 40, InputRectColor);
        }
    }

    BurgerMenuScene::Draw();
}

void FriendListMenuScene::Update()
{
    Scene::Update();
    BurgerMenuScene::Update();

    if (!GetState()->FriendFetched)
    {
        GetState()->FriendFetched = true;
        GetENetClient()->FetchFriend();
    }
    else if (GetState()->Friends.size() > 5)
    {
        if (GetMouseWheelMove() == -1.0f)
        {
            StartIndex += 1;

            if (StartIndex > GetState()->Friends.size() - 5)
            {
                StartIndex = 5;
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

    if (AnimationState == AnimationState::Idle)
    {
        auto x = X + 418;
        auto mousePosition = GetMousePosition();

        if (State == FriendListMenuState::Idle)
        {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                if (CheckCollisionPointRec(mousePosition, { x + 1168.0f, 226, 70, 70 }))
                {
                    State = FriendListMenuState::FriendAdd;
                }
                else if (CheckCollisionPointRec(mousePosition, { x + 1260.0f, 226, 70, 70 }))
                {
                    State = FriendListMenuState::FriendRequest;
                }
            }
        }
        else if (State == FriendListMenuState::FriendRequest)
        {
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mousePosition, {x + 920.0f, 341, 99, 98}))
            {
                State = FriendListMenuState::Idle;
                GetState()->FriendRequestFetched = false;
            }
            else if (!GetState()->FriendRequestFetched)
            {
                GetState()->FriendRequestFetched = true;
                GetENetClient()->FetchFriendRequest();
            }
            else if (!GetState()->FriendRequests.empty() && GetState()->FriendRequests.size() > 3)
            {
                if (GetMouseWheelMove() == -1.0f)
                {
                    RequestStartIndex += 1;

                    if (RequestStartIndex > GetState()->FriendRequests.size() - 3)
                    {
                        RequestStartIndex = (int)GetState()->FriendRequests.size() - 3;
                    }
                }
                else if (GetMouseWheelMove() == 1.0f)
                {
                    RequestStartIndex -= 1;

                    if (RequestStartIndex < 0)
                    {
                        RequestStartIndex = 0;
                    }
                }
            }
        }
        else if (State == FriendListMenuState::FriendAdd)
        {
            if (Writing)
            {
                if (!InputRectColorReverse)
                {
                    InputRectColor.a -= 10;

                    if (InputRectColor.a < 10)
                    {
                        InputRectColor.a = 0;
                        InputRectColorReverse = true;
                    }
                }
                else
                {
                    InputRectColor.a += 10;

                    if (InputRectColor.a > 245)
                    {
                        InputRectColor.a = 255;
                        InputRectColorReverse = false;
                    }
                }
            }

            if (Writing || CheckCollisionPointRec(mousePosition, {x + 474.0f, 467, 430, 59}))
            {
                SetMouseCursor(MOUSE_CURSOR_IBEAM);
            }
            else if (CheckCollisionPointRec(mousePosition, {x + 907.0f, 476, 38, 42}) ||
                     CheckCollisionPointRec(mousePosition, {x + 916.0f, 582, 47, 40}))
            {
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            }
            else
            {
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }

            int keyPressed = GetKeyPressed();
            if (Writing && keyPressed)
            {
                if (!GetState()->NameInput.empty() && keyPressed == KEY_BACKSPACE)
                {
                    GetState()->NameInput.pop_back();
                }
                else
                {
                    char key = GetCharPressed();
                    if (std::isalnum(key) && GetState()->NameInput.size() < 12)
                    {
                        GetState()->NameInput += key;
                    }
                }
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                if (!Writing && CheckCollisionPointRec(mousePosition, {x + 926.0f, 338, 99, 98}))
                {
                    State = FriendListMenuState::Idle;
                    Writing = false;

                    GetState()->NameInput = "";
                    GetState()->SearchNameFetched = false;
                }
                else if (!Writing && CheckCollisionPointRec(mousePosition, {x + 907.0f, 476, 38, 42}))
                {
                    GetState()->SearchNameFetched = true;
                    GetENetClient()->SearchName(GetState()->NameInput);
                }
                else if (!Writing && GetState()->SearchNameFetched &&
                    CheckCollisionPointRec(mousePosition, { x + 916.0f, 582, 47, 40 }))
                {
                    State = FriendListMenuState::Idle;
                    Writing = false;

                    GetState()->NameInput = "";
                    GetState()->SearchNameFetched = false;
                    GetENetClient()->AddFriendRequest(GetState()->SearchNameID);
                }
                else if (CheckCollisionPointRec(mousePosition, {x + 474.0f, 467, 430, 59}))
                {
                    Writing = true;
                }
                else
                {
                    Writing = false;
                    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                }
            }
        }
    }
}
