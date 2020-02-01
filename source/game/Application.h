#pragma once

#include "IGame.h"
#include "ApplicationConfig.h"

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

	std::unique_ptr<IGame> m_game = nullptr;
	ApplicationConfig m_config;
	SDL_Window *m_window = nullptr;
};