#pragma once

#include "DefineLogic.h"

enum class ItemInfo
{
	ITEM = 0,
	WEAPON
};

enum class ItemType
{
	HP,
	Attack,
	Speed
};

class Item
{
public:
	virtual ~Item() = default;

	virtual void Init(const glm::vec2 &position) = 0;
	virtual void Destroy() = 0;

	void Draw(SpriteBatch &spriteBatch);
	void DrawDebug(DebugRenderer &debuger);

	glm::vec2 GetSize() const
	{
		return m_size;
	}
	glm::vec2& GetPosition()
	{
		return m_position;
	}
	bool IsDisappeared()
	{
		return m_isDisappeared;
	}
	bool IsTaken()
	{
		return m_startDisappear;
	}
	ItemInfo GetItemInfo()
	{
		return m_info;
	}

	//For Items
	int GetEffects()
	{
		return m_effects;
	}
	int GetEffectiveTime()
	{
		return m_effectiveTime;
	}
	ItemType GetItemType()
	{
		return m_type;
	}

	//For Weapons
	std::string GetAttackPath()
	{
		return m_attackTexturePath;
	}
	std::string GetSoundPath()
	{
		return m_soundPath;
	}
	int GetAttackPoint()
	{
		return m_attack;
	}
	glm::vec2 GetAttackingArea()
	{
		return m_attackingArea;
	}

protected:
	glm::vec2 m_size = glm::vec2(ITEM_SIZE);
	int m_alpha = 255;
	bool m_isDisappeared = false;
	bool m_startDisappear = false;
	ItemInfo m_info = ItemInfo::ITEM;
	glm::vec2 m_position = glm::vec2(0.0f);
	Texture m_texture;
	ColorRGBA8 m_color = WhiteColor;

	//For Item
	int m_effects = 0;
	int m_effectiveTime = 0;
	ItemType m_type = ItemType::HP;

	//For Weapon
	std::string m_attackTexturePath;
	std::string m_soundPath;
	glm::vec2 m_attackingArea = glm::vec2(0);
	int m_attack = 0;
};

class SpeedPotion : public Item
{
public:
	virtual void Init(const glm::vec2 &position) override;
	virtual void Destroy() override;
};

class AttPotion : public Item
{
public:
	virtual void Init(const glm::vec2 &position) override;
	virtual void Destroy() override;
};

class BigPotion : public Item
{
public:
	virtual void Init(const glm::vec2 &position) override;
	virtual void Destroy() override;
};

class SmallPotion : public Item
{
public:
	virtual void Init(const glm::vec2 &position) override;
	virtual void Destroy() override;
};

class Mace : public Item
{
public:
	virtual void Init(const glm::vec2 &position) override;
	virtual void Destroy() override;
};