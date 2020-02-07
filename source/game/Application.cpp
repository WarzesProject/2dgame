#include "stdafx.h"
#include "Application.h"
#include "oldIGameScreen.h"

//-----------------------------------------------------------------------------
Application::Application(const ApplicationConfig &config)
	: m_config(config)
{
	// консоль всегда включается в _DEBUG
#if _DEBUG
	m_config.app.enableConsole = true;	
#endif
	// установка макс fps
	TODO("удалить");
	m_limiter.SetMaxFPS(m_config.graphics.maxFPS);

	// отображение консоли
	if (m_config.app.enableConsole )
		redirectIOToConsole();

	m_window = std::make_unique<Window>(m_config.window);
	m_audioSystem = std::make_shared<AudioEngine>(m_config.audio);
	m_resourceManager = std::make_unique<ResourceManager>(m_config.resources, m_audioSystem);
	m_eventHandler = std::make_shared<EventHandler>(m_config.window.width, m_config.window.height);

	//m_screenManager.Init(this);

	m_isRunning = true;
}
//-----------------------------------------------------------------------------
Application::~Application()
{
	//if ( m_currentScreen )
	//	m_currentScreen->OnExit();
	//m_screenManager.Destroy();
	m_gameApp.reset();
	m_eventHandler.reset();
	m_resourceManager.reset();
	m_audioSystem.reset();
	m_window.reset();
}
//-----------------------------------------------------------------------------
void Application::Run(std::unique_ptr<IGameApp> game)
{
	m_gameApp = std::move(game);

	/*m_game->OnInit(&m_screenManager, &m_eventHandler);
	m_currentScreen = m_screenManager.GetCurrentScreen();
	m_currentScreen->OnEntry();
	m_currentScreen->SetGameState(ScreenState::RUNNING);*/

	while( m_isRunning )
	{
		m_limiter.Begin();
		m_eventHandler->Update(m_limiter.GetFPS());
		update();
		draw();
		m_limiter.End();
		m_window->SwapBuffer();
	}
}
//-----------------------------------------------------------------------------
// https://stackoverflow.com/questions/191842/how-do-i-get-console-output-in-c-with-a-windows-program
// post - Sev
void Application::redirectIOToConsole()
{
#if _WIN32
	//Create a console for this application
	AllocConsole();

	// Get STDOUT handle
	HANDLE ConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	int SystemOutput = _open_osfhandle(intptr_t(ConsoleOutput), _O_TEXT);
	FILE *COutputHandle = _fdopen(SystemOutput, "w");

	// Get STDERR handle
	HANDLE ConsoleError = GetStdHandle(STD_ERROR_HANDLE);
	int SystemError = _open_osfhandle(intptr_t(ConsoleError), _O_TEXT);
	FILE *CErrorHandle = _fdopen(SystemError, "w");

	// Get STDIN handle
	HANDLE ConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	int SystemInput = _open_osfhandle(intptr_t(ConsoleInput), _O_TEXT);
	FILE *CInputHandle = _fdopen(SystemInput, "r");

	//make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog point to console as well
	std::ios::sync_with_stdio(true);

	// Redirect the CRT standard input, output, and error handles to the console
	freopen_s(&CInputHandle, "CONIN$", "r", stdin);
	freopen_s(&COutputHandle, "CONOUT$", "w", stdout);
	freopen_s(&CErrorHandle, "CONOUT$", "w", stderr);

	//Clear the error state for each of the C++ standard stream objects. We need to do this, as
	//attempts to access the standard streams before they refer to a valid target will cause the
	//iostream objects to enter an error state. In versions of Visual Studio after 2005, this seems
	//to always occur during startup regardless of whether anything has been read from or written to
	//the console or not.
	std::wcout.clear();
	std::cout.clear();
	std::wcerr.clear();
	std::cerr.clear();
	std::wcin.clear();
	std::cin.clear();
#endif
}
//-----------------------------------------------------------------------------
void Application::draw()
{
	//if ( m_currentScreen && m_currentScreen->GetGameState() == ScreenState::RUNNING )
	//{
	glViewport(0, 0, m_config.window.width, m_config.window.height);
	//	m_currentScreen->Draw();
	//}
}
//-----------------------------------------------------------------------------
void Application::update()
{
	SDL_Event ev;

	//if ( m_currentScreen )
	//{
	//	switch ( m_currentScreen->GetGameState() )
	//	{
	//	case ScreenState::RUNNING:
			{
				while ( SDL_PollEvent(&ev) )
					onSDLEvent(ev);
	//			m_currentScreen->Update();
			}
	//		break;
	//	case ScreenState::CHANGE_NEXT:
	//		m_currentScreen->OnExit();
	//		m_currentScreen = m_screenManager.MoveNext();
	//		if ( m_currentScreen )
	//		{
	//			m_currentScreen->SetGameState(ScreenState::RUNNING);
	//			m_currentScreen->OnEntry();
	//		}
	//		break;
	//	case ScreenState::CHANGE_PREVIOUS:
	//		m_currentScreen->OnExit();
	//		m_currentScreen = m_screenManager.MovePrevious();
	//		if ( m_currentScreen )
	//		{
	//			m_currentScreen->OnEntry();
	//			m_currentScreen->SetGameState(ScreenState::RUNNING);
	//		}
	//		break;
	//	case ScreenState::EXIT_GAME:
	//		m_isRunning = false;
	//		break;
	//	default:
	//		break;
	//	}
	//}
	//else
	//{
	//	m_isRunning = false;
	//}
}

//-----------------------------------------------------------------------------
void Application::onSDLEvent(SDL_Event &ev)
{
	if ( ev.type == SDL_QUIT )
	{
		//m_currentScreen->SetGameState(ScreenState::CHANGE_PREVIOUS);
		return;
	}

	if( m_eventHandler->GetPlayWith() == PlayWith::KEYBOARD )
	{
		switch( ev.type )
		{
		case SDL_MOUSEMOTION:
			m_eventHandler->SetMouseCoords((float)ev.motion.x, (float)ev.motion.y);
			break;
		case SDL_KEYDOWN:
			m_eventHandler->PressKey(ev.key.keysym.sym);
			break;
		case SDL_KEYUP:
			m_eventHandler->ReleaseKey(ev.key.keysym.sym);
			break;
		case SDL_MOUSEBUTTONDOWN:
			m_eventHandler->PressKey(ev.button.button);
			break;
		case SDL_MOUSEBUTTONUP:
			m_eventHandler->ReleaseKey(ev.button.button);
			break;
		}
	}
	else
	{
		switch( ev.type )
		{
		case SDL_JOYBUTTONDOWN:
			m_eventHandler->PressKey(ev.jbutton.button + MARGIN);
			break;
		case SDL_JOYBUTTONUP:
			m_eventHandler->ReleaseKey(ev.jbutton.button + MARGIN);
			break;
		case SDL_JOYAXISMOTION:
			m_eventHandler->UpdateJoystickAxis(ev.jaxis.axis, ev.jaxis.value, m_window->GetSDLWindow());
			break;
		case SDL_JOYHATMOTION:
			m_eventHandler->UpdateJoystickHats(ev.jhat.value + MARGIN);
			break;
		}
	}
}
//-----------------------------------------------------------------------------