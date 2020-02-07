#pragma once

#include "ApplicationConfig.h"

class Window
{
public:
	Window(ApplicationConfig::Window &config);
	~Window();

	void SwapBuffer();

	SDL_Window* GetSDLWindow() const { return m_window; }

private:
	Window() = delete;
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;

	SDL_Window *m_window = nullptr;
	ApplicationConfig::Window &m_config;
};