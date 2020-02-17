#pragma once

#include "IGameApp.h"
#include "GameView.h"

class Game final : public IGameApp
{
public:
	Game() = default;
	void Init() final;

private:
	Game(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) = delete;

	std::unique_ptr<GameView> m_gameplay = nullptr;
};