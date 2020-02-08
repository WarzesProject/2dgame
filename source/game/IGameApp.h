#pragma once

#include "GameViewManager.h"
#include "IGameView.h"

class __declspec(novtable) IGameApp
{
	friend class Application;
public:
	virtual ~IGameApp() = default;

	virtual void Init() = 0;

protected:
	Application *m_app = nullptr;
	GameViewManager m_viewManager;
	IGameView *m_currentView = nullptr;

private:
	bool privateInit(Application *app);
	void privateDraw();
	bool privateUpdate();
	bool privateQuit();
	void privateClose();
};