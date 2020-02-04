#include "stdafx.h"
#include "Application.h"
#if RENDER_VULKAN
#	include "VKRenderer.h"
#endif
#include "IGameScreen.h"

//-----------------------------------------------------------------------------
Application::Application(const ApplicationConfig &config)
	: m_config(config)
{
	if( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
		ThrowSDLError("Fail to Initialize SDL: ");

#if RENDER_VULKAN
	if( m_config.renderType == RenderType::Vulkan )
	{
		if( SDL_Vulkan_LoadLibrary(nullptr) != 0 )
			ThrowSDLError("Error initializing SDL Vulkan : ");
	}	
#endif

	m_window = std::make_unique<Window>(m_config);

	m_audioSystem.Init();
	m_resourceManager.Init(&m_audioSystem);

	m_eventHandler.Init(m_config.width, m_config.height);

	m_screenManager.Init(this);

	m_limiter.SetMaxFPS(m_config.MaxFPS);

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
#if RENDER_VULKAN
	if( m_config.renderType == RenderType::Vulkan )
		SDL_Vulkan_UnloadLibrary();
#endif
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

#if RENDER_VULKAN
	if ( m_config.renderType == RenderType::Vulkan )
		m_render = std::make_unique<VKRenderer>(m_window);
#endif

		
	bool run = true;
	while( run && m_isRunning )
	{
		m_limiter.Begin();
		m_eventHandler.Update(m_limiter.GetFPS());
		update();
		run = handleEvents();
		draw();
		m_limiter.End();
		m_window->SwapBuffer();
	}
}
//-----------------------------------------------------------------------------
void Application::OnSDLEvent(SDL_Event &ev)
{
	if ( m_eventHandler.GetPlayWith() == PlayWith::KEYBOARD )
	{
		switch ( ev.type )
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
		switch ( ev.type )
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
void Application::update()
{
	if ( m_currentScreen )
	{
		switch ( m_currentScreen->GetGameState() )
		{
		case ScreenState::RUNNING:
			m_currentScreen->Update();
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
	//m_render->Draw();

	if ( m_currentScreen && m_currentScreen->GetGameState() == ScreenState::RUNNING )
	{
		glViewport(0, 0, m_window->GetScreenWidth(), m_window->GetScreenHeight());
		m_currentScreen->Draw();
	}
}
//-----------------------------------------------------------------------------
bool Application::handleEvents()
{
	//SDL_Event event;
	//while( SDL_PollEvent(&event) != NULL )
	//{
	//	if( event.type == SDL_QUIT )
	//		return false;
	//	//if( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED )
	//	//	m_render->Resize();
	//}

	return true;
}

//-----------------------------------------------------------------------------