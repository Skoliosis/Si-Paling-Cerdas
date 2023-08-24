#include "Scene.hpp"
#include "HomeMenuScene.hpp"
#include "LeaderboardMenuScene.hpp"
#include "MatchMenuScene.hpp"

std::unique_ptr<Scene> g_Scene = std::make_unique<HomeMenuScene>();
std::unique_ptr<Scene> g_NextScene = nullptr;

void Scene::Draw()
{
    DrawTexture(GetTextureManager()->Get("Textures/Background.png"), 0, 0, WHITE);

#ifdef _DEBUG
    DrawFPS(0, 0);
#endif
}

void Scene::Update()
{

}
