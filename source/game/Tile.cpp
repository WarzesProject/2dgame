#include "stdafx.h"
#include "Tile.h"
#include "ResourceManager.h"
//-----------------------------------------------------------------------------
constexpr const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
//-----------------------------------------------------------------------------
Tile::Tile()
{
	Init(ResourceManager::GetTexture("../data/sprite/no_sprite.png"), 0, 0, true);
}
//-----------------------------------------------------------------------------
void Tile::Init(const Tile& tile, int x, int y)
{
	Init(tile.m_texture, x, y, tile.m_crossable);
	m_color = tile.m_color;
}
//-----------------------------------------------------------------------------
void Tile::Init(const Texture &texture, int x, int y, bool crossable)
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
void Tile::Reset()
{
	m_texture = ResourceManager::GetTexture("../data/sprite/no_sprite.png");
	m_crossable = true;
	m_color.SetColor(255, 255, 255, 255);
}
//-----------------------------------------------------------------------------
void Tile::Draw(SpriteBatch &spriteBatch)
{
	spriteBatch.Draw(m_destRect, uvRect, m_texture.id, 0.0f, m_color);
}
//-----------------------------------------------------------------------------
void Tile::Draw(DebugRenderer &debugRender, const ColorRGBA8 &color)
{
	debugRender.DrawBox(m_destRect, color, 0.0f);
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