#pragma once

#include "IGameView.h"
#include "Camera2D.h"

class GameView : public IGameView
{
public:
	GameView();
	~GameView();

	virtual void Build() override;
	virtual void Destroy() override;

	virtual void OnEntry() override;
	virtual void OnExit() override;

	virtual void Update() override;
	virtual void Draw() override;

	virtual int GetNextViewIndex() const override;
	virtual int GetPreviousViewIndex() const override;

private:
	Camera2D m_camera;
};