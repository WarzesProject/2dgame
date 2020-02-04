#pragma once

#include "IGame.h"
#include "GameScreen.h"
#include "MainMenuScreen.h"

TODO("класс можно удалить, вместо этого передавая скрины напрямую");

class Game final : public IGame
{
public:
	void OnInit(ScreenManager *screenMgr, EventHandler *eventHandler) final;

private:
	std::unique_ptr<GameScreen> m_gameplayScreen = nullptr;
	std::unique_ptr<MainMenuScreen> m_mainMenuScreen = nullptr;
};