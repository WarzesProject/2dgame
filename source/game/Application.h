#pragma once

#include "IGameApp.h"
#include "ApplicationConfig.h"
#include "Window.h"
#include "AudioSystem.h"
#include "ResourceManager.h"
#include "InputSystem.h"
#include "FPSLimiter.h"

class Application
{
public:
	Application(const ApplicationConfig &config);
	~Application();

	void Run(std::unique_ptr<IGameApp> game);

	int GetScreenWidth() const { return m_config.window.width; }
	int GetScreenHeight() const { return m_config.window.height; }
	std::shared_ptr<InputSystem> GetEventHandler() { return m_eventHandler; }

private:
	Application() = delete;
	Application(const Application&) = delete;
	Application(Application&&) = delete;
	Application& operator=(const Application&) = delete;
	Application& operator=(Application&&) = delete;

	void redirectIOToConsole();
	void draw();
	void Update();	
	void onSDLEvent(const SDL_Event &ev);
	bool onSDLEventQuit(const SDL_Event &ev);
	void onSDLEventKeyboard(const SDL_Event &ev);
	void onSDLEventGamepad(const SDL_Event &ev);

	ApplicationConfig m_config;
	FPSLimiter m_limiter;
	std::unique_ptr<Window> m_window = nullptr;
	std::shared_ptr<AudioSystem> m_audioSystem = nullptr;
	std::unique_ptr<ResourceManager> m_resourceManager = nullptr;
	std::shared_ptr<InputSystem> m_eventHandler = nullptr;
	std::unique_ptr<IGameApp> m_gameApp = nullptr;

	bool m_isRunning = false;
};