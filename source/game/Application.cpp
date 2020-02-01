#include "stdafx.h"
#include "Application.h"
#include "VKRenderer.h"
//-----------------------------------------------------------------------------
struct WindowsConstants
{
	const float DefaultDotsPerInch = 96.0f;
} PlatformConstants;
//-----------------------------------------------------------------------------
Application::Application(const ApplicationConfig &config)
	: m_config(config)
{
	if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0 )
		ThrowSDLError("Fail to Initialize SDL: ");

	if( SDL_Vulkan_LoadLibrary(nullptr) != 0 )
		ThrowSDLError("Error initializing SDL Vulkan : ");

	const int windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN; // SDL_WINDOW_FULLSCREEN SDL_WINDOW_RESIZABLE SDL_WINDOW_ALLOW_HIGHDPI
	m_window = SDL_CreateWindow(m_config.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_config.width, m_config.height, windowFlags);
	if( !m_window )
		ThrowSDLError("Fail to Create Vulkan-compatible Window with SDL: ");

	recordWindowSize();

	m_render = std::make_unique<VKRenderer>(m_window);
}
//-----------------------------------------------------------------------------
Application::~Application()
{
	m_render.reset();
	SDL_DestroyWindow(m_window);
	SDL_Vulkan_UnloadLibrary();
	SDL_Quit();
}
//-----------------------------------------------------------------------------
void Application::Run(std::unique_ptr<IGame> game)
{
	m_game = std::move(game);
		
	bool run = true;
	while( run )
	{
		run = handleEvents();		

		SDL_UpdateWindowSurface(m_window);
	}

	m_game.reset();
}
//-----------------------------------------------------------------------------
bool Application::GetWindowSize(int &pixelWidth, int &pixelHeight)
{
	std::string error;

	pixelWidth = pixelHeight = 0;

	auto pRenderer = SDL_GetRenderer(m_window);
	if( pRenderer )
	{
		int result = SDL_GetRendererOutputSize(pRenderer, &pixelWidth, &pixelHeight);
		if( result == 0 )  // success
			return true;
		error = "Renderer Output Size returned error";
	}
	else
		error = "Renderer undefined";
	std::string sdlError = SDL_GetError();
	if( sdlError != "" )
		error += ": " + sdlError;

	// see if possible to return valid values from screen coordinates
	SDL_GetWindowSize(m_window, &pixelWidth, &pixelHeight);	// (doesn't return a result)
	if( pixelWidth > 0 && pixelHeight > 0 )
	{
		float scaling = getDisplayScaling();
		if( scaling > 0.0f )
		{
			pixelWidth *= (int)scaling;
			pixelHeight *= (int)scaling;
		}
		return true;
	}
	error += ", Get Window Size returned " + std::to_string(pixelWidth) + " x " + std::to_string(pixelHeight);
	sdlError = SDL_GetError();
	if( sdlError != "" )
		error += ", " + sdlError;
	SDL_Log(error.c_str());
	return false;
}
//-----------------------------------------------------------------------------
void Application::recordWindowSize()
{
	int tempPixelsWide = 0,
		tempPixelsHigh = 0;
	GetWindowSize(tempPixelsWide, tempPixelsHigh);
	if( tempPixelsWide != m_config.width || tempPixelsHigh != m_config.height )
	{
		SDL_Log("Window Resolution is:  %d x %d", tempPixelsWide, tempPixelsHigh);
		m_config.width = tempPixelsWide;
		m_config.height = tempPixelsHigh;
	}
}
//-----------------------------------------------------------------------------
float Application::getDisplayScaling()
{
	auto noHiDPI = SDL_GetHintBoolean(SDL_HINT_VIDEO_HIGHDPI_DISABLED, SDL_TRUE);
	if( noHiDPI == SDL_TRUE )
		return 0.0f;
	return getDisplayDPI() / PlatformConstants.DefaultDotsPerInch;
}
//-----------------------------------------------------------------------------
float Application::getDisplayDPI(int iDisplay)
{
	float horizontalDPI = 0.0f;
	if( SDL_GetDisplayDPI(iDisplay, nullptr, &horizontalDPI, nullptr) == 0 )
		return horizontalDPI;
	return 0.0f;
}
//-----------------------------------------------------------------------------
bool Application::handleEvents()
{
	SDL_Event e;
	while( SDL_PollEvent(&e) != NULL )
	{
		if( e.type == SDL_QUIT )
			return false;		
	}

	return true;
}

//-----------------------------------------------------------------------------