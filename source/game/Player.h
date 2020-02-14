#pragma once

#include "DefineLogic.h"
#include "Items.h"
#include "Monster.h"

constexpr float PLAYER_SPEED = 5.0f;
constexpr int PLAYER_DIM_X = 30;
constexpr int PLAYER_DIM_Y = 40;

enum class PlayerState
{
	ATTACKING,
	NORMAL
};

struct WeaponDesc
{
	WeaponDesc(const Texture &attackTexture, const  glm::ivec2 &attackDims, const Sound &sound, int attack, const glm::vec2 &attackingRange)
	{
		this->attackTexture.Init(attackTexture, attackDims);
		this->sound = sound;
		this->attack = attack;
		this->attackingRange = attackingRange;
	}

	TileSheet attackTexture;
	Sound sound;
	int attack;
	glm::vec2 attackingRange;
};

class Player
{
public:
	~Player();

	static Player* GetInstance();

	void Init(const glm::vec2 &position, float speed);

	glm::vec4 Animation();

	void Draw(SpriteBatch &spriteBatch);
	void DrawDebug(DebugRenderer &debuger);

	void Update(InputSystem &inputManager, const std::vector<std::string> &levelData);

	glm::vec2 CalculatePositionInGrid() const;

	void Recreate(const glm::vec2 &pos);

	bool TakeDamage(int damage);

	bool CollideWithLevel(const std::vector<std::string>& levelData);
	void CollideWithMonsters(std::vector<Monster*>& monsters);
	void CollideWithItems(std::vector<Item*>& items);

	bool IsAttackingFast()
	{
		return (m_animTime > 4.0f && m_playerState == PlayerState::ATTACKING && m_currentWeaponIndex != 0);
	}

	bool IsPlayerDead()
	{
		return m_isDead;
	};

	bool IsReachedGoal()
	{
		return m_isReachedGoal;
	}

	bool IsStartedDeathAnimation()
	{
		return m_startDeathAnimation;
	}

	void GetExperiencePoint(int exp);

	glm::vec2 GetPosition() const
	{
		return m_position + glm::vec2(m_substructWidth, m_substructHeight);
	}
	glm::vec2 GetCutSize() const
	{
		return glm::vec2(m_substructWidth, m_substructHeight);
	}
	glm::vec2 GetSize() const
	{
		return m_size;
	}

	void SetPosition(const glm::vec2& pos)
	{
		m_position = pos;
	}
	void SetReachedState(bool isReached)
	{
		m_isReachedGoal = isReached;
	}

private:
	void checkTilePosition(const std::vector<std::string>& levelData, std::vector<glm::vec2>& collideTilePosition, float x, float y);
	void collideWithTile(glm::vec2 tilePos, bool isRight = false);
	bool collideWithMonster(Monster* monster);
	bool collideWithItem(Item* item);
	void calculateHitPoint();
	bool Attack(const glm::vec2& distance);

	static Player* m_instance;

	bool m_isStanding = false;

	glm::vec2 m_position;
	glm::vec2 m_direction = glm::vec2(1.0f, 0.0);
	glm::vec2 m_attackingRange = glm::vec2(0.0f);

	MovingState m_state = MovingState::DOWN;
	PlayerState m_playerState = PlayerState::NORMAL;
	float m_speed = 0;
	float m_health = 0;
	float m_animTime = 0.0f;

	bool m_couldRun = true;
	bool m_finishAttacking = true;
	bool m_startDeathAnimation = false;
	bool m_isDead = false;
	bool m_isLookingItem = false;
	bool m_isReachedGoal = false;

	const int MAX_RUN_COUNT = 30;
	int m_runCount = 0;
	int m_counter = 0;
	int m_multiplyer = 1;

	TileSheet m_texture;
	Texture m_hpBar;
	Texture m_hitPoint;
	ColorRGBA8 m_hpColor;
	ColorRGBA8 m_color;
	float m_baseHitPoint = 100.0f;
	float m_maxHitPoint = 100.0f;
	float m_currentHitPoint = 100.0f;
	float m_hpPercentage = 1.0f;
	float m_experiencePoint = 0;
	float m_needExperiencePoint = 0;
	float m_hpSize = 0;
	int m_hpGreen = 255;
	int m_playerAlpha = 255;

	int m_level = 1;
	int m_baseAttackPoint = 5;
	int m_currentAttackPoint = 5;

	glm::vec2 m_size = glm::vec2(0.0f);

	int m_additionalWidth = 0;
	int m_additionalHeight = 0;

	int m_substructWidth = 0;
	int m_substructHeight = 0;

	int m_cutSize = 0;

	int m_numTiles = 0;

	//For item
	int m_effectiveAttackTime = 0;
	int m_effectiveSpeedTime = 0;
	int m_attackEffectsCount = 0;
	int m_speedEffectsCount = 0;
	int m_additionalAttack = 0;
	int m_additionalSpeed = 0;

	bool m_startAttackEffect = false;
	bool m_startSpeedEffect = false;

	Sound m_levelUp;
	Sound m_item;
	SpriteFont m_font;

	std::vector<WeaponDesc> m_weapon;
	int m_currentWeaponIndex = 0;
};