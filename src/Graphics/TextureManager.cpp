#include "TextureManager.hpp"

void TextureManager::Init()
{
}

void TextureManager::Destroy()
{
    for (auto &&texture : Textures)
    {
        UnloadTexture(texture);
    }
}

void TextureManager::AddTexture(std::string_view path)
{
    Texture2D texture = LoadTexture(path.data());
    Textures.emplace_back(texture);
}
