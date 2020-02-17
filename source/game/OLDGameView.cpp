#include "stdafx.h"
#include "OLDGameView.h"
#include "Application.h"
#include "TileSheet.h"
#include "GLSLProgram.h"
#include "SpriteBatch.h"
//-----------------------------------------------------------------------------
void OLDGameView::OnEntry()
{
	const int screenWidth = m_app->GetScreenWidth();
	const int screenHeight = m_app->GetScreenHeight();
	m_screenSize = glm::vec2(screenWidth, screenHeight);
	m_camera.Init(screenWidth, screenHeight);
	m_camera.SetScale(0.9);	

	m_debuger.Init();

	m_spriteBatch.Init();

	m_textureProgram.CompileShadersFromFile("../test/Shaders/color.vert", "../test/Shaders/color.frag");
	m_textureProgram.AddAttribute("vertexPosition");
	m_textureProgram.AddAttribute("vertexColor");
	m_textureProgram.AddAttribute("vertexUV");
	m_textureProgram.LinkShaders();

	m_bgm = ResourceManager::GetMusic("../test/Sound/Battleship.ogg");
	m_bgm.SetVolume(50);
	m_bgm.Play();

	initLevel();	
}
//-----------------------------------------------------------------------------
void OLDGameView::OnExit()
{
	for ( auto &it : m_monsters )
		delete it;
	m_monsters.clear();
	for ( auto &it :m_items )
		delete it;
	m_items.clear();
	m_level.release();
	m_textureProgram.Dispose();	
	m_spriteBatch.Dispose();
	m_debuger.Dispose();
	if ( m_player )
		m_player->SetReachedState(false);
}
//-----------------------------------------------------------------------------
void OLDGameView::Update()
{
	m_camera.SetPosition(m_level->GetCameraPos(m_player->GetPosition(), m_screenSize, m_camera.GetScale()));
	m_camera.Update();

	if ( m_app->GetEventHandler()->IsKeyPressed(SDLK_d) )
		m_isDebugMode = !m_isDebugMode;

	updateObject();
}
//-----------------------------------------------------------------------------
void OLDGameView::Draw()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(195.0 / 255.0, 195.0 / 255.0, 195.0 / 255.0, 1.0);

	m_textureProgram.Use();

	const GLint textureUniform = m_textureProgram.GetUniformLocation("samplerUniform");
	glUniform1i(textureUniform, 0);
	glActiveTexture(GL_TEXTURE0);

	const glm::mat4 projectionMatrix = m_camera.GetCameraMatrix();
	const GLint pUniform = m_textureProgram.GetUniformLocation("projectionMatrix");
	glUniformMatrix4fv(pUniform, 1, GL_FALSE, &(projectionMatrix[0][0]));

	m_level->Draw();

	m_spriteBatch.Begin();
	{
		for ( auto &it : m_items )
		{
			if ( m_camera.IsBoxInView(it->GetPosition(), it->GetSize()) )
				it->Draw(m_spriteBatch);
		}

		m_player->Draw(m_spriteBatch);

		for ( auto &it : m_monsters )
		{
			if ( m_camera.IsBoxInView(it->GetPosition(), it->GetSize()) )
				it->Draw(m_spriteBatch);
		}
	}
	m_spriteBatch.End();
	m_spriteBatch.RenderBatch();

	m_textureProgram.Unuse();

	if ( m_isDebugMode )
		DrawDebug(projectionMatrix);
}
//-----------------------------------------------------------------------------
int OLDGameView::GetNextViewIndex() const
{
	return -1;
}
//-----------------------------------------------------------------------------
int OLDGameView::GetPreviousViewIndex() const
{
	return -1;
}
//-----------------------------------------------------------------------------
void OLDGameView::initLevel()
{
	m_level = std::make_unique<Level>("../test/Levels/testLevel.txt");

	std::mt19937 randomEngine((unsigned int)time(nullptr));

	const std::uniform_int_distribution<int> randomMonsterNum(10, 10);
	const std::uniform_int_distribution<int> randomMovement(0, 1000);
	const std::uniform_int_distribution<int> yPos(3, m_level->GetHeight() - 3);
	const std::uniform_int_distribution<int> xPos(3, m_level->GetWidth() - 3);
	const std::uniform_int_distribution<int> randomItemNum(1, 10);
	const std::uniform_int_distribution<int> randomItemKind(0, 3);	

	int i = 0;
	int count = 2;
	const int numMonster = randomMonsterNum(randomEngine);
	m_monsters.reserve(numMonster);
	while ( i < numMonster )
	{
		const int x = xPos(randomEngine);
		const int y = yPos(randomEngine);
		const int movement = ((randomMovement(randomEngine) * count) % MAX_MOVEMENT + count / 2);
		if ( m_level->GetSymbol(x, y) == '.' )
		{
			glm::vec2 pos = glm::vec2(x * TILE_WIDTH, y * TILE_WIDTH);
			m_monsters.push_back(new Skeleton);
			m_monsters.back()->Init(2, 3, pos, 20);
			i++;
		}
		count++;
	}

	const int numItem = randomItemNum(randomEngine);
	int j = 0;
	while ( j < numItem )
	{
		const int typeItem = randomItemKind(randomEngine);
		const int x = xPos(randomEngine);
		const int y = yPos(randomEngine);
		if ( m_level->GetSymbol(x, y) == '.' )
		{
			glm::vec2 pos = glm::vec2(x * TILE_WIDTH, y * TILE_WIDTH);

			switch ( typeItem )
			{
			case 0:
				m_items.push_back(new BigPotion);
				break;
			case 1:
				m_items.push_back(new SmallPotion);
				break;
			case 2:
				m_items.push_back(new AttPotion);
				break;
			case 3:
				m_items.push_back(new SpeedPotion);
				break;
			default:
				Throw("Not ID item");
				break;
			}

			m_items.back()->Init(pos);
			j++;
		}
	}

	m_player = Player::GetInstance();
	m_player->Init(m_level->GetStartPlayerPosition(), PLAYER_SPEED);
}
//-----------------------------------------------------------------------------
void OLDGameView::DrawDebug(const glm::mat4 &projectionMatrix)
{
	glm::vec4 destRect;
	for ( auto &it : m_monsters )
		it->DrawDebug(m_debuger);

	for ( auto &it : m_items )
		it->DrawDebug(m_debuger);

	m_player->DrawDebug(m_debuger);

	m_debuger.End();
	m_debuger.Render(projectionMatrix, 2.0);
}
//-----------------------------------------------------------------------------
void OLDGameView::updateObject()
{
	auto eventHandler = m_app->GetEventHandler();

	m_player->Update(*eventHandler, m_level->GetLevelData());
	if ( m_player->IsPlayerDead() )
		m_player->Recreate(m_level->GetStartPlayerPosition());

	for ( size_t i = 0; i < m_monsters.size(); i++ )
	{
		if ( m_monsters[i]->IsDead() )
		{
			delete m_monsters[i];
			m_monsters[i] = m_monsters.back();
			m_monsters.pop_back();
			i--;
		}
	}

	for ( size_t i = 0; i < m_items.size(); i++ )
	{
		if ( m_items[i]->IsDisappeared() )
		{
			delete m_items[i];
			m_items[i] = m_items.back();
			m_items.pop_back();
			i--;
		}
	}

	m_player->CollideWithMonsters(m_monsters);
	m_player->CollideWithItems(m_items);

	for ( auto &it : m_monsters )
		it->Update(m_level->GetLevelData(), m_player->GetPosition());

	for ( size_t i = 0; i < m_monsters.size(); i++ )
	{
		m_monsters[i]->CollideWithMonsters(m_monsters, i);
		m_monsters[i]->CollideWithItems(m_items);
	}
}
//-----------------------------------------------------------------------------