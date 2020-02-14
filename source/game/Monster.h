#pragma once

#include "DefineLogic.h"
#include "Items.h"

enum class MonsterState
{
	ATTACKING,
	ATTAKCED,
	NORMAL,
	WAITING
};

class Monster
{
public:
	Monster() = default;
	virtual ~Monster() = default;

	virtual glm::vec4 Animation() = 0;
	virtual void Init(int level, float speed, const glm::vec2 &position, int movementCount = 1) = 0;
	virtual void Destroy() = 0;
	virtual void Update(const std::vector<std::string> &levelData, const glm::vec2 &playerPosition) = 0;

	void Draw(SpriteBatch &spriteBatch);
	void DrawDebug(DebugRenderer &debuger);

	bool TakeDamage(int damage)
	{
		if ( m_startDeathAnimation )
			return true;

		m_currentHitPoint -= damage;
		if ( m_currentHitPoint <= 0 )
		{
			// Monster died;
			m_currentHitPoint = 0;
			Destroy();
			return true;
		}
		return false;
	}

	bool CollideWithLevel(const std::vector<std::string> &levelData)
	{
		std::vector<glm::vec2> collideTilePosition;

		//check the four corners
		//first corner
		checkTilePosition(levelData, collideTilePosition, m_position.x + m_substructDims.x, m_position.y + m_substructDims.y);

		//Second corner
		checkTilePosition(levelData, collideTilePosition, m_position.x + m_size.x + m_substructDims.x, m_position.y + m_substructDims.y);

		//Third corner
		checkTilePosition(levelData, collideTilePosition, m_position.x + m_substructDims.x, m_position.y + m_size.y + m_substructDims.y);

		//Fourth corner
		checkTilePosition(levelData, collideTilePosition, m_position.x + m_size.x + m_substructDims.x, m_position.y + m_size.y + m_substructDims.y);

		//if it doesn't collide with anything, just return false
		if ( collideTilePosition.size() == 0 )
			return false;

		//Do the collision
		for ( size_t i = 0; i < collideTilePosition.size(); i++ )
			collideWithTile(collideTilePosition[i]);

		return true;

	}

	void CollideWithMonsters(std::vector<Monster*> &monsters, int currentMonsterIndex)
	{
		for ( size_t i = (size_t)currentMonsterIndex + 1; i < monsters.size(); i++ )
			collideWithMonster(monsters[i]);
	}

	void CollideWithItems(std::vector<Item*> &items)
	{
		for ( size_t i = 0; i < items.size(); i++ )
			collideWithItem(items[i]);
	}

	bool IsDead() const
	{
		return m_isDied;
	}
	bool IsStartedDeathAnimation() const
	{
		return m_startDeathAnimation;
	}

	int GetAttackPoint() const
	{
		return m_attackPoint;
	}
	int GetExperiencePoint() const
	{
		return m_experiencePoint;
	}
	float GetSpeed() const
	{
		return m_speed;
	}
	glm::vec2& GetPosition()
	{
		return m_position;
	}
	glm::vec2 GetSize() const
	{
		return m_size;
	}
	glm::vec2 GetSubstructDims() const
	{
		return m_substructDims;
	}
	MonsterState GetMonsterState() const
	{
		return m_state;
	}
	bool IsAttacked() const
	{
		return m_isAttacked;
	}

	void SetPosition(const glm::vec2 &pos)
	{
		m_position = pos;
	}
	void SetAttackState(bool IsAttacked)
	{
		m_isAttacked = IsAttacked; m_flashingCount = 0;
	}

protected:
	bool collideWithMonster(Monster *monster)
	{
		const float MIN_DISTANCE_X = (m_size.x / 2.0f) + (monster->GetSize().x / 2.0f);
		const float MIN_DISTANCE_Y = (m_size.y / 2.0f) + (monster->GetSize().y / 2.0f);

		glm::vec2 centerPosA = m_position + glm::vec2(m_size / 2.0f) + m_substructDims;
		glm::vec2 centerPosB = monster->GetPosition() + glm::vec2(monster->GetSize() / 2.0f) + monster->GetSubstructDims();

		glm::vec2 distVec = centerPosA - centerPosB;

		glm::vec2 MIN_DISTANCE = glm::vec2(MIN_DISTANCE_X, MIN_DISTANCE_Y);

		float xDepth = MIN_DISTANCE.x - abs(distVec.x);
		float yDepth = MIN_DISTANCE.y - abs(distVec.y);

		if ( xDepth > 0 && yDepth > 0 )
		{
			m_isCollided = true;
			if ( std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f) )
			{
				if ( distVec.x < 0 )
				{
					m_position.x -= (xDepth / 2.0f);
					monster->SetPosition(glm::vec2(monster->GetPosition().x + (xDepth / 2.0f), monster->GetPosition().y));
				}
				else
				{
					m_position.x += (xDepth / 2.0f);
					monster->SetPosition(glm::vec2(monster->GetPosition().x - (xDepth / 2.0f), monster->GetPosition().y));
				}
			}
			else
			{
				if ( distVec.y < 0 )
				{
					m_position.y -= (yDepth / 2.0f);
					monster->SetPosition(glm::vec2(monster->GetPosition().x, monster->GetPosition().y + (yDepth / 2.0f)));
				}
				else
				{
					m_position.y += (yDepth / 2.0f);
					monster->SetPosition(glm::vec2(monster->GetPosition().x, monster->GetPosition().y - (yDepth / 2.0f)));
				}
			}

			return true;
		}

