#include "IntroductionScene.hpp"
#include "MainMenuScene.hpp"
#include "Utils/Utils.hpp"

#include <algorithm>

#define APP_NAME "Si Paling Cerdas"

void IntroductionScene::Init()
{
    m_DevLists.emplace_back("Ricky Jiunardi (2540133436) (Leader)");
    m_DevLists.emplace_back("Catherine Kwee (2540124841)");
    m_DevLists.emplace_back("Marco Anthonio Samkakai (2501960442)");
    m_DevLists.emplace_back("Devani Marcellina Calista (2540124570)");
    m_DevLists.emplace_back("Oey, Kevin Andrian Santoso (2540131084)");
}

void IntroductionScene::Draw()
{
    int appNamePositionY = 30;
    int appNameFontSize = 50;
    int presentingPositionY = appNamePositionY + appNameFontSize + 120;
    int presentingFontSize = 32;
    int devNamePadding = 20;
    int devNameFontSize = 28;

    Utils::DrawTextCentered(APP_NAME, appNamePositionY, appNameFontSize, WHITE);
    Utils::DrawTextCentered("Project made by the Skoliosis Team:", presentingPositionY, presentingFontSize, WHITE);

    for (int i = 0; i <= m_CurrentDevIndex; ++i)
    {
        if (i >= m_DevLists.size())
        {
            break;
        }

        std::string &devName = m_DevLists[i];

        int devNameTextLength = MeasureText(devName.data(), devNameFontSize);
        int devNamePositionX = GetScreenWidth() / 2 - devNameTextLength / 2;
        int devNamePositionY = presentingPositionY + presentingFontSize + devNamePadding + i * devNameFontSize + 10;
        Color devNameColor = i == m_CurrentDevIndex ? m_TransitionColor : WHITE;

        DrawText(devName.data(), devNamePositionX, devNamePositionY, devNameFontSize, devNameColor);
    }
}

void IntroductionScene::Update()
{
    m_TransitionTimer += GetFrameTime();
    m_TransitionTextAlpha = (int)(255.0f * std::clamp(m_TransitionTimer, 0.0f, 1.0f));

    if (m_TransitionTimer >= 1.0f)
    {
        m_CurrentDevIndex += 1;
        m_TransitionTimer = -1.0f;
        m_TransitionColor = WHITE;
        m_TransitionTextAlpha = 0.0f;

        if (m_CurrentDevIndex == m_DevLists.size())
        {
            m_TransitionTimer = -1.5f; // Make the transition a bit longer ig
        }
        else if (m_CurrentDevIndex > m_DevLists.size())
        {
            SetCurrentScene<MainMenuScene>(); // We are done!
        }
    }

    m_TransitionColor.a = (uint8_t)m_TransitionTextAlpha;
}
