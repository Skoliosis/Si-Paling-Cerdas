#include "ENetClient.hpp"

#include "Utils/State.hpp"
#include "Utils/ProfilePicture.hpp"

#include <enet/enet.h>
#include <iostream>

bool ENetClient::Init()
{
    if (enet_initialize() != 0)
    {
        return false;
    }

    m_Host = enet_host_create(nullptr, 1024, 10, 0, 0);
    if (!m_Host)
    {
        return false;
    }

    return true;
}

bool ENetClient::Connect()
{
    ENetAddress address{};
    address.port = 17091;

    enet_address_set_host_ip(&address, "172.23.76.243");

    m_Peer = enet_host_connect(m_Host, &address, 1, 0);
    if (!m_Peer)
    {
        return false;
    }

    enet_peer_timeout(m_Peer, 2, 1000, 1000);

    return true;
}

void ENetClient::Poll()
{
    if (LastRetry.has_value())
    {
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(Clock::now() - LastRetry.value()).count();
        if (seconds >= 5)
        {
            Connect();
            LastRetry = std::nullopt;
        }

        return;
    }

    ENetEvent event;
    if (enet_host_service(m_Host, &event, 1) <= 0)
    {
        return;
    }

    switch (event.type)
    {
        case ENET_EVENT_TYPE_CONNECT:
        {
            std::cout << "Peer connected!" << std::endl;

            //enet_peer_timeout(m_Peer, ENET_PEER_TIMEOUT_LIMIT, ENET_PEER_TIMEOUT_MINIMUM, ENET_PEER_TIMEOUT_MAXIMUM);
            SendAuthentication();
            break;
        }

        case ENET_EVENT_TYPE_RECEIVE:
        {
            HandleIncomingPacket(event.packet);
            break;
        }

        case ENET_EVENT_TYPE_DISCONNECT:
        {
            std::cout << "Peer disconnected!" << std::endl;

            if (State == ClientState::Ready)
            {
                Connect();
            }
            else
            {
                LastRetry = Clock::now();
            }

            State = ClientState::Authenticating;
            GetState()->HomeMenuSceneState = HomeMenuSceneState::Idle;
            break;
        }
    }
}

void ENetClient::Destroy()
{
    if (IsConnected())
    {
        enet_peer_disconnect_later(m_Peer, 0);
        enet_host_flush(m_Host);
    }
}

void ENetClient::HandleIncomingPacket(_ENetPacket *packet)
{
    //std::string jsonStr{(char *)packet->data, packet->dataLength};

    auto json = nlohmann::json::from_bson(packet->data, packet->data + packet->dataLength);
    int packetID = json["PacketID"];

    switch ((PacketID)packetID)
    {
        case PacketID::Authentication:
        {
            if (json["Error"] == false)
            {
                State = ClientState::Ready;
                ProfilePictureUtils::PutProfilePicture(json["ProfilePicture"].get_binary(), json["ProfilePictureExtension"],
                                                       "Textures/ProfilePicture.png", "Textures/RoundProfilePicture.png");

                GetState()->Name = json["Name"];
            }
            else
            {
                State = ClientState::AuthenticationFailed;
            }
            
            break;
        }

        case PacketID::AddQueue:
        {
            ProfilePictureUtils::PutProfilePicture(json["ProfilePicture"].get_binary(), json["ProfilePictureExtension"],
                                                   "Textures/EnemyProfilePicture.png",
                                                   "Textures/EnemyRoundProfilePicture.png");

            GetState()->EnemyName = json["Name"];
            GetState()->GameFoundSince = Clock::now();
            GetState()->HomeMenuSceneState = HomeMenuSceneState::GameFound;
            break;
        }

        case PacketID::QuestionUpdate:
        {
            GetState()->Points = json["Points"];
            GetState()->Question = json["Question"];
            GetState()->AnswerReady = false;
            GetState()->EnemyPoints = json["EnemyPoints"];
            GetState()->AnswerOption1 = json["AnswerOption1"];
            GetState()->AnswerOption2 = json["AnswerOption2"];
            GetState()->AnswerOption3 = json["AnswerOption3"];
            GetState()->AnswerOption4 = json["AnswerOption4"];
            GetState()->QuestionTimer = Clock::now();
            GetState()->AlreadyAnswered = false;
            break;
        }

        case PacketID::QuestionAnswer:
        {
            GetState()->AnswerReady = true;
            GetState()->CorrectAnswerIndex = json["AnswerIndex"];
            break;
        }

        case PacketID::FetchLeaderboard:
        {
            GetState()->Leaderboard.clear();

            const auto &array = json["Leaderboard"];

            for (size_t i = 0; i < array.size(); ++i)
            {
                const auto &j = array[i];

                LeaderboardInfo info{};
                info.Win = j["Win"];
                info.Lose = j["Lose"];
                info.Name = j["Name"];
                info.Rating = j["Rating"];

                GetState()->Leaderboard.emplace_back(info);

                auto profilePicturePath = "Textures/LeaderboardProfilePicture" + std::to_string(i) + ".png";
                auto roundProfilePicturePath = "Textures/LeaderboardRoundProfilePicture" + std::to_string(i) + ".png";

                ProfilePictureUtils::PutProfilePicture(j["ProfilePicture"].get_binary(), j["ProfilePictureExtension"],
                                                       profilePicturePath,
                                                       roundProfilePicturePath);
            }

            break;
        }

        case PacketID::ChangeProfilePicture:
        {
            ProfilePictureUtils::PutProfilePicture(json["ProfilePicture"].get_binary(), json["ProfilePictureExtension"],
                                                   "Textures/ProfilePicture.png", "Textures/RoundProfilePicture.png");
            break;
        }

        case PacketID::UpdateName:
        {
            if (json["Error"] == false)
            {
                GetState()->Name = json["Name"];
                GetState()->NameInput.clear();
                GetState()->ChangeName = false;
            }
            else
            {
                GetState()->TextError = true;
            }

            break;
        }

        case PacketID::GameEnded:
        {
            GetState()->Winner = json["Winner"];
            GetState()->GameEndedSince = Clock::now();
            break;
        }

        case PacketID::FetchFriend:
        {
            GetState()->Friends.clear();

            const auto &array = json["Friends"];

            for (size_t i = 0; i < array.size(); ++i)
            {
                const auto &j = array[i];

                FriendInfo info{};
                info.Name = j["Name"];
                info.Online = j["Online"];

                GetState()->Friends.emplace_back(info);

                auto profilePicturePath = "Textures/FriendProfilePicture" + std::to_string(i) + ".png";

                ProfilePictureUtils::PutFriendProfilePicture(j["ProfilePicture"].get_binary(), j["ProfilePictureExtension"],
                                                             profilePicturePath);
            }

            break;
        }

        case PacketID::FetchFriendRequest:
        {
            GetState()->FriendRequests.clear();

            const auto &array = json["FriendRequests"];

            for (size_t i = 0; i < array.size(); ++i)
            {
                const auto &j = array[i];

                FriendRequestInfo info{};
                info.ID = j["ID"];
                info.Name = j["Name"];

                GetState()->FriendRequests.emplace_back(info);

                auto profilePicturePath = "Textures/FriendRequestProfilePicture" + std::to_string(i) + ".png";

                ProfilePictureUtils::PutFriendProfilePicture(j["ProfilePicture"].get_binary(), j["ProfilePictureExtension"],
                                                             profilePicturePath, 55, 55);
            }

            break;
        }

        case PacketID::SearchName:
        {
            GetState()->NameFound = json["Name"];
            GetState()->SearchNameFound = json["Found"];

            if (GetState()->SearchNameFound)
            {
                ProfilePictureUtils::PutFriendProfilePicture(json["ProfilePicture"].get_binary(),
                                                             json["ProfilePictureExtension"],
                                                             "Textures/NameFoundProfilePicture.png", 55, 55);
            }

            break;
        }
    }
}

