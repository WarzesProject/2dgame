#include "stdafx.h"
#include "Application.h"
#include "VKRenderer.h"
//-----------------------------------------------------------------------------
Application::Application(const ApplicationConfig &config)
	: m_config(config)
{
	if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0 )
		ThrowSDLError("Fail to Initialize SDL: ");

	if( m_config.renderType == RenderType::Vulkan )
	{
		if( SDL_Vulkan_LoadLibrary(nullptr) != 0 )
			ThrowSDLError("Error initializing SDL Vulkan : ");
	}	

	int windowFlags = SDL_WINDOW_SHOWN;
	if( m_config.renderType == RenderType::Vulkan )
		windowFlags |= SDL_WINDOW_VULKAN;
	//windowFlags |= SDL_WINDOW_FULLSCREEN;
	//windowFlags |= SDL_WINDOW_RESIZABLE;
	//windowFlags |= SDL_WINDOW_ALLOW_HIGHDPI;


	m_window = SDL_CreateWindow(m_config.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_config.width, m_config.height, windowFlags);
	if( !m_window )
		ThrowSDLError("Fail to Create Window with SDL: ");

	if( m_config.renderType == RenderType::Vulkan )
		m_render = std::make_unique<VKRenderer>(m_window);
}
//-----------------------------------------------------------------------------
Application::~Application()
{
	m_render.reset();
	SDL_DestroyWindow(m_window);
	if( m_config.renderType == RenderType::Vulkan )
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

		m_render->Draw();
		SDL_UpdateWindowSurface(m_window);
	}

	m_game.reset();
}
//-----------------------------------------------------------------------------
bool Application::handleEvents()
{
	SDL_Event event;
	while( SDL_PollEvent(&event) != NULL )
	{
		if( event.type == SDL_QUIT )
			return false;	

		if( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED )
			m_render->Resize();
	}

	return true;
}

//-----------------------------------------------------------------------------