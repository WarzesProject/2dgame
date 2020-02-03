#pragma once

#include "ApplicationConfig.h"

class Window
{
public:
	Window(ApplicationConfig &config);
	~Window();

	void SwapBuffer();

	int GetScreenWidth()
	{
		return m_config.width;
	}
	int GetScreenHeight()
	{
		return m_config.height;
	}
	SDL_Window* GetSDLWindow() const
	{
		return m_window;
	}
private:
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	SDL_Window *m_window = nullptr;
	ApplicationConfig &m_config;
};