#include "State.hpp"
#include "json.hpp"

#include <filesystem>
#include <fstream>
#include <ctime>
#include <sstream>

static std::string GenerateRID()
{
    time_t theTime = time(NULL);

    struct tm aTime;
    if (localtime_s(&aTime, &theTime) != 0)
    {
        return "";
    }

    int day = aTime.tm_mday;
    int sec = aTime.tm_sec;
    int min = aTime.tm_min;
    int hour = aTime.tm_hour;
    int month = aTime.tm_mon + 1;
    int year = aTime.tm_year + 1900;

    std::stringstream stream;
    stream 
        << std::hex
        << std::uppercase
        << day
        << sec
        << min
        << hour
        << month
        << year
        << rand();
    stream.flush();

    return stream.str();
}

State State::Create()
{
    if (!std::filesystem::exists("config.json"))
    {
        nlohmann::json json;
        json["RID"] = GenerateRID();

        std::ofstream fs("config.json");
        fs << json.dump(1);
        fs.flush();
        fs.close();
    }

    nlohmann::json json = nlohmann::json::parse(std::ifstream("config.json"));

    State state;
    state.RID = json["RID"];

    return state;
}
