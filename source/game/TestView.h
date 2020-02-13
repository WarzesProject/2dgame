#pragma once

#include "IGameView.h"
#include "Camera2D.h"
#include "LevelManager.h"
#include "SpriteBatch.h"
#include "GLSLProgram.h"
#include "DebugRenderer.h"
#include "SpriteFont.h"
#include "AudioEngine.h"
#include "Texture.h"

class TestView : public IGameView
{
public:
	TestView();
	~TestView();

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