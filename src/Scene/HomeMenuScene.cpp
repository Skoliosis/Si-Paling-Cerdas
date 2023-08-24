#include "HomeMenuScene.hpp"
#include "Utils/Utils.hpp"
#include "Network/ENetClient.hpp"
#include "MatchMenuScene.hpp"
#include "Utils/FileDialogUtils.hpp"

#include <fstream>

void HomeMenuScene::Draw()
{
    Scene::Draw();

    auto x = X + 418;
    auto mousePosition = GetMousePosition();

    DrawTexture(GetTextureManager()->Get("Textures/Logo.png"), x + 331, 117, WHITE);

    if (GetENetClient()->IsConnecting())
    {
        Utils::DrawTextCentered("Connecting to server...", 620, 30, GREEN);
    }
    else if (!GetENetClient()->IsConnected())
    {
        auto lastRetry = GetENetClient()->LastRetry.value_or(Clock::now());
        auto seconds = 5 - std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - lastRetry).count();

        Utils::DrawTextCentered("Cannot connect to server, retrying in " + std::to_string(seconds) + " seconds..", 620, 30, RED);
    }
    else if (GetENetClient()->State == ClientState::Authenticating)
    {
        Utils::DrawTextCentered("Authenticating...", 620, 30, GREEN);
    }
    else if (GetENetClient()->State == ClientState::AuthenticationFailed)
    {
        Utils::DrawTextCentered("Authentication failed, please restart the application", 620, 30, RED);
    }
    else if (GetState()->HomeMenuSceneState == HomeMenuSceneState::Idle)
    {
        Utils::DrawHoverButton("Textures/StartButton.png", "Textures/HoveredStartButton.png", {x + 546.0f, 653, 347, 115});
    }

    if (GetState()->HomeMenuSceneState == HomeMenuSceneState::Idle)
    {
        Utils::DrawHoverButton("Textures/ExitButton.png", "Textures/HoveredExitButton.png", {x + 546.0f, 808, 347, 115});
    }

    BurgerMenuScene::Draw();

    if (GetState()->HomeMenuSceneState == HomeMenuSceneState::SelectingGameMode)
    {
        DrawTexture(GetTextureManager()->Get("Textures/GameModeBackground.png"), x + 245, 199, WHITE);
        DrawTexture(GetTextureManager()->Get("Textures/GameModeTitle.png"), x + 478, 296, WHITE);

        Utils::DrawHoverButton("Textures/GameModeClassic.png", "Textures/GameModeClassicHovered.png",
                               {x + 478.0f, 544, 505, 106});
        Utils::DrawHoverButton("Textures/GameModeCompetitive.png", "Textures/GameModeCompetitiveHovered.png",
                               {x + 478.0f, 679, 505, 106});
    }
    else if (GetState()->HomeMenuSceneState == HomeMenuSceneState::WaitingQueue)
    {
        Utils::DrawTextCentered("Waiting for queue..", 705, 30, BLACK);
    }
    else if (GetState()->HomeMenuSceneState == HomeMenuSceneState::GameFound)
    {
        auto elapsed = std::chrono::high_resolution_clock::now() - GetState()->GameFoundSince;
        auto seconds = 3 - (int)std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
        Utils::DrawTextCentered("Match found, entering in " + std::to_string(seconds) + "..", 705, 30, BLACK);
    }
    else if (GetState()->HomeMenuSceneState == HomeMenuSceneState::ChangeProfile)
    {
        DrawTexture(GetTextureManager()->Get("Textures/GameModeBackground.png"), 234, 218, WHITE);

        if (GetState()->ChangeName)
        {
            DrawTexture(GetTextureManager()->Get("Textures/ChangeNameText.png"), 467, 315, WHITE);
            DrawTexture(GetTextureManager()->Get("Textures/UsernameInput.png"), 467, 471, WHITE);
            DrawTexture(GetTextureManager()->Get("Textures/UsernameNote.png"), 467, 606, WHITE);
            DrawText(GetState()->NameInput.data(), 730, 500, 40, BLACK);

            if (Writing)
            {
                int width = MeasureText(GetState()->NameInput.data(), 40);
                DrawRectangle(735 + width, 500, 10, 40, InputRectColor);
            }

            bool valid = std::all_of(GetState()->NameInput.begin(), GetState()->NameInput.end(),
                                     [](char c) { return std::isalnum(c); });
            if (!GetState()->NameInput.empty() && GetState()->NameInput.size() <= 12 && valid)
            {
                Utils::DrawHoverButton("Textures/Checklist.png", "Textures/ChecklistHovered.png", {1120, 491, 76, 73});
            }

            Utils::DrawHoverButton("Textures/BackText.png", "Textures/BackTextHovered.png", {467, 740, 132, 134});

            if (GetState()->TextError)
            {
                DrawTexture(GetTextureManager()->Get("Textures/TextNote1.png"), 450, 500, WHITE);
            }
        }
        else
        {
            DrawTexture(GetTextureManager()->Get("Textures/ChangeProfileText.png"), 467, 315, WHITE);

            Utils::DrawHoverButton("Textures/ChangeUsernameText.png", "Textures/ChangeUsernameTextHovered.png",
                                   {467, 471, 505, 121});
            Utils::DrawHoverButton("Textures/ChangePictureText.png", "Textures/ChangePictureTextHovered.png",
                                   {467, 606, 505, 121});
            Utils::DrawHoverButton("Textures/CloseText.png", "Textures/CloseTextHovered.png",
                                   {467, 740, 505, 121});

            if (!GetState()->ImageErrorNote.empty())
            {
                DrawTexture(GetTextureManager()->Get(GetState()->ImageErrorNote), 450, 500, WHITE);
            }
        }
    }
}

