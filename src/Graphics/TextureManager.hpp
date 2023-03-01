#pragma once
#include <raylib.h>
#include <vector>
#include <string_view>

class TextureManager
{
public:
    std::vector<Texture2D> Textures;

    void Init();
    void Destroy();
    void AddTexture(std::string_view path);
};

inline TextureManager* GetTextureManager()
{
    static TextureManager textureManager;
    return &textureManager;
}
