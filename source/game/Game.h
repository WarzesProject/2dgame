#pragma once

#include "IGame.h"
#include "GameScreen.h"
#include "MainMenuScreen.h"

class Game final : public IGame
{
public:
	Game();
	~Game();

	void OnInit(ScreenManager *screenMgr, EventHandler *eventHandler) final;

private:
	std::unique_ptr<GameScreen> m_gameplayScreen = nullptr;
	std::unique_ptr<MainMenuScreen> m_mainMenuScreen = nullptr;
};