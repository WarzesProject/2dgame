#pragma once

#include "Texture.h"
#include "IParticleBatch2D.h"

class ParticleBatch2D : public IParticleBatch2D
{
public:
	void Init(int maxParticles, float decayRate, Texture texture, std::function<void(Particle2D&, float deltaTime)> updateFunc = DefaultParticleUpdate);

	virtual void Draw(SpriteBatch& spriteBatch, Particle2D& particle) override;

private:
	glm::vec4 m_uvRect;
	Texture m_texture;
};