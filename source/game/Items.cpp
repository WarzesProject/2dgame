#include "stdafx.h"
#include "Items.h"
//-----------------------------------------------------------------------------
void Item::Draw(SpriteBatch &spriteBatch)
{
	const glm::vec4 destRect = { m_position.x, m_position.y, m_size.x, m_size.y };

	if ( m_startDisappear )
	{
		m_alpha -= 20;
		if ( m_alpha <= 0 )
		{
			m_alpha = 0;
			m_isDisappeared = true;
		}
	}
	m_color.a = m_alpha;
	spriteBatch.Draw(destRect, UVRect, m_texture.id, 0.0f, m_color);
}
//-----------------------------------------------------------------------------
void Item::DrawDebug(DebugRenderer &debuger)
{
	const glm::vec4 destRect = { m_position.x, m_position.y, m_size.x, m_size.y };
	debuger.DrawBox(destRect, ColorRGBA8(0, 0, 255, 255), 0);
}
//-----------------------------------------------------------------------------
void SpeedPotion::Init(const glm::vec2 &position)
{
	m_texture = ResourceManager::GetTexture("../test/Assets/Items/potion/sky_blue.png");
	m_position = position;
	m_type = ItemType::Speed;
	m_effects = 1;
	m_effectiveTime = 1000;
}
//-----------------------------------------------------------------------------
void SpeedPotion::Destroy()
{
	m_startDisappear = true;
}
//-----------------------------------------------------------------------------
void AttPotion::Init(const glm::vec2 &position)
{
	m_texture = ResourceManager::GetTexture("../test/Assets/Items/potion/brilliant_blue_new.png");
	m_position = position;
	m_type = ItemType::Attack;
	m_effects = 10;
	m_effectiveTime = 1000;
}
//-----------------------------------------------------------------------------
void AttPotion::Destroy()
{
	m_startDisappear = true;
}
//-----------------------------------------------------------------------------
void BigPotion::Init(const glm::vec2 &position)
{
	m_texture = ResourceManager::GetTexture("../test/Assets/Items/potion/potion.png");
	m_position = position;
	m_type = ItemType::HP;
	m_effects = 100;
}
//-----------------------------------------------------------------------------
void BigPotion::Destroy()
{
	m_startDisappear = true;
}
//-----------------------------------------------------------------------------
void SmallPotion::Init(const glm::vec2 &position)
{
	m_texture = ResourceManager::GetTexture("../test/Assets/Items/potion/potionStick.png");
	m_position = position;
	m_type = ItemType::HP;
	m_effects = 20;
}
//-----------------------------------------------------------------------------
void SmallPotion::Destroy()
{
	m_startDisappear = true;
}
//-----------------------------------------------------------------------------