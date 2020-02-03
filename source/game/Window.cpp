#include "stdafx.h"
#include "Window.h"
#if RENDER_OPENGL
#	pragma warning( push, 1 )
#	include <glad/glad.c>
#	pragma warning( pop )
#endif
//-----------------------------------------------------------------------------
Window::Window(ApplicationConfig &config)
	: m_config(config)
{
#if RENDER_OPENGL
	if ( m_config.renderType == RenderType::OpenGL )
	{
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	}
#endif

	Uint32 windowFlags = SDL_WINDOW_SHOWN;
#if RENDER_VULKAN
	if ( m_config.renderType == RenderType::Vulkan )
		windowFlags |= SDL_WINDOW_VULKAN;
#endif
#if RENDER_OPENGL
	if ( m_config.renderType == RenderType::OpenGL )
		windowFlags |= SDL_WINDOW_OPENGL;
#endif

	if ( m_config.fullscreen )
	{
		if (m_config.windowedFullscreen )
			windowFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		else
			windowFlags |= SDL_WINDOW_FULLSCREEN;
	}
	if ( m_config.resizable )
		windowFlags |= SDL_WINDOW_RESIZABLE;
	if ( m_config.highDPI )
		windowFlags |= SDL_WINDOW_ALLOW_HIGHDPI;

	m_window = SDL_CreateWindow(m_config.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_config.width, m_config.height, windowFlags);
	if ( !m_window )
		ThrowSDLError("Fail to Create Window with SDL: ");

#if RENDER_OPENGL
	if ( m_config.renderType == RenderType::OpenGL )
	{
		SDL_GLContext glContext = SDL_GL_CreateContext(m_window);
		if ( !glContext )
			ThrowSDLError("SDL_GL context could not be created! ");

		if ( !gladLoadGL() )
			Throw("Could not initialize Glad!");

		SDL_Log("OpenGL %d.%d", GLVersion.major, GLVersion.minor);

		SDL_GL_SetSwapInterval(0);

		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClearDepth(1.0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
#endif

	SDL_GetWindowSize(m_window, &m_config.width, &m_config.height);
}
//-----------------------------------------------------------------------------
Window::~Window()
{
	SDL_DestroyWindow(m_window);
}
//-----------------------------------------------------------------------------
void Window::SwapBuffer()
{
#if RENDER_VULKAN
	if ( m_config.renderType == RenderType::Vulkan )
		SDL_UpdateWindowSurface(m_window);
#endif
#if RENDER_OPENGL
	if ( m_config.renderType == RenderType::OpenGL )
		SDL_GL_SwapWindow(m_window);
#endif
}
//-----------------------------------------------------------------------------