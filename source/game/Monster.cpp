#include "stdafx.h"
#include "Monster.h"
#include "Player.h"
//-----------------------------------------------------------------------------
void Monster::Draw(SpriteBatch &spriteBatch)
{
	{
		const glm::vec4 uvRect = Animation();
		const glm::vec4 destRect = { m_position.x, m_position.y, (float)m_size.x + m_additionalDims.x, (float)m_size.y + m_additionalDims.y };		

		if ( m_isAttacked && m_state == MonsterState::NORMAL && !m_startDeathAnimation )
		{
			if ( m_flashing )
			{
				m_monsterAlpha = 255;
				if ( m_flashingCount % 2 == 0 )
					m_flashing = false;
			}
			else
			{
				m_monsterAlpha = 0;
				if ( m_flashingCount % 2 == 0 )
					m_flashing = true;
			}

			m_flashingCount++;
		}

		if ( !m_isAttacked && !m_startDeathAnimation )
			m_monsterAlpha = 255;

		m_color = ColorRGBA8(m_monsterRed, m_monsterGreen, m_monsterBlue, m_monsterAlpha);

		if ( (m_state == MonsterState::NORMAL || m_state == MonsterState::WAITING) && !m_startDeathAnimation )
			spriteBatch.Draw(destRect, uvRect, m_texture.texture.ID(), 1.0f, m_color);
		else if ( m_state == MonsterState::ATTACKING || m_startDeathAnimation )
			spriteBatch.Draw(destRect, uvRect, m_attackMotion.texture.ID(), 1.0f, m_color);
	}

	{//This is for HitPoint
		m_hpBarSize = m_size.x;

		const glm::vec4 hpBarDestRect = glm::vec4(m_position.x + m_substructDims.x, m_position.y + m_size.y + m_substructDims.y + 5, m_hpBarSize, 3.0f);

		calculateHitPoint();

		glm::uvec4 hpDestRect;
		hpDestRect.x = (glm::uint)hpBarDestRect.x;
		hpDestRect.y = (glm::uint)hpBarDestRect.y;
		hpDestRect.z = (glm::uint)(m_hpBarSize * m_hpPercentage);
		hpDestRect.w = (glm::uint)3.0f;

		m_hpColor.r = 255 - m_hpGreen;
		m_hpColor.g = m_hpGreen;

		spriteBatch.Draw(hpBarDestRect, UVRect, m_hpBarTexture.ID(), 2.0f, m_color);
		spriteBatch.Draw(hpDestRect, UVRect, m_hpTexture.ID(), 2.0f, m_hpColor);
	}
}
//-----------------------------------------------------------------------------
void Monster::DrawDebug(DebugRenderer &debuger)
{
	glm::vec4 destRect;
	destRect.x = m_position.x + m_substructDims.x;
	destRect.y = m_position.y + m_substructDims.y;
	destRect.z = m_size.x;
	destRect.w = m_size.y;

	debuger.DrawBox(destRect, ColorRGBA8(255, 0, 0, 255), 0);
}
//-----------------------------------------------------------------------------
Skeleton::Skeleton()
{
	m_baseHitPoint = 300;
	m_baseAttaciPoint = 10;
	m_experiencePoint = 30;
	m_followingArea = 2000;

	m_attackingAreaX = PLAYER_DIM_X + 30;
	m_attackingAreaY = PLAYER_DIM_Y + 20;

	m_size = glm::vec2(30, 50);
	m_additionalDims = glm::vec2(60, 40.0f);
	m_substructDims = glm::vec2(30.0f, 20.0f);

	m_nearRange = 500;
}
//-----------------------------------------------------------------------------
void Skeleton::Init(int level, float speed, const glm::vec2 &position, int movementCount)
{
	Texture texture = ResourceManager::GetTexture("../test/Assets/Monsters/skelton_movement.png");
	Texture attack = ResourceManager::GetTexture("../test/Assets/Monsters/skelton_attack.png");

	m_level = level;
	m_speed = speed;
	m_position = position;
	m_movementCount = movementCount;
	m_direction = MovingState::DOWN;

	m_attackPoint = m_baseAttaciPoint + m_level * 3;
	m_hitPoint = m_baseHitPoint + m_level * 3;
	m_currentHitPoint = m_hitPoint;

	m_experiencePoint = int(m_experiencePoint * (m_level / 2));

	m_texture.Init(texture, glm::ivec2(8, 4));
	m_attackMotion.Init(attack, glm::ivec2(8, 5));


	m_hpBarTexture = ResourceManager::GetTexture("../test/Assets/HitPoint/HitPoint.png");
	m_hpTexture = ResourceManager::GetTexture("../test/Assets/HitPoint/HitPoint.png");

	m_hpPercentage = 1.0f;
	m_hpBarSize = 20;
	m_hpGreen = 255;
	m_hpColor = ColorRGBA8(0, m_hpGreen, 0, 255);

	m_waitingTime = 10;

	m_baseSpeed = m_speed;
}
//-----------------------------------------------------------------------------
glm::vec4 Skeleton::Animation()
{
	glm::vec4 uvRect;
	int tileIndex;
	float animSpeed = 0.2f;
	const int NUM_TILE = 8;

	if ( !m_startDeathAnimation )
	{
		if ( m_state == MonsterState::NORMAL || m_state == MonsterState::ATTACKING )
		{
			//Check the MovingState.
			switch ( m_direction )
			{
			case MovingState::LEFT:
				tileIndex = 16;
				break;
			case MovingState::RIGHT:
				tileIndex = 0;
				break;
			case MovingState::UP:
				tileIndex = 24;
				break;
			case MovingState::DOWN:
				tileIndex = 8;
				break;
			}

			//Apply animation
			tileIndex = tileIndex + (int)m_animTime % NUM_TILE;

			//Check for attack end
			if ( m_animTime > NUM_TILE && m_state != MonsterState::NORMAL )
			{
				if ( abs(m_distanceX) > (PLAYER_DIM_X / 2.0f) || abs(m_distanceX) > (PLAYER_DIM_Y / 2.0f) )
				{
					m_state = MonsterState::NORMAL;
				}
				m_animTime = 0.0f;
				m_attacked = false;
				m_waitingCounter = 0;
			}

			if ( m_state == MonsterState::NORMAL )
			{
				//Increment animation time
				m_animTime += animSpeed;
				//Get the uv coordinates from the tile index
				uvRect = m_texture.GetUV(tileIndex);
			}
			else if ( m_state == MonsterState::ATTACKING )
			{

				m_animTime += animSpeed * 0.85f;
				uvRect = m_attackMotion.GetUV(tileIndex);
			}
		}
	}
	else
	{
		//This is death animation.
		int deathTile = 6;
		tileIndex = 32;
		m_animTime += animSpeed * 0.5f;

		if ( m_animTime >= deathTile )
		{
			tileIndex = 37;
			m_monsterAlpha -= 10;
			if ( m_monsterAlpha < 0 )
			{
				m_monsterAlpha = 0;
				m_isDied = true;
			}
		}
		else
		{
			tileIndex = tileIndex + (int)m_animTime % deathTile;
		}

		uvRect = m_attackMotion.GetUV(tileIndex);

	}

	return uvRect;
}
//-----------------------------------------------------------------------------
void Skeleton::Update(const std::vector<std::string>& levelData, const glm::vec2& playerPosition)
{
	//Think about something.
	std::mt19937 randomEngine((unsigned int)time(NULL));
	std::uniform_int_distribution<int> randDirection(0, 3);
	glm::vec2 distance = playerPosition - (m_position + m_substructDims);

	//If monster is dead, do nothing.
	if ( m_startDeathAnimation )
		return;

	//Check the direction.
	m_distanceX = distance.x;
	m_distanceY = distance.y;

	if ( abs(distance.x) <= (m_attackingAreaX) && abs(distance.y) <= (m_attackingAreaY / 1.5f) && !m_isAttacked )
	{
		if ( m_waitingCounter >= m_waitingTime )
		{
			if ( m_state != MonsterState::ATTACKING )
			{
				m_animTime = 0;
				m_waitingCounter = 0;
				changeDirection(levelData);
			}

			m_state = MonsterState::ATTACKING;

			//Player Attack First.
			if ( Player::GetInstance()->IsAttackingFast() && m_animTime < 5.0 )
			{
				m_state = MonsterState::NORMAL;
				m_isAttacked = true;
				m_waitingCounter = 0;
				return;
			}

			//Actual attack here.
			if ( !m_attacked && m_animTime >= 5.0f )
			{
				auto player = Player::GetInstance();
				if ( !player->IsStartedDeathAnimation() )
				{
					player->TakeDamage(m_attackPoint);
					m_attacked = true;
				}
				m_waitingCounter = 0;
			}
		}
		else
		{
			m_waitingCounter++;
		}
	}
	else if ( m_state == MonsterState::NORMAL )
	{
		m_waitingCounter = 0;

		if ( abs(distance.x) < m_followingArea && abs(distance.y) < m_followingArea )
		{
			//Actualy this is movement command here.
			if ( m_direction == MovingState::UP )
			{
				m_position.y += m_speed;
			}
			else if ( m_direction == MovingState::LEFT )
			{
				m_position.x -= m_speed;
			}
			else if ( m_direction == MovingState::DOWN )
			{
				m_position.y -= m_speed;
			}
			else
			{
				m_position.x += m_speed;
			}

			m_currentMovementIndex++;
			if ( m_currentMovementIndex == m_movementCount || m_isCollided )
			{
				m_currentMovementIndex = 0;

				if ( abs(distance.x) < m_nearRange && abs(distance.y) < m_nearRange && m_attackingCount == m_inRangeMovementCount )
				{
					m_attackingCount = 0;
					//THink I want to speed up.
					m_speed = m_baseSpeed * 1.2f;
				}
				else if ( m_attackingCount < m_inRangeMovementCount )
				{
					m_attackingCount++;
				}
				else
				{
					//It's in following range but not in near range.
					//So speed down.
					m_speed = m_baseSpeed;
				}

				auto prevState = m_direction;
				changeDirection(levelData);
			}
		}
	}	   

	//Colliding with Level (walls).
	CollideWithLevel(levelData);
}
//-----------------------------------------------------------------------------
void Skeleton::Destroy()
{
	m_startDeathAnimation = true;
	m_animTime = 0.0f;
	m_monsterAlpha = 255;
	m_isAttacked = false;
	Player::GetInstance()->GetExperiencePoint(m_experiencePoint);
}
//-----------------------------------------------------------------------------
void Skeleton::changeDirection(const std::vector<std::string> &levelData)
{
	auto prevState = m_direction;
	glm::vec2 gridPos = glm::vec2(floor(m_position.x / (float)TILE_WIDTH), floor(m_position.y / (float)TILE_WIDTH));
	int x = 0, y = 0;
	int additionalIndex = 0;

	if ( m_distanceX > 0.0f && m_distanceY > 0.0f )
	{
		//right upper
		if ( abs(m_distanceX) > abs(m_distanceY) )
		{
			m_direction = MovingState::RIGHT;
			x += 1;
		}
		else
		{
			m_direction = MovingState::UP;
			y -= 1;
		}
	}
	else if ( m_distanceX > 0.0f && m_distanceY <= 0.0f )
	{
		//right lower
		if ( abs(m_distanceX) > abs(m_distanceY) )
		{
			m_direction = MovingState::RIGHT;
			x += 1;
		}
		else
		{
			m_direction = MovingState::DOWN;
			y += 1;
		}
	}
	else if ( m_distanceX < 0.0f && m_distanceY > 0.0f )
	{
		//left upper
		if ( abs(m_distanceX) > abs(m_distanceY) )
		{
			m_direction = MovingState::LEFT;
			x -= 1;
		}
		else
		{
			m_direction = MovingState::UP;
			y -= 1;
		}
	}
	else
	{
		//left lower
		if ( abs(m_distanceX) > abs(m_distanceY) )
		{
			m_direction = MovingState::LEFT;
			x -= 1;
		}
		else
		{
			m_direction = MovingState::DOWN;
			y += 1;
		}
	}

	if ( m_isCollided )
	{
		m_isCollided = false;
		if ( levelData[(int)gridPos.y + y][(int)gridPos.x + x] != '.' )
		{
			y = -y;
			x = -x;
			if ( levelData[(int)gridPos.y + y][(int)gridPos.x + x] != '.' )
			{
				if ( x == 0 )
				{
					x = 1;
					y = 0;
				}
				else
				{
					x = 0;
					y = 1;
				}

				if ( levelData[(int)gridPos.y + y][(int)gridPos.x + x] != '.' )
				{
					x = -x;
					y = -y;
				}
			}

			if ( x == 1 )
			{
				m_direction = MovingState::RIGHT;
			}
			else if ( x == -1 )
			{
				m_direction = MovingState::LEFT;
			}
			else if ( y == 1 )
			{
				m_direction = MovingState::UP;
			}
			else
			{
				m_direction = MovingState::DOWN;
			}
		}
	}
}
//-----------------------------------------------------------------------------