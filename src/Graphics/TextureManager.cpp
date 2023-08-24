#include "TextureManager.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>

void TextureManager::Init()
{
    AddTexture("Textures/Background.png", 1440, 1024);
    AddTexture("Textures/Bar.png", 89, 78);
    AddTexture("Textures/HoveredBar.png", 89, 78);
    AddTexture("Textures/StartButton.png", 347, 115);
    AddTexture("Textures/ExitButton.png", 347, 115);
    AddTexture("Textures/HoveredStartButton.png", 347, 115);
    AddTexture("Textures/HoveredExitButton.png", 347, 115);
    AddTexture("Textures/ProfileHover.png", 420, 111);
    AddTexture("Textures/ProfileHoverReversed.png", 420, 111);
    AddTexture("Textures/Logo.png", 777, 496);
    AddTexture("Textures/BurgerBarBackground.png", 418, 1024);
    AddTexture("Textures/BurgerBarExit.png", 99, 98);
    AddTexture("Textures/BurgerBarExitHovered.png", 99, 98);
    AddTexture("Textures/BurgerBarHome.png", 418, 102);
    AddTexture("Textures/BurgerBarHomeHovered.png", 418, 102);
    AddTexture("Textures/BurgerBarHomeChosen.png", 418, 102);
    AddTexture("Textures/BurgerBarLeaderboard.png", 418, 108);
    AddTexture("Textures/BurgerBarLeaderboardHovered.png", 418, 108);
    AddTexture("Textures/BurgerBarLeaderboardChosen.png", 418, 108);
    AddTexture("Textures/BurgerBarFriendList.png", 418, 108);
    AddTexture("Textures/BurgerBarFriendListHovered.png", 418, 108);
    AddTexture("Textures/BurgerBarFriendListChosen.png", 418, 108);
    AddTexture("Textures/BurgerBarOption.png", 418, 102);
    AddTexture("Textures/BurgerBarOptionHovered.png", 418, 102);
    AddTexture("Textures/BurgerBarOptionChosen.png", 418, 102);
    AddTexture("Textures/GameModeBackground.png", 972, 705);
    AddTexture("Textures/GameModeTitle.png", 505, 106);
    AddTexture("Textures/GameModeClassic.png", 505, 106);
    AddTexture("Textures/GameModeClassicHovered.png", 505, 106);
    AddTexture("Textures/GameModeCompetitive.png", 505, 106);
    AddTexture("Textures/GameModeCompetitiveHovered.png", 505, 106);
    AddTexture("Textures/MatchMenuBackground.png", 1440, 1024);
    AddTexture("Textures/VSLogo.png", 151, 116);
    AddTexture("Textures/AnswerOptionBackground.png", 310, 99);
    AddTexture("Textures/AnswerOptionBackgroundHovered.png", 310, 99);
    AddTexture("Textures/CorrectAnswerOptionBackground.png", 310, 99);
    AddTexture("Textures/WrongAnswerOptionBackground.png", 310, 99);
    AddTexture("Textures/LeaderboardText.png", 457, 100);
    AddTexture("Textures/LeaderboardLine.png", 1440, 10);
    AddTexture("Textures/LeaderboardInfo.png", 1144, 49);
    AddTexture("Textures/LeaderboardBackground.png", 1098, 97);
    AddTexture("Textures/FriendListText.png", 383, 100);
    AddTexture("Textures/ChangeProfileText.png", 505, 121);
    AddTexture("Textures/ChangeUsernameText.png", 505, 121);
    AddTexture("Textures/ChangeUsernameTextHovered.png", 505, 121);
    AddTexture("Textures/ChangePictureText.png", 505, 121);
    AddTexture("Textures/ChangePictureTextHovered.png", 505, 121);
    AddTexture("Textures/ChallengeLogo.png", 67, 67);
    AddTexture("Textures/ChangeNameText.png", 505, 121);
    AddTexture("Textures/UsernameInput.png", 653, 121);
    AddTexture("Textures/UsernameNote.png", 653, 134);
    AddTexture("Textures/TextNote1.png", 608, 162);
    AddTexture("Textures/ImageNote1.png", 608, 162);
    AddTexture("Textures/ImageNote2.png", 608, 162);
    AddTexture("Textures/CloseText.png", 505, 121);
    AddTexture("Textures/CloseTextHovered.png", 505, 121);
    AddTexture("Textures/BackText.png", 132, 134);
    AddTexture("Textures/BackTextHovered.png", 132, 134);
    AddTexture("Textures/Checklist.png", 76, 73);
    AddTexture("Textures/ChecklistHovered.png", 76, 73);
    AddTexture("Textures/FriendBackground.png", 1220, 112);
    AddTexture("Textures/FriendBackgroundOffline.png", 1220, 112);
    AddTexture("Textures/FriendAddLogo.png", 70, 70);
    AddTexture("Textures/FriendAddLogoHovered.png", 70, 70);
    AddTexture("Textures/FriendRequestLogo.png", 70, 70);
    AddTexture("Textures/FriendRequestLogoHovered.png", 70, 70);
    AddTexture("Textures/Trophy.png", 91, 83);
    AddTexture("Textures/OnlineLogo.png", 20, 20);
    AddTexture("Textures/OfflineLogo.png", 20, 20);
    AddTexture("Textures/FriendRequestBackground.png", 594, 559);
    AddTexture("Textures/FriendRequestOffer.png", 528, 77);
    AddTexture("Textures/FriendAddBackground.png", 600, 400);
    AddTexture("Textures/NameSearch.png", 484, 59);
    AddTexture("Textures/NameFoundBackground.png", 528, 77);
}

void TextureManager::Destroy()
{
    for (auto &&pair : Textures)
    {
        UnloadTexture(pair.second);
    }
}

void TextureManager::AddTexture(std::string path)
{
    Texture2D texture = LoadTexture(path.data());
    if (Textures.find(path) != Textures.end())
    {
        UnloadTexture(Textures[path]);
    }

    Textures.insert_or_assign(path, texture);
}

void TextureManager::AddTexture(std::string path, Image image)
{
    if (Textures.find(path) != Textures.end())
    {
        UnloadTexture(Textures[path]);
    }

    Textures.insert_or_assign(path, LoadTextureFromImage(image));
}

void TextureManager::AddTexture(std::string path, int w, int h)
{
    Image image = LoadImage(path.data());
    if (image.width != w || image.height != h)
    {
        ImageResize(&image, w, h);
    }

    if (Textures.find(path) != Textures.end())
    {
        UnloadTexture(Textures[path]);
    }

    Textures.insert_or_assign(path, LoadTextureFromImage(image));
    UnloadImage(image);
}
