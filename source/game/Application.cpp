#include "stdafx.h"
#include "Application.h"

//-----------------------------------------------------------------------------
Application::Application(const ApplicationConfig &config)
	: m_config(config)
{
	// консоль всегда включается в _DEBUG
#if _DEBUG
	m_config.app.enableConsole = true;	
#endif
	// установка макс fps
	m_limiter.SetMaxFPS(m_config.graphics.maxFPS);

	// отображение консоли
	if (m_config.app.enableConsole )
		redirectIOToConsole();

	m_window = std::make_unique<Window>(m_config.window);
	m_audioSystem = std::make_shared<AudioSystem>(m_config.audio);
	m_resourceManager = std::make_unique<ResourceManager>(m_config.resources, m_audioSystem);
	m_eventHandler = std::make_shared<InputSystem>(m_config.window.width, m_config.window.height);

	m_isRunning = true;
	sApp = this;
}
//-----------------------------------------------------------------------------
Application::~Application()
{
	m_gameApp->privateClose();
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
	m_isRunning = m_gameApp->privateInit(this);

	while( m_isRunning )
	{
		m_limiter.Begin();
		Update();
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
	m_gameApp->privateDraw();
}
//-----------------------------------------------------------------------------
void Application::Update()
{
	m_eventHandler->Update(m_limiter.GetFPS());

	SDL_Event ev;
	while( SDL_PollEvent(&ev) )
		onSDLEvent(ev);

	m_isRunning = m_gameApp->privateUpdate();
}

//-----------------------------------------------------------------------------
void Application::onSDLEvent(const SDL_Event &ev)
{
	if( onSDLEventQuit(ev) )
		return;

	if( m_eventHandler->GetPlayWith() == PlayWith::KEYBOARD )
		onSDLEventKeyboard(ev);
	else
		onSDLEventGamepad(ev);
}
//-----------------------------------------------------------------------------
bool Application::onSDLEventQuit(const SDL_Event &ev)
{
	if( ev.type == SDL_QUIT )
	{
		m_isRunning = !m_gameApp->privateQuit();
		return true;
	}

	return false;
}
//-----------------------------------------------------------------------------
void Application::onSDLEventKeyboard(const SDL_Event &ev)
{
	switch( ev.type )
	{
	case SDL_MOUSEMOTION:
		m_eventHandler->SetMouseCoords((float)ev.motion.x, (float)ev.motion.y);
		break;
	case SDL_KEYDOWN:
		m_eventHandler->PressKey((uint32_t)ev.key.keysym.sym);
		break;
	case SDL_KEYUP:
		m_eventHandler->ReleaseKey((uint32_t)ev.key.keysym.sym);
		break;
	case SDL_MOUSEBUTTONDOWN:
		m_eventHandler->PressKey(ev.button.button);
		break;
	case SDL_MOUSEBUTTONUP:
		m_eventHandler->ReleaseKey(ev.button.button);
		break;
	}
}
//-----------------------------------------------------------------------------
void Application::onSDLEventGamepad(const SDL_Event &ev)
{
	switch( ev.type )
	{
	case SDL_JOYBUTTONDOWN:
		m_eventHandler->PressKey((uint32_t)ev.jbutton.button + MARGIN);
		break;
	case SDL_JOYBUTTONUP:
		m_eventHandler->ReleaseKey((uint32_t)ev.jbutton.button + MARGIN);
		break;
	case SDL_JOYAXISMOTION:
		m_eventHandler->UpdateJoystickAxis(ev.jaxis.axis, ev.jaxis.value, m_window->GetSDLWindow());
		break;
	case SDL_JOYHATMOTION:
		m_eventHandler->UpdateJoystickHats((uint32_t)ev.jhat.value + MARGIN);
		break;
	}
}
//-----------------------------------------------------------------------------