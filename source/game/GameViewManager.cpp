#include "stdafx.h"
#include "GameViewManager.h"
#include "IGameView.h"
//-----------------------------------------------------------------------------
void GameViewManager::Init(Application *app)
{
	m_app = app;
}
//-----------------------------------------------------------------------------
void GameViewManager::Destroy()
{
	for( auto &i : m_views )
		i->Destroy();
	m_views.clear();

	m_currentViewIndex = VIEW_INDEX_NO_VIEW;
}
//-----------------------------------------------------------------------------
IGameView* GameViewManager::MovePrevious()
{
	auto current = GetCurrentGameView();
	if( current->GetPreviousViewIndex() != VIEW_INDEX_NO_VIEW )
		m_currentViewIndex = current->GetPreviousViewIndex();
	return GetCurrentGameView();
}
//-----------------------------------------------------------------------------
IGameView* GameViewManager::MoveNext()
{
	auto currentScreen = GetCurrentGameView();
	if( currentScreen->GetNextViewIndex() != VIEW_INDEX_NO_VIEW )
		m_currentViewIndex = currentScreen->GetNextViewIndex();
	return GetCurrentGameView();
}
//-----------------------------------------------------------------------------
void GameViewManager::AddGameView(IGameView *view)
{
	view->Build();
	view->SetApplication(m_app);
	view->SetViewIndex((int)m_views.size());
	m_views.push_back(view);
}
//-----------------------------------------------------------------------------
void GameViewManager::SetGameView(int screenIndex)
{
	m_currentViewIndex = screenIndex;
}
//-----------------------------------------------------------------------------
IGameView* GameViewManager::GetCurrentGameView()
{
	if( m_currentViewIndex != VIEW_INDEX_NO_VIEW )
		return m_views[(size_t)m_currentViewIndex];
	else
		return nullptr;
}
//-----------------------------------------------------------------------------