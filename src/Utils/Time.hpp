#pragma once
#include <chrono>

class TimeSpan
{
private:
    std::chrono::high_resolution_clock::duration m_Time;

public:
    TimeSpan() { m_Time = std::chrono::milliseconds(0); }
    TimeSpan(std::chrono::high_resolution_clock::duration time) { m_Time = time; }

    inline int64_t GetSeconds()
    {
        return std::chrono::duration_cast<std::chrono::seconds>(m_Time).count();
    }

    inline int64_t GetMicroseconds()
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(m_Time).count();
    }

    inline int64_t GetMilliseconds()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_Time).count();
    }
};

class Timer
{
private:
    std::chrono::high_resolution_clock::time_point m_Start;

public:
    inline Timer() { Reset(); }
    inline void Reset()
    {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    inline TimeSpan GetElapsed()
    {
        return TimeSpan(std::chrono::high_resolution_clock::now() - m_Start);
    }
};
