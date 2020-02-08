#pragma once

#include "IGameApp.h"
#include "GameView.h"

class Game final : public IGameApp
{
public:
	void Init() final;

private:
	std::unique_ptr<GameView> m_gameplayScreen = nullptr;
};