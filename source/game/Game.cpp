#include "stdafx.h"
#include "Game.h"
//-----------------------------------------------------------------------------
void Game::Init()
{
	m_gameplay = std::make_unique<GameView>();
	m_viewManager.AddGameView(m_gameplay.get());
	m_viewManager.SetGameView(m_gameplay->GetIndex());
}
//-----------------------------------------------------------------------------