#include "stdafx.h"
#include "Game.h"
//-----------------------------------------------------------------------------
void Game::Init()
{
	m_gameplayScreen = std::make_unique<GameView>();
	m_viewManager.AddGameView(m_gameplayScreen.get());
	m_viewManager.SetGameView(m_gameplayScreen->GetIndex());
}
//-----------------------------------------------------------------------------