#pragma once

#include "IGame.h"
#include "GameScreen.h"

class Game final : public IGame
{
public:
	Game();
	~Game();

	void OnInit(ScreenManager *screenMgr) final;

private:
	std::unique_ptr<GameScreen> m_gameplayScreen = nullptr;
};