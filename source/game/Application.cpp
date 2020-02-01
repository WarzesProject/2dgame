#include "stdafx.h"
#include "Application.h"
//-----------------------------------------------------------------------------
Application::Application(const ApplicationConfig &config)
	: m_config(config)
{
	if( SDL_Init(SDL_INIT_VIDEO) != 0 )
		ThrowSDLError("Error initializing SDL : ");

	m_window = SDL_CreateWindow(m_config.title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_config.width, m_config.height, SDL_WINDOW_SHOWN);
	if( !m_window )
		ThrowSDLError("Can't create window: ");
}
//-----------------------------------------------------------------------------
Application::~Application()
{
	SDL_DestroyWindow(m_window);
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