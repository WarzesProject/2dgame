#pragma once

#include "IGame.h"
#include "ApplicationConfig.h"
#include "Renderer.h"
#include "Window.h"
#include "AudioEngine.h"
#include "ResourceManager.h"
#include "EventHandler.h"
#include "ScreenManager.h"
#include "FPSLimiter.h"

class Application
{
public:
	Application(const ApplicationConfig &config);
	~Application();

	void Run(std::unique_ptr<IGame> game);

	void OnSDLEvent(SDL_Event &ev);

private:
	Application() = delete;
	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	void update();
	void draw();
	bool handleEvents();

	ApplicationConfig m_config;
	
	std::unique_ptr<Renderer> m_render = nullptr;
	std::unique_ptr<Window> m_window = nullptr;
	std::unique_ptr<IGame> m_game = nullptr;	
	AudioEngine m_audioSystem;
	ResourceManager m_resourceManager;
	EventHandler m_eventHandler;
	ScreenManager m_screenManager;
	IGameScreen* m_currentScreen = nullptr;
	FPSLimiter m_limiter;
	bool m_isRunning = false;
};