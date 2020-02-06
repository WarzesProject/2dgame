#include "stdafx.h"
#include "Application.h"
#include "IGameScreen.h"

//-----------------------------------------------------------------------------
Application::Application(const ApplicationConfig &config)
	: m_config(config)
{
	m_limiter.SetMaxFPS(m_config.MaxFPS);

	if( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
		ThrowSDLError("Fail to Initialize SDL: ");
	
	m_window = std::make_unique<Window>(m_config);
	m_audioSystem.Init();
	m_resourceManager.Init(&m_audioSystem);
	m_eventHandler.Init(m_config.width, m_config.height);
	m_screenManager.Init(this);

	m_isRunning = true;
}
//-----------------------------------------------------------------------------
Application::~Application()
{
	if ( m_currentScreen )
		m_currentScreen->OnExit();
	m_screenManager.Destroy();
	m_game.reset();
	m_eventHandler.Destroy();
	m_resourceManager.Destroy();
	m_audioSystem.Destroy();
	m_render.reset();
	m_window.reset();
	SDL_Quit();
}
//-----------------------------------------------------------------------------
void Application::Run(std::unique_ptr<IGame> game)
{
	m_game = std::move(game);

	m_game->OnInit(&m_screenManager, &m_eventHandler);
	m_currentScreen = m_screenManager.GetCurrentScreen();
	m_currentScreen->OnEntry();
	m_currentScreen->SetGameState(ScreenState::RUNNING);

	while( m_isRunning )
	{
		m_limiter.Begin();
		m_eventHandler.Update(m_limiter.GetFPS());
		update();
		draw();
		m_limiter.End();
		m_window->SwapBuffer();
	}
}
//-----------------------------------------------------------------------------
void Application::update()
{
	SDL_Event ev;

	if ( m_currentScreen )
	{
		switch ( m_currentScreen->GetGameState() )
		{
		case ScreenState::RUNNING:
			{
				while ( SDL_PollEvent(&ev) )
					onSDLEvent(ev);
				m_currentScreen->Update();
			}
			break;
		case ScreenState::CHANGE_NEXT:
			m_currentScreen->OnExit();
			m_currentScreen = m_screenManager.MoveNext();
			if ( m_currentScreen )
			{
				m_currentScreen->SetGameState(ScreenState::RUNNING);
				m_currentScreen->OnEntry();
			}
			break;
		case ScreenState::CHANGE_PREVIOUS:
			m_currentScreen->OnExit();
			m_currentScreen = m_screenManager.MovePrevious();
			if ( m_currentScreen )
			{
				m_currentScreen->OnEntry();
				m_currentScreen->SetGameState(ScreenState::RUNNING);
			}
			break;
		case ScreenState::EXIT_GAME:
			m_isRunning = false;
			break;
		default:
			break;
		}
	}
	else
	{
		m_isRunning = false;
	}
}
//-----------------------------------------------------------------------------
void Application::draw()
{
	if ( m_currentScreen && m_currentScreen->GetGameState() == ScreenState::RUNNING )
	{
		glViewport(0, 0, m_window->GetScreenWidth(), m_window->GetScreenHeight());
		m_currentScreen->Draw();
	}
}
//-----------------------------------------------------------------------------
void Application::onSDLEvent(SDL_Event &ev)
{
	if( m_eventHandler.GetPlayWith() == PlayWith::KEYBOARD )
	{
		switch( ev.type )
		{
		case SDL_QUIT:
			m_currentScreen->SetGameState(ScreenState::CHANGE_PREVIOUS);
			break;
		case SDL_MOUSEMOTION:
			m_eventHandler.SetMouseCoords((float)ev.motion.x, (float)ev.motion.y);
			break;
		case SDL_KEYDOWN:
			m_eventHandler.PressKey(ev.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_eventHandler.ReleaseKey(ev.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_eventHandler.PressKey(ev.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_eventHandler.ReleaseKey(ev.button.button);
			break;
		}
	}
	else
	{
		switch( ev.type )
		{
		case SDL_QUIT:
			m_currentScreen->SetGameState(ScreenState::CHANGE_PREVIOUS);
			break;
		case SDL_JOYBUTTONDOWN:
			m_eventHandler.PressKey(ev.jbutton.button + MARGIN);
			break;
		case SDL_JOYBUTTONUP:
			m_eventHandler.ReleaseKey(ev.jbutton.button + MARGIN);
			break;
		case SDL_JOYAXISMOTION:
			m_eventHandler.UpdateJoystickAxis(ev.jaxis.axis, ev.jaxis.value, m_window->GetSDLWindow());
			break;
		case SDL_JOYHATMOTION:
			m_eventHandler.UpdateJoystickHats(ev.jhat.value + MARGIN);
			break;
		}
	}
}
//-----------------------------------------------------------------------------