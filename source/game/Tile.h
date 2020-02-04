#pragma once

#include "SpriteBatch.h"
#include "Texture.h"
#include "DebugRenderer.h"

constexpr auto TILESIZE = 32.0f;

class Tile
{
public:
	Tile();

private:




public:
	void Init(const Texture& texture, int x, int y, bool crossable);
	void Init(Tile& tile, int x, int y);
	void Reset();
	virtual void Draw(SpriteBatch& spriteBatch);
	virtual void Draw(DebugRenderer& debugRender, ColorRGBA8& color);
	virtual bool IsCrossable() const { return m_crossable; }

	void SetColor(ColorRGBA8& color) { m_color = color; }
	void SetCrossable(bool crossable) { m_crossable = crossable; }
	void SetTexture(Texture& texture) { m_texture = texture; }
	void SetPosition(int x, int y);
	void UpdatePosition(glm::vec2& pos) { m_destRect.x = pos.x; m_destRect.y = pos.y; }
	void SetCenter(glm::vec2& center)
	{
		center.x = m_destRect.x + (TILESIZE / 2.0f); center.y = m_destRect.y + (TILESIZE / 2.0f);
	}

	int GetX() { return m_position.x; }
	int GetY() { return m_position.y; }
	glm::vec2 getCenter()
	{
		return glm::vec2(m_destRect.x + (TILESIZE / 2.0f), m_destRect.y + (TILESIZE / 2.0f));
	}
	const glm::ivec2& getPosition() const { return m_position; }
	const glm::vec4& getDestRect() const { return m_destRect; }
	ColorRGBA8& getColor() { return m_color; }
	Texture& getTexture() { return m_texture; }

protected:
	bool m_crossable;
	glm::vec4 m_destRect;
	glm::ivec2 m_position;
	static const glm::vec4 m_uvRect;
	ColorRGBA8 m_color;
	Texture m_texture;
};