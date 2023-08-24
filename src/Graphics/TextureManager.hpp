#pragma once
#include <raylib.h>

#include <map>
#include <string_view>

class TextureManager
{
public:
    std::map<std::string, Texture2D> Textures;

    void Init();
    void Destroy();
    void AddTexture(std::string path);
    void AddTexture(std::string path, Image image);
    void AddTexture(std::string path, int w, int h);

    Texture2D Get(const std::string& id) { return Textures[id]; }
};

inline TextureManager* GetTextureManager()
{
    static TextureManager textureManager;
    return &textureManager;
}
