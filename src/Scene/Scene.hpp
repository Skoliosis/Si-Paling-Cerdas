#pragma once
#include <memory>

#include "Utils/Utils.hpp"
#include "Utils/State.hpp"
#include "Graphics/TextureManager.hpp"

class Scene
{
public:
    virtual void Init() {}
    virtual void Draw();
    virtual void Update();
    virtual void Destroy() {}

    ~Scene() { Destroy(); }
};

extern std::unique_ptr<Scene> g_NextScene;
extern std::unique_ptr<Scene> g_Scene;

template<typename T>
inline void SetNextScene()
{
    g_NextScene = std::make_unique<T>();
    g_NextScene->Init();
}

inline std::unique_ptr<Scene>& GetCurrentScene()
{
    return g_Scene;
}
