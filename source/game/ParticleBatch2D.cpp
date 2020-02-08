#include "stdafx.h"
#include "ParticleBatch2D.h"
//-----------------------------------------------------------------------------
void ParticleBatch2D::Init(int maxParticles, float decayRate, Texture texture, std::function<void(Particle2D&, float deltaTime)> updateFunc)
{
	m_texture = texture;
	m_uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	IParticleBatch2D::Init(maxParticles, decayRate, updateFunc);
}
//-----------------------------------------------------------------------------
void  ParticleBatch2D::Draw(SpriteBatch &spriteBatch, Particle2D &p)
{
	spriteBatch.Draw(p.destRect, m_uvRect, m_texture.id, 0.0f, p.color);
}
//-----------------------------------------------------------------------------