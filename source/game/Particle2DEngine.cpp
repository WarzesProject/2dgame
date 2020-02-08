#include "stdafx.h"
#include "Particle2DEngine.h"
#include "IParticleBatch2D.h"
#include "SpriteBatch.h"
#include "Camera2D.h"
//-----------------------------------------------------------------------------
void Particle2DEngine::AddParticleBatch(IParticleBatch2D *particleBatch)
{
	m_particleBatches.push_back(particleBatch);
}
//-----------------------------------------------------------------------------
void Particle2DEngine::Update(float deltaTime)
{
	for ( auto &pb : m_particleBatches )
		pb->Update(deltaTime);
}
//-----------------------------------------------------------------------------
void Particle2DEngine::Draw(SpriteBatch &spriteBatch)
{
	for ( auto &pb : m_particleBatches )
	{
		spriteBatch.Begin();
		pb->Draw(spriteBatch);
		spriteBatch.End();
		spriteBatch.RenderBatch();
	}
}
//-----------------------------------------------------------------------------