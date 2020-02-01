#include "stdafx.h"
#include "Application.h"
//-----------------------------------------------------------------------------
Application::Application()
{
	if( SDL_Init(SDL_INIT_VIDEO) != 0 )
		ThrowSDLError("Can't init: ");

	m_window = SDL_CreateWindow("Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	if( !m_window )
		ThrowSDLError("Can't create window: ");

	m_surface = SDL_GetWindowSurface(m_window);
	
	m_hero = SDL_LoadBMP("../data/sprite/hero.bmp");
	if( !m_hero )
		ThrowSDLError("Can't load image: ");
}
//-----------------------------------------------------------------------------
Application::~Application()
{
	SDL_FreeSurface(m_hero);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}
//-----------------------------------------------------------------------------
void Application::Run()
{
	SDL_Event e;
	bool run = true;
	while( run )
	{
		while( SDL_PollEvent(&e) != NULL )
		{
			if( e.type == SDL_QUIT )
			{
				run = false;
				break;
			}

			if( e.type == SDL_KEYDOWN )
			{
				if( e.key.keysym.sym == SDLK_UP )
					m_pos.y -= 1;
				if( e.key.keysym.sym == SDLK_DOWN )
					m_pos.y += 1;
				if( e.key.keysym.sym == SDLK_RIGHT )
					m_pos.x += 1;
				if( e.key.keysym.sym == SDLK_LEFT )
					m_pos.x -= 1;
			}
		}

		SDL_FillRect(m_surface, NULL, SDL_MapRGB(m_surface->format, 108, 120, 255));
		SDL_BlitSurface(m_hero, NULL, m_surface, &m_pos);
		SDL_UpdateWindowSurface(m_window);
	}
}
//-----------------------------------------------------------------------------