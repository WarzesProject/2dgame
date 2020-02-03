#include "stdafx.h"
#include "Game.h"
#include "ScreenManager.h"
#include "EventHandler.h"
#include "EventConfig.h"

TODO("���������� �� ������");
//-----------------------------------------------------------------------------
Game::Game()
{
}
//-----------------------------------------------------------------------------
Game::~Game()
{
}
//-----------------------------------------------------------------------------
void Game::OnInit(ScreenManager *screenMgr, EventHandler *eventHandler)
{
	if( !eventHandler->loadConfigFile() )
	{
		eventHandler->addEvent(MyEvent::PAUSE, SDLK_p, Joystick::START);
		eventHandler->addEvent(MyEvent::RUN, SDLK_SPACE, Joystick::LB);
		eventHandler->addEvent(MyEvent::RELOAD, SDLK_r, Joystick::RB);
		eventHandler->addEvent(MyEvent::ATTACK, SDL_BUTTON_LEFT, Joystick::DPAD_LEFT);
		eventHandler->addEvent(MyEvent::ACTION, SDL_BUTTON_RIGHT, Joystick::DPAD_RIGHT);
		eventHandler->addEvent(MyEvent::CHANGE_OBJECT, SDLK_j, Joystick::DPAD_LEFT);
		eventHandler->addEvent(MyEvent::CHANGE_WEAPON, SDLK_k, Joystick::DPAD_RIGHT);
		eventHandler->addEvent(MyEvent::USE_OBJECT, SDLK_l, Joystick::DPAD_UP);
		eventHandler->addEvent(MyEvent::GO_UP, SDLK_w, Joystick::AXE1_UP);
		eventHandler->addEvent(MyEvent::GO_DOWN, SDLK_s, Joystick::AXE1_DOWN);
		eventHandler->addEvent(MyEvent::GO_LEFT, SDLK_a, Joystick::AXE1_LEFT);
		eventHandler->addEvent(MyEvent::GO_RIGHT, SDLK_d, Joystick::AXE1_RIGHT);
	}

	//m_mainMenuScreen = std::make_unique<MainMenuScreen>();
	m_gameplayScreen = std::make_unique<GameScreen>();	

	//creenMgr->AddScreen(m_mainMenuScreen.get());
	screenMgr->AddScreen(m_gameplayScreen.get());
	screenMgr->SetScreen(m_gameplayScreen->GetScreenIndex());
}
//-----------------------------------------------------------------------------