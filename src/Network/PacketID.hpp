#pragma once
#include <cstdint>

enum class PacketID : uint32_t
{
	Authentication,
	AddQueue,
	QuestionUpdate,
	QuestionAnswer,
    FetchLeaderboard,
	ChangeProfilePicture,
    UpdateName,
	GameEnded,
	FetchFriend,
    FetchFriendRequest,
	AcceptFriendRequest,
	DeclineFriendRequest,
	SearchName,
	AddFriendRequest
};
