#include "stdafx.h"
#include "Player.h"
//-----------------------------------------------------------------------------
Player* Player::m_instance = nullptr;
//-----------------------------------------------------------------------------
Player::~Player()
{
	m_font.Dispose();
}
//-----------------------------------------------------------------------------
Player* Player::GetInstance()
{
	if( m_instance == nullptr )
	{
		m_instance = new Player();
	}
	return m_instance;
}
//-----------------------------------------------------------------------------
void Player::Init(const glm::vec2& position, float speed)
{
	Texture texture = ResourceManager::GetTexture("../test/Assets/Player/player.png");
	m_hpBar = ResourceManager::GetTexture("../test/Assets/HitPoint/HitPoint.png");
	m_hitPoint = ResourceManager::GetTexture("../test/Assets/HitPoint/HitPoint.png");
	
	//Initialize sprite Font.
	m_font.Init("../test/Fonts/shangri-la.ttf", 32);

	m_texture.Init(texture, glm::ivec2(9, 4));

	m_levelUp = ResourceManager::GetSound("../test/Sound/LevelUp.ogg");
	m_item = ResourceManager::GetSound("../test/Sound/item.ogg");
	
	m_weapon.push_back(WeaponDesc(ResourceManager::GetTexture("../test/Assets/Player/player_mace.png"), glm::ivec2(6, 5), ResourceManager::GetSound("../test/Sound/mace.mp3"), 0, glm::vec2(0.0)));
	m_attackingRange = glm::vec2(0.0f);

	m_weapon.push_back(WeaponDesc(ResourceManager::GetTexture("../test/Assets/Player/player_mace.png"), glm::ivec2(6, 5), ResourceManager::GetSound("../test/Sound/mace.mp3"), 30, glm::vec2(85, 85)));
	m_currentWeaponIndex = (m_currentWeaponIndex + 1) % (int)m_weapon.size();
	m_attackingRange = m_weapon[m_currentWeaponIndex].attackingRange / glm::vec2(2.0f);


	m_position = position;
	m_speed = speed;
	m_color = ColorRGBA8(255, 255, 255, 255);

	m_hpPercentage = 1.0f;
	m_hpSize = 30;

	m_hpColor = ColorRGBA8(0, 255, 0, 255);
	
	m_maxHitPoint = m_level * m_baseHitPoint;
	m_currentHitPoint = m_maxHitPoint;
	m_currentAttackPoint = m_level * m_baseAttackPoint;

	m_size = glm::vec2(30.0f, 40.0f);
	m_additionalWidth = 130;
	m_additionalHeight = 60;
	m_substructWidth = 65;
	m_substructHeight = 30;

	m_needExperiencePoint = 30;
}
//-----------------------------------------------------------------------------
glm::vec4 Player::Animation()
{
	int tileIndex;

	float animSpeed = 0.3f;
	glm::vec4 uvRect;

	if( !m_startDeathAnimation )
	{
		m_numTiles = m_playerState == PlayerState::NORMAL ? 9 : 6;

		if( m_animTime > m_numTiles )
		{
			m_playerState = PlayerState::NORMAL;
			m_animTime = 0;
			m_numTiles = 9;
			m_finishAttacking = true;
		}

		if( m_playerState == PlayerState::NORMAL )
		{
			//Check the state.
			if( m_state == MovingState::UP )
			{
				tileIndex = 27;
			}
			else if( m_state == MovingState::LEFT )
			{
				tileIndex = 18;
			}
			else if( m_state == MovingState::DOWN )
			{
				tileIndex = 9;
			}
			else if( m_state == MovingState::RIGHT )
			{
				tileIndex = 0;
			}

			if( m_isStanding )
			{
				m_numTiles = 1;
				m_animTime = 0.0f;
			}
		}
		else
		{
			switch( m_state )
			{
			case MovingState::UP:
				tileIndex = 18;
				break;
			case MovingState::DOWN:
				tileIndex = 6;
				break;
			case MovingState::RIGHT:
				tileIndex = 0;
				break;
			case MovingState::LEFT:
				tileIndex = 12;
				break;
			}
		}

		//Increment animation time
		m_animTime += animSpeed * m_multiplyer;
		//Apply animation
		tileIndex = tileIndex + (int)m_animTime % m_numTiles;

		if( m_playerState == PlayerState::NORMAL )
		{
			//Get the uv coordinates from the tile index
			uvRect = m_texture.GetUV(tileIndex);
		}
		else
		{
			//Get the uv coordinates from the tile index
			uvRect = m_weapon[m_currentWeaponIndex].attackTexture.GetUV(tileIndex);
		}
	}
	else
	{
		m_numTiles = 6;
		tileIndex = 24;
		m_animTime += animSpeed * 0.5f;
		if( m_animTime >= m_numTiles )
		{
			tileIndex = 29;
			m_playerAlpha -= 5;
			if( m_playerAlpha < 0 )
			{
				m_playerAlpha = 0;
				m_isDead = true;
			}
		}
		else
		{
			tileIndex = tileIndex + (int)m_animTime % m_numTiles;
		}

		uvRect = m_weapon[m_currentWeaponIndex].attackTexture.GetUV(tileIndex);
	}

	return uvRect;
}
//-----------------------------------------------------------------------------
void Player::Draw(SpriteBatch &spriteBatch)
{
	{// This is for player itself.
		glm::vec4 uvRect;

		glm::vec4 destRect;
		destRect.x = m_position.x;
		destRect.y = m_position.y;
		destRect.z = (float)m_size.x + m_additionalWidth;
		destRect.w = (float)m_size.y + m_additionalHeight;

		uvRect = Animation();

		m_color = ColorRGBA8(255, 255, 255, m_playerAlpha);

		if( m_playerState == PlayerState::NORMAL && !m_startDeathAnimation )
		{
			spriteBatch.Draw(destRect, uvRect, m_texture.texture.ID(), 1.0f, m_color);
		}
		else
		{
			spriteBatch.Draw(destRect, uvRect, m_weapon[m_currentWeaponIndex].attackTexture.texture.ID(), 1.0f, m_color);
		}
	}

	{//This is for HitPoint

		//Text label for Level.
		std::string text = "Lv. " + std::to_string(m_level);
		m_font.Draw(spriteBatch, text.c_str(), glm::vec2(m_position.x + 45, m_position.y + m_size.y + 30), glm::vec2(0.4f), 2.0f, ColorRGBA8(0, 0, 0, 255));


		glm::vec4 hpUVRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		glm::vec4 hpBarDestRect = glm::vec4(m_position.x + 65, m_position.y + m_size.y + 35, m_hpSize, 5.0f);

		calculateHitPoint();

		glm::uvec4 hpDestRect;
		hpDestRect.x = (glm::uint)hpBarDestRect.x;
		hpDestRect.y = (glm::uint)hpBarDestRect.y;
		hpDestRect.z = (glm::uint)(m_hpPercentage * m_hpSize);
		hpDestRect.w = (glm::uint)5.0f;

		m_hpColor.r = 255 - m_hpGreen;
		m_hpColor.g = m_hpGreen;

		spriteBatch.Draw(hpBarDestRect, hpUVRect, m_hpBar.ID(), 2.0f, m_color);
		spriteBatch.Draw(hpDestRect, hpUVRect, m_hitPoint.ID(), 2.0f, m_hpColor);
	}
}
//-----------------------------------------------------------------------------
void Player::Update(InputSystem& inputManager, const std::vector<std::string>& levelData)
{
	if( m_startDeathAnimation )
		return;

	if( m_playerState == PlayerState::NORMAL )
	{

		/*if( inputManager.IsKeyDown(SDLK_SPACE) && MAX_RUN_COUNT >= m_runCount )
		{
			m_multiplyer = 2;
			m_runCount++;
			m_counter = 0;
		}
		else
		{
			m_multiplyer = 1;
			m_couldRun = false;
		}*/

		//if( !m_couldRun )
		//{
		//	m_counter++;
		//	if( m_counter == 100 )
		//	{
		//		m_couldRun = true;
		//		m_runCount = 0;
		//	}
		//}

		if( inputManager.IsKeyDown(SDLK_LEFT) )
		{
			m_position.x -= ((m_speed)*m_multiplyer + m_additionalSpeed);
			m_state = MovingState::LEFT;
			m_isStanding = false;
			m_playerState = PlayerState::NORMAL;
		}
		else if( inputManager.IsKeyDown(SDLK_RIGHT) )
		{
			m_position.x += ((m_speed)*m_multiplyer + m_additionalSpeed);
			m_state = MovingState::RIGHT;
			m_isStanding = false;
			m_playerState = PlayerState::NORMAL;
		}
		else if( inputManager.IsKeyDown(SDLK_UP) )
		{
			m_position.y += ((m_speed)*m_multiplyer + m_additionalSpeed);
			m_state = MovingState::UP;
			m_isStanding = false;
			m_playerState = PlayerState::NORMAL;

		}
		else if( inputManager.IsKeyDown(SDLK_DOWN) )
		{
			m_position.y -= ((m_speed)*m_multiplyer + m_additionalSpeed);
			m_state = MovingState::DOWN;
			m_playerState = PlayerState::NORMAL;
			m_isStanding = false;
		}
		else
		{
			m_playerState = PlayerState::NORMAL;
			m_isStanding = true;
		}


		if ( inputManager.IsKeyDown(SDLK_x) )
		{
			m_isLookingItem = true;
		}
		else
		{
			m_isLookingItem = false;
		}

		if ( inputManager.IsKeyDown(SDLK_z) )
		{
			m_playerState = PlayerState::ATTACKING;
			m_animTime = 0.0f;
			if ( m_finishAttacking )
			{
				m_weapon[m_currentWeaponIndex].sound.Play(0, 60);
				m_finishAttacking = false;
			}
		}

	/*	if( inputManager.IsKeyDown(SDL_BUTTON_LEFT) )
		{
			m_playerState = PlayerState::ATTACKING;
			m_animTime = 0.0f;
			if( m_finishAttacking )
			{
				m_weapon[m_currentWeaponIndex].sound.Play(0, 60);
				m_finishAttacking = false;
			}
		}*/

		//if( inputManager.IsKeyDown(SDL_BUTTON_RIGHT) )
		//{
		//	m_isLookingItem = true;
		//}
		//else
		//{
		//	m_isLookingItem = false;
		//}

		//if( inputManager.IsKeyPressed(SDLK_1) )
		//{
		//	m_currentWeaponIndex = (m_currentWeaponIndex + 1) % (int)m_weapon.size();
		//	m_attackingRange = m_weapon[m_currentWeaponIndex].attackingRange / glm::vec2(2.0f);
		//}
	}

	// Do the collision check.
	CollideWithLevel(levelData);
}
//-----------------------------------------------------------------------------
glm::vec2 Player::CalculatePositionInGrid() const
{
	glm::vec2 centerPos = glm::vec2(floor((m_position.x + m_size.x / 2 + m_substructWidth) / (float)TILE_WIDTH), floor((m_position.y + m_size.y / 2 + m_substructHeight) / (float)TILE_WIDTH));
	return centerPos;
}
//-----------------------------------------------------------------------------
void Player::GetExperiencePoint(int exp)
{
	m_experiencePoint += exp;
	if( m_experiencePoint >= m_needExperiencePoint )
	{
		m_experiencePoint -= m_needExperiencePoint;
		m_level++;
		m_needExperiencePoint = (m_needExperiencePoint * (m_level));
		m_maxHitPoint = m_level * m_baseHitPoint;
		m_currentHitPoint = m_maxHitPoint;
		m_currentAttackPoint = m_level * m_baseAttackPoint;

		//Add the effects for level up;

		m_levelUp.Play();
	}
}
//-----------------------------------------------------------------------------
bool Player::CollideWithLevel(const std::vector<std::string>& levelData)
{
	std::vector<glm::vec2> collideTilePosition;

	//check the four corners
	//first corner. Left bottom
	checkTilePosition(levelData, collideTilePosition, m_position.x + m_substructWidth, m_position.y + m_substructHeight);

	//Second corner. Right bottom
	checkTilePosition(levelData, collideTilePosition, m_position.x + m_size.x + m_substructWidth, m_position.y + m_substructHeight);

	//Third corner. Left top
	checkTilePosition(levelData, collideTilePosition, m_position.x + m_substructWidth, m_position.y + m_size.y + m_substructHeight);

	//Fourth corner Right top
	checkTilePosition(levelData, collideTilePosition, m_position.x + m_size.x + m_substructWidth, m_position.y + m_size.y + m_substructHeight);

	//if it doesn't collide with anything, just return false
	if( collideTilePosition.size() == 0 )
		return false;

	//Do the collision
	for( size_t i = 0; i < collideTilePosition.size(); i++ )
	{
		collideWithTile(collideTilePosition[i]);
	}

	return true;
}
//-----------------------------------------------------------------------------
void Player::checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y)
{
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH), floor(y / (float)TILE_WIDTH));

	//If human is outside the world, just return.
	if( cornerPos.x < 0 || cornerPos.x >= levelData[0].size() || cornerPos.y < 0 || cornerPos.y >= levelData.size() )
	{
		//std::cout << "Human is created outside the world" << std::endl;
		return;
	}

	if( levelData[(int)cornerPos.y][(int)cornerPos.x] != '.' && levelData[(int)cornerPos.y][(int)cornerPos.x] != '#' )
	{
		collideTilePosition.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2((float)TILE_WIDTH / 2.0f));
	}
}
//-----------------------------------------------------------------------------
//AABB collision
void Player::collideWithTile(glm::vec2 tilePos, bool isRight)
{

	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE_X = (m_size.x / 2.0f) + TILE_RADIUS;
	const float MIN_DISTANCE_Y = (m_size.y / 2.0f) + TILE_RADIUS;


	glm::vec2 centerPlayerPosition = m_position + glm::vec2(m_size.x / 2.0f, m_size.y / 2.0f) + glm::vec2(m_substructWidth, m_substructHeight);
	glm::vec2 distVec = centerPlayerPosition - tilePos;


	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

	if( xDepth > 0 && yDepth > 0 )
	{
		if( std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f) )
		{
			if( distVec.x < 0 )
				m_position.x -= (xDepth);
			else
				m_position.x += (xDepth);
		}
		else
		{
			if( distVec.y < 0 )
				m_position.y -= (yDepth);
			else
				m_position.y += (yDepth);
		}
	}
}
//-----------------------------------------------------------------------------
void Player::CollideWithMonsters(std::vector<Monster*>& monsters)
{
	for( size_t i = 0; i < monsters.size(); i++ )
	{
		collideWithMonster(monsters[i]);
	}
}
//-----------------------------------------------------------------------------
void Player::CollideWithItems(std::vector<Item*>& items)
{
	for( size_t i = 0; i < items.size(); i++ )
	{
		collideWithItem(items[i]);
	}
}
//-----------------------------------------------------------------------------
bool Player::collideWithMonster(Monster* monster)
{
	const float MIN_DISTANCE_X = (m_size.x / 2.0f) + (monster->GetSize().x / 2.0f);
	const float MIN_DISTANCE_Y = (m_size.y / 2.0f) + (monster->GetSize().y / 2.0f);

	glm::vec2 centerPosA = m_position + glm::vec2(m_size / 2.0f) + glm::vec2(m_substructWidth, m_substructHeight);
	glm::vec2 centerPosB = monster->GetPosition() + (monster->GetSize() / 2.0f) + monster->GetSubstructDims();

	glm::vec2 distVec = centerPosA - centerPosB;

	float xDepth = MIN_DISTANCE_X - abs(distVec.x);
	float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

	if( !monster->IsStartedDeathAnimation() )
	{
		if( m_playerState == PlayerState::ATTACKING && m_animTime > 2.0f && (m_currentWeaponIndex != 0) )
		{
			if( Attack(distVec) && !monster->IsAttacked() )
			{
				monster->TakeDamage((m_currentAttackPoint + m_additionalAttack) + m_weapon[m_currentWeaponIndex].attack);
				monster->SetAttackState(true);
			}
		}

		if( m_animTime > m_numTiles )
		{
			monster->SetAttackState(false);
		}

		if( xDepth > 0 && yDepth > 0 )
		{
			if( std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f) )
			{
				if( distVec.x < 0 )
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
				if( distVec.y < 0 )
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

		}
	}
	else
	{
		if( xDepth > 0 && yDepth > 0 )
		{
			if( std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f) )
			{
				if( distVec.x < 0 )
				{
					m_position.x -= (xDepth);
				}
				else
				{
					m_position.x += (xDepth);
				}
			}
			else
			{
				if( distVec.y < 0 )
				{
					m_position.y -= (yDepth);
				}
				else
				{
					m_position.y += (yDepth);
				}
			}			
		}
	}

	return false;
}
//-----------------------------------------------------------------------------
void Player::DrawDebug(DebugRenderer& debuger)
{

	//For palyer itself.
	glm::vec4 destRect;
	destRect.x = m_position.x + m_substructWidth;
	destRect.y = m_position.y + m_substructHeight;
	destRect.z = m_size.x;
	destRect.w = m_size.y;

	//For player sprites.
	glm::vec4 destRect2;
	destRect2.x = m_position.x;
	destRect2.y = m_position.y;
	destRect2.z = m_size.x + m_additionalWidth;
	destRect2.w = m_size.y + m_additionalHeight;

	//For player attacking range.
	glm::vec4 destRect3;
	destRect3.x = m_position.x + ((m_substructWidth + m_size.x / 2.0) - m_attackingRange.x);
	destRect3.y = m_position.y + ((m_substructHeight + m_size.y / 2.0) - m_attackingRange.y);
	destRect3.z = m_attackingRange.x * 2;
	destRect3.w = m_attackingRange.y * 2;

	debuger.DrawBox(destRect, ColorRGBA8(255, 255, 255, 255), 0);
	debuger.DrawBox(destRect2, ColorRGBA8(255, 255, 255, 255), 0);
	debuger.DrawBox(destRect3, ColorRGBA8(255, 255, 255, 255), 0);
}
//-----------------------------------------------------------------------------
bool Player::collideWithItem(Item* item)
{
	//If it's too much, just delete last part.
	const float MIN_DISTANCE_X = (m_size.x / 2.0f) + (item->GetSize().x / 2.0f) - 5.0f;
	const float MIN_DISTANCE_Y = (m_size.y / 2.0f) + (item->GetSize().y / 2.0f) - 3.0f;

	glm::vec2 centerPosA = m_position + glm::vec2(m_size / 2.0f) + glm::vec2(m_substructWidth, m_substructHeight);
	glm::vec2 centerPosB = item->GetPosition() + glm::vec2(item->GetSize() / 2.0f);

	glm::vec2 distVec = centerPosA - centerPosB;

	glm::vec2 MIN_DISTANCE = glm::vec2(MIN_DISTANCE_X, MIN_DISTANCE_Y);

	float xDepth = MIN_DISTANCE.x - abs(distVec.x);
	float yDepth = MIN_DISTANCE.y - abs(distVec.y);

	if( xDepth + 5 > 0 && yDepth + 5 > 0 && m_isLookingItem && !item->IsTaken() )
	{
		m_item.Play(0, 20);
		if( item->GetItemInfo() == ItemInfo::ITEM )
		{
			//For Items
			int effects = item->GetEffects();
			switch( item->GetItemType() )
			{
			case ItemType::HP:
				m_currentHitPoint += effects;
				if( m_currentHitPoint > m_maxHitPoint )
				{
					m_currentHitPoint = m_maxHitPoint;
				}
				break;
			case ItemType::Attack:
				m_effectiveAttackTime = item->GetEffectiveTime();
				m_additionalAttack += effects;
				if( abs(m_additionalAttack) > m_currentAttackPoint && m_additionalAttack < 0 )
				{
					m_additionalAttack = -1 * (m_currentAttackPoint);
				}
				m_attackEffectsCount = 0;
				m_startAttackEffect = true;
				break;
			case ItemType::Speed:
				m_effectiveSpeedTime = item->GetEffectiveTime();
				m_additionalSpeed += effects;
				m_speedEffectsCount = 0;
				m_startSpeedEffect = true;
				break;
			}
		}
		//else
		//{
		//	//For Weapons.
		//	m_weapon.push_back(WeaponDesc(ResourceManager::GetTexture(item->GetAttackPath()), glm::ivec2(6, 5), ResourceManager::GetSound(item->GetSoundPath()), item->GetAttackPoint(), item->GetAttackingArea()));

		//}
		item->Destroy();
	}

	if( m_startAttackEffect )
	{
		m_attackEffectsCount++;
		if( m_effectiveAttackTime < m_attackEffectsCount )
		{
			m_attackEffectsCount = 0;
			m_additionalAttack = 0;
			m_startAttackEffect = false;
		}
	}

	if( m_startSpeedEffect )
	{
		m_speedEffectsCount++;
		if( m_effectiveSpeedTime < m_speedEffectsCount )
		{
			m_speedEffectsCount = 0;
			m_additionalSpeed = 0;
			m_startSpeedEffect = false;
		}
	}

	if( xDepth > 0 && yDepth > 0 )
	{

		if( std::max(xDepth, 0.0f) < std::max(yDepth, 0.0f) )
		{
			if( distVec.x < 0 )
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
			if( distVec.y < 0 )
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
//-----------------------------------------------------------------------------
bool Player::Attack(const glm::vec2& distance)
{
	if( m_attackingRange.x >= abs(distance.x) && m_attackingRange.y * 1.5f >= abs(distance.y) )
	{
		if( distance.x < 0.0f && m_state == MovingState::RIGHT )
		{
			return true;
		}

		if( distance.x >= 0.0f && m_state == MovingState::LEFT )
		{
			return true;
		}

		if( distance.y < 0.0f && m_state == MovingState::UP )
		{
			return true;
		}

		if( distance.y >= 0.0f && m_state == MovingState::DOWN )
		{
			return true;
		}

	}

	return false;
}
//-----------------------------------------------------------------------------
void Player::calculateHitPoint()
{
	m_hpPercentage = m_currentHitPoint / m_maxHitPoint;
	m_hpGreen = (int)(255 * m_hpPercentage);
}
//-----------------------------------------------------------------------------
bool Player::TakeDamage(int damage)
{
	m_currentHitPoint -= damage;
	if( m_currentHitPoint < 0 )
	{
		m_currentHitPoint = 0;
		m_startDeathAnimation = true;
		m_animTime = 0;
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
void Player::Recreate(const glm::vec2& pos)
{
	m_playerAlpha = 255;
	m_startDeathAnimation = false;
	m_isDead = false;

	m_position = pos;
	m_currentHitPoint = m_maxHitPoint;
	m_hpGreen = 255;
}
//-----------------------------------------------------------------------------