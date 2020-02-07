#pragma once

#include "oldIGame.h"
#include "IGameApp.h"
#include "ApplicationConfig.h"
#include "Renderer.h"
#include "Window.h"
#include "AudioEngine.h"
#include "ResourceManager.h"
#include "EventHandler.h"
#include "oldScreenManager.h"
#include "FPSLimiter.h"

class Application
{
public:
	Application(const ApplicationConfig &config);
	~Application();

	void Run(std::unique_ptr<IGameApp> game);

	TODO("удалить GetScreenWidth/GetScreenHeight/GetEventHandler");
	int GetScreenWidth() const { return m_config.window.width; }
	int GetScreenHeight() const { return m_config.window.height; }
	std::shared_ptr<EventHandler> GetEventHandler() { return m_eventHandler; }

private:
	Application() = delete;
	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	void redirectIOToConsole();
	void draw();
	void update();	
	void onSDLEvent(SDL_Event &ev);

	ApplicationConfig m_config;
	FPSLimiter m_limiter;
	std::unique_ptr<Window> m_window = nullptr;
	std::shared_ptr<AudioEngine> m_audioSystem = nullptr;
	std::unique_ptr<ResourceManager> m_resourceManager = nullptr;
	std::shared_ptr<EventHandler> m_eventHandler = nullptr;
	std::unique_ptr<IGameApp> m_gameApp = nullptr;

	bool m_isRunning = false;




	oldScreenManager m_screenManager;
	oldIGameScreen *m_currentScreen = nullptr;	
};