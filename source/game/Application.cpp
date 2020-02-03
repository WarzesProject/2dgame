#include "stdafx.h"
#include "Application.h"
#if RENDER_VULKAN
#	include "VKRenderer.h"
#endif
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

#if RENDER_VULKAN
	if( m_config.renderType == RenderType::Vulkan )
		m_render = std::make_unique<VKRenderer>(m_window);
#endif
}
//-----------------------------------------------------------------------------
Application::~Application()
{
	m_eventHandler.destroy();
	m_resourceManager.destroy();
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
		
	bool run = true;
	while( run )
	{
		run = handleEvents();

		//m_render->Draw();
		m_window->SwapBuffer();
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

		//if( event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED )
		//	m_render->Resize();
	}

	return true;
}

//-----------------------------------------------------------------------------