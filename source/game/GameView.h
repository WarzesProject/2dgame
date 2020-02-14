#pragma once

#include "IGameView.h"
#include "Camera2D.h"
#include "Level.h"
#include "SpriteBatch.h"
#include "GLSLProgram.h"
#include "DebugRenderer.h"
#include "SpriteFont.h"
#include "AudioSystem.h"
#include "Texture.h"
#include "DefineLogic.h"
#include "Items.h"
#include "Monster.h"
#include "Player.h"

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
	void initLevel();
	void DrawDebug(const glm::mat4 &projectionMatrix);
	void updateObject();

	SpriteBatch m_spriteBatch;
	Camera2D m_camera;
	Texture m_texture;
	GLSLProgram m_textureProgram;
	Music m_bgm;
	DebugRenderer m_debuger;

	std::unique_ptr<Level> m_level;
	Player *m_player = nullptr;
	std::vector<Monster*> m_monsters;
	std::vector<Item*> m_items;

	glm::vec2 m_screenSize;
	bool m_isDebugMode = false;
};