void HomeMenuScene::Update()
{
    Scene::Update();

    if (GetState()->HomeMenuSceneState == HomeMenuSceneState::Idle)
    {
        BurgerMenuScene::Update();
    }
    else if (GetState()->HomeMenuSceneState == HomeMenuSceneState::GameFound)
    {
        auto elapsed = std::chrono::high_resolution_clock::now() - GetState()->GameFoundSince;
        auto seconds = 3 - (int)std::chrono::duration_cast<std::chrono::seconds>(elapsed).count();
        if (seconds <= 0)
        {
            SetNextScene<MatchMenuScene>();
        }

        return;
    }

    if (AnimationState == AnimationState::Idle)
    {
        if (GetState()->ChangeName)
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

        auto mousePosition = GetMousePosition();
        if (Writing || (GetState()->ChangeName && CheckCollisionPointRec(mousePosition, {467, 471, 653, 121})))
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
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
            if (GetState()->HomeMenuSceneState == HomeMenuSceneState::SelectingGameMode)
            {
                if (CheckCollisionPointRec(mousePosition, {478.0f, 544, 505, 106}))
                {
                    GetState()->HomeMenuSceneState = HomeMenuSceneState::WaitingQueue;
                    GetENetClient()->SendAddQueue(false);
                }
                else if (CheckCollisionPointRec(mousePosition, {478.0f, 679, 505, 106}))
                {
                    GetState()->HomeMenuSceneState = HomeMenuSceneState::WaitingQueue;
                    GetENetClient()->SendAddQueue(true);
                }
            }
            else
            {
                if (GetState()->HomeMenuSceneState == HomeMenuSceneState::ChangeProfile)
                {
                    if (Writing)
                    {
                        Writing = false;
                        SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                    }
                    else if (GetState()->ChangeName)
                    {
                        if (GetState()->TextError)
                        {
                            if (CheckCollisionPointRec(mousePosition, {450, 500, 608, 162}))
                            {
                                GetState()->TextError = false;
                            }
                        }
                        else if (CheckCollisionPointRec(mousePosition, {467, 740, 132, 134}))
                        {
                            GetState()->NameInput.clear();
                            GetState()->ChangeName = false;
                        }
                        else if (CheckCollisionPointRec(mousePosition, {467, 471, 653, 121}))
                        {
                            Writing = true;
                        }
                        else if (CheckCollisionPointRec(mousePosition, {1120, 491, 76, 73}))
                        {
                            bool valid = std::all_of(GetState()->NameInput.begin(), GetState()->NameInput.end(),
                                                     [](char c) { return std::isalnum(c); });
                            if (!GetState()->NameInput.empty() && GetState()->NameInput.size() <= 12 && valid)
                            {
                                GetENetClient()->SendUpdateName(GetState()->NameInput);
                            }
                        }
                    }
                    else
                    {
                        if (!GetState()->ImageErrorNote.empty())
                        {
                            if (CheckCollisionPointRec(mousePosition, {450, 602, 608, 162}))
                            {
                                GetState()->ImageErrorNote = "";
                            }
                        }
                        else if (CheckCollisionPointRec(mousePosition, {467, 471, 505, 121}))
                        {
                            GetState()->ChangeName = true;
                        }
                        else if (CheckCollisionPointRec(mousePosition, {467, 606, 505, 121}))
                        {
                            std::string path = FileDialogUtils::ShowFileDialog();
                            std::ifstream fs(path, std::ios::binary);
                            if (fs.is_open())
                            {
                                fs.seekg(0, std::ios::end);
                                size_t fileSize = fs.tellg();
                                fs.seekg(0, std::ios::beg);

                                if (fileSize > (size_t)(10 * 1024 * 1024))
                                {
                                    GetState()->ImageErrorNote = "Textures/ImageNote1.png";
                                    return;
                                }

                                std::vector<uint8_t> fileData;
                                fileData.resize(fileSize);

                                fs.read((char *)fileData.data(), fileSize);
                                fs.close();

                                auto ext = GetFileExtension(path.data());
                                auto image = LoadImageFromMemory(ext, fileData.data(), (int)fileSize);
                                if (!image.data)
                                {
                                    GetState()->ImageErrorNote = "Textures/ImageNote2.png";
                                    return;
                                }

                                GetENetClient()->SendChangeProfilePicture(std::move(fileData), ext);
                                UnloadImage(image);

                                std::cout << "Sent" << std::endl;
                            }
                        }
                        else if (CheckCollisionPointRec(mousePosition, {467, 740, 505, 121}))
                        {
                            GetState()->HomeMenuSceneState = HomeMenuSceneState::Idle;
                        }
                    }
                }
                else if (CheckCollisionPointRec(mousePosition, {546, 808, 347, 115}))
                {
                    GetState()->Running = false;
                }
                else if (GetState()->HomeMenuSceneState != HomeMenuSceneState::ChangeProfile &&
                         GetENetClient()->State == ClientState::Ready &&
                         CheckCollisionPointRec(mousePosition, {546.0f, 653, 347, 115}))
                {
                    GetState()->HomeMenuSceneState = HomeMenuSceneState::SelectingGameMode;
                }
                else if (CheckCollisionPointCircle(mousePosition, {X + 418 + 950 + 127.0f / 2, 45 + 127.0f / 2},
                                                   127.0f / 2) ||
                         CheckCollisionPointRec(mousePosition, {X + 418.0f + 953.0f, 53.0f, 420.0f, 111.0f}))
                {
                    GetState()->HomeMenuSceneState = HomeMenuSceneState::ChangeProfile;
                }
            }
        }
    }
}
