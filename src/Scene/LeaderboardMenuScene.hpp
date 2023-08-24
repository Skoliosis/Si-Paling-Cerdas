#pragma once
#include "BurgerMenu.hpp"

class LeaderboardMenuScene : public BurgerMenuScene
{
public:
    LeaderboardMenuScene() : BurgerMenuScene(BurgerMenuType::Leaderboard) {}

    void Init() override;
    void Draw() override;
    void Update() override;

    int StartIndex = 0;
};
