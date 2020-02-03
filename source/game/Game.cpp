#include "stdafx.h"
#include "Game.h"
#include "ScreenManager.h"

//-----------------------------------------------------------------------------
Game::Game()
{
}
//-----------------------------------------------------------------------------
Game::~Game()
{
}
//-----------------------------------------------------------------------------
void Game::OnInit(ScreenManager *screenMgr)
{
	m_gameplayScreen = std::make_unique<GameScreen>();

	screenMgr->AddScreen(m_gameplayScreen.get());
	screenMgr->SetScreen(m_gameplayScreen->GetScreenIndex());
}
//-----------------------------------------------------------------------------