bool ENetClient::IsConnected() const
{
    return m_Peer != nullptr && m_Peer->state == ENET_PEER_STATE_CONNECTED;
}

bool ENetClient::IsConnecting() const
{
    return m_Peer != nullptr && m_Peer->state == ENET_PEER_STATE_CONNECTING;
}

void ENetClient::SendPacket(nlohmann::json &&json)
{
    std::vector<uint8_t> bson = nlohmann::json::to_bson(json);

    ENetPacket *packet = enet_packet_create(bson.data(), bson.size(), ENET_PACKET_FLAG_RELIABLE);
    if (!packet)
    {
        return;
    }

    if (enet_peer_send(m_Peer, 0, packet) != 0)
    {
        enet_packet_destroy(packet);
    }
}

void ENetClient::SendAuthentication()
{
    nlohmann::json json;
    json["PacketID"] = (int)PacketID::Authentication;
    json["RID"] = GetState()->RID;

    SendPacket(std::move(json));
}

void ENetClient::SendAddQueue(bool competitive)
{
    nlohmann::json json;
    json["PacketID"] = (int)PacketID::AddQueue;
    json["Competitive"] = competitive;

    SendPacket(std::move(json));
}

void ENetClient::SendAnswer(int index)
{
    nlohmann::json json;
    json["PacketID"] = (int)PacketID::QuestionAnswer;
    json["AnswerIndex"] = index;

    SendPacket(std::move(json));
}

void ENetClient::FetchFriend()
{
    nlohmann::json json;
    json["PacketID"] = (int)PacketID::FetchFriend;

    SendPacket(std::move(json));
}

void ENetClient::SearchName(const std::string &name)
{
    nlohmann::json json;
    json["PacketID"] = (int)PacketID::SearchName;
    json["Name"] = name;

    SendPacket(std::move(json));
}

void ENetClient::FetchLeaderboard()
{
    nlohmann::json json;
    json["PacketID"] = (int)PacketID::FetchLeaderboard;

    SendPacket(std::move(json));
}

void ENetClient::FetchFriendRequest()
{
    nlohmann::json json;
    json["PacketID"] = (int)PacketID::FetchFriendRequest;

    SendPacket(std::move(json));
}

void ENetClient::SendUpdateName(const std::string &name)
{
    nlohmann::json json;
    json["PacketID"] = (int)PacketID::UpdateName;
    json["Name"] = name;

    SendPacket(std::move(json));
}

void ENetClient::AddFriendRequest(int id)
{
    nlohmann::json json;
    json["PacketID"] = (int)PacketID::AddFriendRequest;
    json["ID"] = id;

    SendPacket(std::move(json));
}

void ENetClient::SendChangeProfilePicture(std::vector<uint8_t> &&picture, std::string extension)
{
    nlohmann::json json;
    json["PacketID"] = (int)PacketID::ChangeProfilePicture;
    json["ProfilePicture"] = nlohmann::json::binary_t{ picture };
    json["ProfilePictureExtension"] = extension;

    SendPacket(std::move(json));
}
