#include "stdafx.h"
#include "GameView.h"
//-----------------------------------------------------------------------------
namespace
{
}
//-----------------------------------------------------------------------------
void GameView::OnEntry()
{
	m_world.Init();
}
//-----------------------------------------------------------------------------
void GameView::OnExit()
{
	m_world.Close();
}
//-----------------------------------------------------------------------------
void GameView::Update()
{
	m_world.Update();
}
//-----------------------------------------------------------------------------
void GameView::Draw()
{
	m_world.Draw();
}
//-----------------------------------------------------------------------------
int GameView::GetNextViewIndex() const
{
	return -1;
}
//-----------------------------------------------------------------------------
int GameView::GetPreviousViewIndex() const
{
	return -1;
}
//-----------------------------------------------------------------------------