		return false;
	}

	bool collideWithItem(Item *item)
	{
		//If it's too much, just delete last part.
		const float MIN_DISTANCE_X = (m_size.x / 2.0f) + (item->GetSize().x / 2.0f) - 5.0f;
		const float MIN_DISTANCE_Y = (m_size.y / 2.0f) + (item->GetSize().y / 2.0f) - 3.0f;

		glm::vec2 centerPosA = m_position + glm::vec2(m_size / 2.0f) + m_substructDims;
		glm::vec2 centerPosB = item->GetPosition() + glm::vec2(item->GetSize() / 2.0f);

		glm::vec2 distVec = centerPosA - centerPosB;

		glm::vec2 MIN_DISTANCE = glm::vec2(MIN_DISTANCE_X, MIN_DISTANCE_Y);

		float xDepth = MIN_DISTANCE.x - abs(distVec.x);
		float yDepth = MIN_DISTANCE.y - abs(distVec.y);

		if ( xDepth > 0 && yDepth > 0 )
		{
			m_isCollided = true;
			if ( std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f) )
			{
				if ( distVec.x < 0 )
				{
					m_position.x -= xDepth;
				}
				else
				{
					m_position.x += xDepth;
				}
			}
			else
			{
				if ( distVec.y < 0 )
				{
					m_position.y -= yDepth;
				}
				else
				{
					m_position.y += yDepth;
				}
			}

			return true;
		}

		return false;
	}


	void checkTilePosition(const std::vector<std::string> &levelData, std::vector<glm::vec2> &collideTilePosition, float x, float y)
	{
		glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH), floor(y / (float)TILE_WIDTH));

		if ( cornerPos.x < 0 || cornerPos.x >= levelData[0].size() || cornerPos.y < 0 || cornerPos.y >= levelData.size() )
			return;

		if ( levelData[(int)cornerPos.y][(int)cornerPos.x] != '.' )
			collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}

	//AABB collision
	void collideWithTile(glm::vec2 tilePos)
	{
		const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
		const float MIN_DISTANCE_X = m_size.x / 2.0f + TILE_RADIUS;
		const float MIN_DISTANCE_Y = m_size.y / 2.0f + TILE_RADIUS;

		glm::vec2 centerPlayerPosition = m_position + glm::vec2(m_size / 2.0f) + m_substructDims;
		glm::vec2 distVec = centerPlayerPosition - tilePos;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		//if this is true, we are colliding with tile
		if ( xDepth > 0 && yDepth > 0 )
		{
			m_isCollided = true;
			if ( (std::max(xDepth, 0.0f)) < (std::max(yDepth, 0.0f)) )
			{
				if ( distVec.x < 0 )
					m_position.x -= xDepth;
				else
					m_position.x += xDepth;
			}
			else
			{
				if ( distVec.y < 0 )
					m_position.y -= yDepth;
				else
					m_position.y += yDepth;
			}
		}
	}

	void calculateHitPoint()
	{
		m_hpPercentage = ((float)m_currentHitPoint / (float)m_hitPoint);
		m_hpGreen = (int)(255 * m_hpPercentage);
	}

	int m_baseHitPoint = 1;
	int m_baseAttaciPoint = 1;
	int m_hitPoint = 0;
	int m_attackPoint = 0;
	int m_currentHitPoint = 0;
	int m_level = 1;
	int m_movementCount = 0;
	int m_currentMovementIndex = 0;
	int m_experiencePoint = 0;
	int m_followingArea = 0;
	int m_attackingAreaX = 0;
	int m_attackingAreaY = 0;
	int m_waitingTime = 0;
	int m_waitingCounter = 0;
	int m_hpGreen = 255;
	int m_monsterRed = 255;
	int m_monsterGreen = 255;
	int m_monsterBlue = 255;
	int m_monsterAlpha = 255;
	int m_flashingCount = 0;
	float m_hpPercentage = 1.0f;
	float m_hpBarSize = 0;
	float m_animTime = 0;
	float m_speed = 0;
	bool m_isCollided = false;
	bool m_isAttacked = false;
	bool m_attacked = false;
	bool m_isDied = false;
	bool m_flashing = false;
	bool m_startDeathAnimation = false;
	glm::vec2 m_position = glm::vec2(0.0f);
	MonsterState m_state = MonsterState::NORMAL;
	MovingState m_direction = MovingState::UP;
	ColorRGBA8 m_color = ColorRGBA8(255, 255, 255, 255);
	ColorRGBA8 m_hpColor = ColorRGBA8(0, 0, 0, 255);
	TileSheet m_texture;
	TileSheet m_attackMotion;
	Texture m_hpBarTexture;
	Texture m_hpTexture;

	glm::vec2 m_size = glm::vec2(0.0f);
	glm::vec2 m_additionalDims = glm::vec2(0.0f);
	glm::vec2 m_substructDims = glm::vec2(0.0f);
};

class Skeleton : public Monster
{
public:
	Skeleton();

	virtual void Init(int level, float speed, const glm::vec2 &position, int movementCount = 1) override;
	virtual void Update(const std::vector<std::string> &levelData, const glm::vec2 &playerPosition) override;
	virtual glm::vec4 Animation() override;
	virtual void Destroy() override;

private:
	void changeDirection(const std::vector<std::string> &levelData);

	float m_distanceX = 0;
	float m_distanceY = 0;
	float m_nearRange = 0;
	int m_attackingCount = 0;
	int m_baseSpeed = 0;
	const int m_inRangeMovementCount = 4;
};