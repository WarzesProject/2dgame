#pragma once

class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	SDL_Window *m_window = nullptr;
	SDL_Surface *m_surface = nullptr;

	SDL_Surface *m_hero = nullptr;
	SDL_Rect m_pos = { 0,0,0,0 };
};