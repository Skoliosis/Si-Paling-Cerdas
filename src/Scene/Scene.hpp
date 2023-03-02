#pragma once
#include <memory>
#include <raylib.h>

class Scene
{
public:
    virtual void Init() {}
    virtual void Draw() = 0;
    virtual void Update() = 0;
    virtual void Destroy() {}

    ~Scene() { Destroy(); }
};

extern std::shared_ptr<Scene> g_Scene;

template<typename T>
void SetCurrentScene()
{
    g_Scene = std::make_shared<T>();
    g_Scene->Init();
}

inline std::shared_ptr<Scene> GetCurrentScene()
{
    return g_Scene;
}
