#include "stdafx.h"
#include "World.h"
//-----------------------------------------------------------------------------
void World::Init()
{
}
//-----------------------------------------------------------------------------
void World::Close()
{
}
//-----------------------------------------------------------------------------
void World::Update()
{
	if ( !m_pause ) return;
}
//-----------------------------------------------------------------------------
void World::Draw()
{
	if ( !m_isRender ) return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
//-----------------------------------------------------------------------------