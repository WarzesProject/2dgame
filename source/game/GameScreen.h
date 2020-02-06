#pragma once

#include "IGameScreen.h"
#include "Camera2D.h"
#include "GameRenderer.h"
#include "Particle2DEngine.h"
#include "World.h"
#include "EntityController.h"

class Monster;

enum class GameState
{
	PLAY, EXIT
};

class GameScreen : public IGameScreen
{
public:
	GameScreen();
	~GameScreen();

	virtual void Build() override;
	virtual void Destroy() override;

	virtual void OnEntry() override;
	virtual void OnExit() override;

	virtual void Update() override;
	virtual void Draw() override;

	virtual int GetNextScreenIndex() const override;
	virtual int GetPreviousScreenIndex() const override;

private:
	void checkInput();
	void initSystems();
	void initLevel();

	float m_previousTicks;
	bool m_inPause = false;

	Camera2D m_camera;
	Particle2DEngine m_particleEngine;

	World m_currentLevel;
	EntityController m_entityController;
	GameRenderer m_renderer;

	int m_screenWidth; ///< Screen width
	int m_screenHeight; ///< Screen height
	GameState m_gameState; ///< State of the game
};