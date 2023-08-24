#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include <json.hpp>

#include "PacketID.hpp"

struct _ENetHost;
struct _ENetPeer;
struct _ENetPacket;

enum class ClientState
{
    Authenticating,
    AuthenticationFailed,
    Ready
};

class ENetClient
{
public:
    bool Init();
    bool Connect();
    void Poll();
    void Destroy();

    void HandleIncomingPacket(_ENetPacket *packet);

    bool IsConnected() const;
    bool IsConnecting() const;

    void SendPacket(nlohmann::json &&json);
    void SendAuthentication();
    void SendAddQueue(bool competitive);
    void SendAnswer(int index);
    void FetchFriend();
    void SearchName(const std::string &name);
    void FetchLeaderboard();
    void FetchFriendRequest();
    void SendUpdateName(const std::string &name);
    void AddFriendRequest(int id);
    void SendChangeProfilePicture(std::vector<uint8_t> &&picture, std::string extension);

    ClientState State = ClientState::Authenticating;
    std::optional<std::chrono::high_resolution_clock::time_point> LastRetry;

private:
    _ENetHost *m_Host = nullptr;
    _ENetPeer *m_Peer = nullptr;
};

inline ENetClient* GetENetClient()
{
    static ENetClient client;
    return &client;
}
