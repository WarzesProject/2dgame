#pragma once

class Timer
{
public:
	Timer();
	void Reset();

	int64_t GetMilliseconds() const;
	int64_t GetMicroseconds() const;
	int64_t GetStartMs() const;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_startTime;
	std::chrono::high_resolution_clock m_HRClock;

};