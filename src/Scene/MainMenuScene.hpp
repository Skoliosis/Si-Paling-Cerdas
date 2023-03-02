#pragma once
#include "Scene.hpp"

class MainMenuScene : public Scene
{
public:
    void Init();
    void Draw();
    void Update();
    void Destroy() {}
};
