#pragma once

#include "IGameView.h"
#include "Camera2D.h"
#include "TestLogic.h"
#include "SpriteBatch.h"
#include "GLSLProgram.h"
#include "DebugRenderer.h"
#include "SpriteFont.h"
#include "AudioEngine.h"
#include "Texture.h"

#define FIRST_TEST 0

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
#if !FIRST_TEST

#endif
};