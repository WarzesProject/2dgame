#include "stdafx.h"
#include "Tile.h"
#include "ResourceManager.h"

const glm::vec4 Tile::m_uvRect(0.0f, 0.0f, 1.0f, 1.0f);

Tile::Tile()
{
	init(ResourceManager::GetTexture("../Assets/ground.png"), 0, 0, true);
}

void Tile::init(const Texture& texture, int x, int y, bool crossable)
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

void Tile::init(Tile& tile, int x, int y)
{
	init(tile.m_texture, x, y, tile.m_crossable);
	m_color = tile.m_color;
}

void Tile::reset()
{
	m_texture = ResourceManager::GetTexture("../Assets/ground.png");
	m_crossable = true;
	m_color.SetColor(255, 255, 255, 255);
}

void Tile::setPosition(int x, int y)
{
	m_position.x = x;
	m_position.y = y;
	m_destRect.x = x * TILESIZE;
	m_destRect.y = y * TILESIZE;
}

void Tile::draw(SpriteBatch& spriteBatch)
{
	spriteBatch.Draw(m_destRect, m_uvRect, m_texture.id, 0.0f, m_color);
}

void Tile::draw(DebugRenderer& debugRender, ColorRGBA8& color)
{
	debugRender.DrawBox(m_destRect, color, 0.0f);
}