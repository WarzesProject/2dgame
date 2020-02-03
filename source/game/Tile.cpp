#include "stdafx.h"
#include "Tile.h"
#include "ResourceManager.h"
//-----------------------------------------------------------------------------
const glm::vec4 Tile::m_uvRect(0.0f, 0.0f, 1.0f, 1.0f);
//-----------------------------------------------------------------------------
Tile::Tile()
{
	Init(ResourceManager::GetTexture("../data/sprite/ground.png"), 0, 0, true);
}
//-----------------------------------------------------------------------------
void Tile::Init(const Texture& texture, int x, int y, bool crossable)
{
	m_texture = texture;
	m_position.x = x;
	m_position.y = y;
	m_destRect.x = x * TILESIZE;
	m_destRect.y = y * TILESIZE;
	m_destRect.z = TILESIZE;
	m_destRect.w = TILESIZE;
	m_crossable = crossable;
	m_color = ColorRGBA8(255, 255, 255, 255);
}
//-----------------------------------------------------------------------------
void Tile::Init(Tile& tile, int x, int y)
{
	Init(tile.m_texture, x, y, tile.m_crossable);
	m_color = tile.m_color;
}
//-----------------------------------------------------------------------------
void Tile::Reset()
{
	m_texture = ResourceManager::GetTexture(".../data/sprite/ground.png");
	m_crossable = true;
	m_color.SetColor(255, 255, 255, 255);
}
//-----------------------------------------------------------------------------
void Tile::SetPosition(int x, int y)
{
	m_position.x = x;
	m_position.y = y;
	m_destRect.x = x * TILESIZE;
	m_destRect.y = y * TILESIZE;
}
//-----------------------------------------------------------------------------
void Tile::Draw(SpriteBatch &spriteBatch)
{
	spriteBatch.Draw(m_destRect, m_uvRect, m_texture.id, 0.0f, m_color);
}
//-----------------------------------------------------------------------------
void Tile::Draw(DebugRenderer &debugRender, ColorRGBA8& color)
{
	debugRender.DrawBox(m_destRect, color, 0.0f);
}
//-----------------------------------------------------------------------------