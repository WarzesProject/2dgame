#pragma once

class FPSLimiter
{
public:
	void Init(float maxFPS);

	void Begin();
	void End();

	void SetMaxFPS(float maxFPS);
	float GetFPS() const { return m_fps; }

private:
	void calculateFPS();

	float m_fps = 0.0f;
	float m_maxFPS = 0.0f;
	float m_frameTime = 0.0f;
	unsigned int m_startTicks = 0;
};