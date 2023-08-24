#include "ProfilePicture.hpp"
#include "Utils.hpp"
#include "Graphics/TextureManager.hpp"

#include <raylib.h>

namespace ProfilePictureUtils
{
void PutProfilePicture(const std::vector<uint8_t> &data, const std::string &extension, const std::string &pfpFile,
                       const std::string &roundPfpFile)
{
    Image image = LoadImageFromMemory(extension.data(), data.data(), (int)data.size());
    ImageResize(&image, 127, 127);

    Image roundedImage = Utils::RoundImage(image);

    GetTextureManager()->AddTexture(pfpFile, image);
    GetTextureManager()->AddTexture(roundPfpFile, roundedImage);

    UnloadImage(image);
    UnloadImage(roundedImage);
}

void PutFriendProfilePicture(const std::vector<uint8_t> data, const std::string &extension, const std::string &pfpFile,
                             int w, int h)
{
    Image image = LoadImageFromMemory(extension.data(), data.data(), (int)data.size());
    ImageResize(&image, w, h);
    GetTextureManager()->AddTexture(pfpFile, image);
    UnloadImage(image);
}

}
