#pragma once
#include "BurgerMenu.hpp"

class FriendListMenuScene : public BurgerMenuScene
{
public:
    enum class FriendListMenuState
    {
        Idle,
        FriendRequest,
        FriendAdd
    };

    FriendListMenuScene() : BurgerMenuScene(BurgerMenuType::FriendList) {}

    void Draw() override;
    void Update() override;

    int StartIndex = 0;
    int RequestStartIndex = 0;
    bool InputRectColorReverse = false;
    Color InputRectColor = BLACK;
    FriendListMenuState State = FriendListMenuState::Idle;
};
