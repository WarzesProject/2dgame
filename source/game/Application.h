#pragma once

#include "IGame.h"
#include "ApplicationConfig.h"
#include "Renderer.h"

class Application
{
public:
	Application(const ApplicationConfig &config);
	~Application();

	void Run(std::unique_ptr<IGame> game);

private:
	Application() = delete;
	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	bool handleEvents();

	ApplicationConfig m_config;
	SDL_Window *m_window = nullptr;
	std::unique_ptr<Renderer> m_render = nullptr;
	std::unique_ptr<IGame> m_game = nullptr;	
};