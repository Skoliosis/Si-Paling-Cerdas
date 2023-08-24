#include <vector>
#include <string>

namespace ProfilePictureUtils
{
void PutProfilePicture(const std::vector<uint8_t> &data, const std::string &extension, const std::string &pfpFile,
                       const std::string &roundPfpFile);
void PutFriendProfilePicture(const std::vector<uint8_t> data, const std::string &extension, const std::string &pfpFile,
                             int w = 90, int h = 90);
}