#pragma once

class FPSLimiter
{
public:
	void Init(float maxFPS);

	void Begin();
	void End();

	void SetMaxFPS(float maxFPS);
	float GetFPS() const
	{
		return m_fps;
	}

private:
	void calculateFPS();

	float m_fps;
	float m_maxFPS;
	float m_frameTime;
	unsigned int m_startTicks;
};