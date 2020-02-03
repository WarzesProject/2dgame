#pragma once

#include "IGameScreen.h"
#include "Camera2D.h"
#include "GameRenderer.h"

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
	Camera2D m_camera;
	GameRenderer m_renderer;
};