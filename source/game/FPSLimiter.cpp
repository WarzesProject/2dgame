#include "stdafx.h"
#include "FPSLimiter.h"
//-----------------------------------------------------------------------------
constexpr auto NUM_SAMPLES = 10;
//-----------------------------------------------------------------------------
void FPSLimiter::Init(float maxFPS)
{
	SetMaxFPS(maxFPS);
}
//-----------------------------------------------------------------------------
void FPSLimiter::SetMaxFPS(float maxFPS)
{
	m_maxFPS = maxFPS;
}
//-----------------------------------------------------------------------------
void FPSLimiter::Begin()
{
	m_startTicks = SDL_GetTicks();
}
//-----------------------------------------------------------------------------
void FPSLimiter::End()
{
	calculateFPS();

	float frameTicks = (float)(SDL_GetTicks() - m_startTicks);
	const float step = 1000.0f / m_maxFPS;

	if ( step > frameTicks )
		SDL_Delay((unsigned int)(step - frameTicks));
}
//-----------------------------------------------------------------------------
void FPSLimiter::calculateFPS()
{
	static float frameTimes[NUM_SAMPLES];
	static int currentFrame = 0;
	static float prevTiks = (float)SDL_GetTicks();
	float currentTick = (float)SDL_GetTicks();

	m_frameTime = currentTick - prevTiks;
	frameTimes[currentFrame % NUM_SAMPLES] = m_frameTime;

	currentFrame++;
	prevTiks = currentTick;

	int count;

	if ( currentFrame < NUM_SAMPLES )
		count = currentFrame;
	else
		count = NUM_SAMPLES;

	float frameTimeAverage = 0;
	for ( int i = 0; i < count; i++ )
	{
		frameTimeAverage += frameTimes[i];
	}
	frameTimeAverage /= count;

	if ( frameTimeAverage > 0 )
		m_fps = 1000.0f / frameTimeAverage;
	else
		m_fps = 60.0f;
}
//-----------------------------------------------------------------------------