#pragma once

#include "SpriteFont.h"
#include "IParticleBatch2D.h"

class FontParticleBatch2D : public IParticleBatch2D
{
public:
	void Init(int maxParticles, float decayRate, SpriteFont font, std::function<void(Particle2D&, float deltaTime)> updateFunc = DefaultParticleUpdate);

	virtual void Draw(SpriteBatch& spriteBatch, Particle2D& particle) override;

private:
	SpriteFont m_font;
};