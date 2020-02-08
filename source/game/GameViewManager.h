#pragma once

constexpr auto VIEW_INDEX_NO_VIEW = -1;

class IGameView;
class Application;

class GameViewManager
{
public:
	void Init(Application *app);
	void Destroy();

	IGameView* MovePrevious();
	IGameView* MoveNext();
	

	void AddGameView(IGameView *view);
	void SetGameView(int screenIndex);
	IGameView* GetCurrentGameView();

private:
	Application *m_app = nullptr;
	std::vector<IGameView*> m_views;
	int m_currentViewIndex = VIEW_INDEX_NO_VIEW;
};