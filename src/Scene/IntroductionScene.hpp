#pragma once
#include <vector>
#include <string>

#include "Scene.hpp"
#include "Utils/Time.hpp"

class IntroductionScene : public Scene
{
private:
    int m_CurrentDevIndex = 0;
    int m_TransitionTextAlpha = 0;
    Color m_TransitionColor = WHITE;
    float m_TransitionTimer = -1.0f;
    std::vector<std::string> m_DevLists;

public:
    void Init();
    void Draw();
    void Update();
    void Destroy() {}
};
