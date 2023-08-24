#pragma once
#include <chrono>

#include "BurgerMenu.hpp"

class HomeMenuScene : public BurgerMenuScene
{
public:
    HomeMenuScene() : BurgerMenuScene(BurgerMenuType::Home) {}

    void Draw() override;
    void Update() override;

    bool InputRectColorReverse = false;
    Color InputRectColor = BLACK;
};
