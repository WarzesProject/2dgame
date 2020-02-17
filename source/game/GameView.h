#pragma once

#include "IGameView.h"
#include "World.h"

class GameView : public IGameView
{
public:
	virtual void Build() override {}
	virtual void Destroy() override {}

	virtual void OnEntry() override;
	virtual void OnExit() override;

	virtual void Update() override;
	virtual void Draw() override;

	virtual int GetNextViewIndex() const override;
	virtual int GetPreviousViewIndex() const override;

private:
	World m_world;
};