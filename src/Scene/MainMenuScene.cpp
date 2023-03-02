#include "MainMenuScene.hpp"
#include "Utils/Utils.hpp"

void MainMenuScene::Init()
{

}

void MainMenuScene::Draw()
{
    static bool b = false;
    if (!b)
    {
        Utils::DrawTextCentered("alien is real", GetScreenHeight() / 2, 20, WHITE);
        b = true;
    }
}

void MainMenuScene::Update()
{

}
