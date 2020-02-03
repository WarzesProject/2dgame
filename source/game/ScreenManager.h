#pragma once

#define SCREEN_INDEX_NO_SCREEN -1

class IGameScreen;
class Application;

class ScreenManager
{
public:
	IGameScreen* MoveNext();
	IGameScreen* MovePrevious();

	void AddScreen(IGameScreen *screen);

	void SetScreen(int screenIndex);

	void Init(Application* game);

	void Destroy();

	IGameScreen* GetCurrentScreen();

protected:
	Application *m_app = nullptr;
	std::vector<IGameScreen*> m_screens;
	int m_currentScreenIndex = SCREEN_INDEX_NO_SCREEN;
};