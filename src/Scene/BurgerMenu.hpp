#pragma once
#include "Scene/Scene.hpp"

enum class BurgerMenuType
{
    Home,
    Leaderboard,
    FriendList,
    Option
};

enum class AnimationState
{
    Idle,
    Running,
    Closing,
    Done
};

class BurgerMenuScene : public Scene
{
public:
    BurgerMenuScene(BurgerMenuType type) : Type(type) {}

    void Draw() override;
    void Update() override;

    int X = -418;
    bool Writing = false;

    BurgerMenuType Type = BurgerMenuType::Home;
    AnimationState AnimationState = AnimationState::Idle;
};
