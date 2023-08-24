#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

typedef std::chrono::high_resolution_clock Clock;

enum class HomeMenuSceneState
{
    Idle,
    SelectingGameMode,
    WaitingQueue,
    GameFound,
    ChangeProfile
};

struct LeaderboardInfo
{
    int Win = 0;
    int Lose = 0;
    int Rating = 0;

    std::string Name;
};

struct FriendInfo
{
    bool Online = false;
    std::string Name;
};

struct FriendRequestInfo
{
    int ID = 0;
    std::string Name;
};

class State
{
public:
    int Points = 0;
    int EnemyPoints = 0;
    int AnswerIndex = -1;
    int SearchNameID = -1;
    int CorrectAnswerIndex = -1;

    bool Running = true;
    bool TextError = false;
    bool ChangeName = false;
    bool AnswerReady = false;
    bool FriendFetched = false;
    bool SearchNameFound = false;
    bool AlreadyAnswered = false;
    bool SearchNameFetched = false;
    bool LeaderboardFetched = false;
    bool FriendRequestFetched = false;

    std::string RID;
    std::string Name;
    std::string Winner;
    std::string Question;
    std::string NameInput;
    std::string NameFound;
    std::string EnemyName;
    std::string AnswerOption1;
    std::string AnswerOption2;
    std::string AnswerOption3;
    std::string AnswerOption4;
    std::string ImageErrorNote;

    std::vector<FriendInfo> Friends;
    std::vector<LeaderboardInfo> Leaderboard;
    std::vector<FriendRequestInfo> FriendRequests;

    HomeMenuSceneState HomeMenuSceneState = HomeMenuSceneState::Idle;

    Clock::time_point QuestionTimer = Clock::now();
    Clock::time_point GameFoundSince = Clock::now();
    Clock::time_point GameEndedSince = Clock::now();

    static State Create();
};

inline State *GetState()
{
    static State state = State::Create();
    return &state;
}
