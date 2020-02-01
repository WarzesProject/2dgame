#include "stdafx.h"
#include "Timer.h"
//-----------------------------------------------------------------------------
Timer::Timer()
{
	Reset();
}
//-----------------------------------------------------------------------------
void Timer::Reset()
{
	m_startTime = m_HRClock.now();
}
//-----------------------------------------------------------------------------
int64_t Timer::GetMilliseconds() const
{
	auto newTime = m_HRClock.now();
	std::chrono::duration<double> dur = newTime - m_startTime;

	return duration_cast<std::chrono::milliseconds>(dur).count();
}
//-----------------------------------------------------------------------------
int64_t Timer::GetMicroseconds() const
{
	auto newTime = m_HRClock.now();
	std::chrono::duration<double> dur = newTime - m_startTime;

	return duration_cast<std::chrono::microseconds>(dur).count();
}
//-----------------------------------------------------------------------------
int64_t Timer::GetStartMs() const
{
	std::chrono::nanoseconds startTimeNs = m_startTime.time_since_epoch();
	return duration_cast<std::chrono::milliseconds>(startTimeNs).count();
}
//-----------------------------------------------------------------------------