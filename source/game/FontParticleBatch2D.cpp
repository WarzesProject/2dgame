#include "stdafx.h"
#include "FontParticleBatch2D.h"
//-----------------------------------------------------------------------------
void FontParticleBatch2D::Init(int maxParticles, float decayRate, SpriteFont font, std::function<void(Particle2D&, float deltaTime)> updateFunc)
{
	m_font = font;
	IParticleBatch2D::Init(maxParticles, decayRate, updateFunc);
}
//-----------------------------------------------------------------------------
void  FontParticleBatch2D::Draw(SpriteBatch& spriteBatch, Particle2D& p)
{
	m_font.Draw(spriteBatch, p.text.c_str(), p.destRect, 0.0f, p.color);
}
//-----------------------------------------------------------------------------