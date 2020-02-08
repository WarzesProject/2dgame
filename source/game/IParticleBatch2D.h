#pragma once

#include "Vertex.h"
#include "SpriteBatch.h"
#include "Camera2D.h"

class Particle2D
{
public:
	float life;
	std::string text;
	glm::vec2 velocity;
	glm::vec4 destRect;
	ColorRGBA8 color;
};

inline void DefaultParticleUpdate(Particle2D &particle, float deltaTime)
{
	particle.destRect.x += particle.velocity.x * deltaTime;
	particle.destRect.y += particle.velocity.y * deltaTime;
}

class SpriteBatch;
class IParticleBatch2D
{
public:
	IParticleBatch2D();
	~IParticleBatch2D();

	/*!
	* \brief Initialize particle batch stuffs.
	* \param maxParticles : number maximum of particles which can be generated
	* \param decayRate : Reduced life rate after a deltaTime
	* \param texture : particle texture
	* \param updateFunc : update particle function
	*/
	void Init(int maxParticles, float decayRate, std::function<void(Particle2D&, float deltaTime)> updateFunc);


	void AddParticle(const glm::vec4 &destRect, const glm::vec2 &velocity, const ColorRGBA8 &color, std::string text = std::string());


	void Update(float deltaTime);

	void Draw(SpriteBatch &spriteBatch);
	virtual void Draw(SpriteBatch &spriteBatch, Particle2D &particle) = 0;

protected:
	int findFreeParticle();

	float m_decayRate; // Reduced life rate after a deltaTime
	Particle2D *m_particles;
	int m_lastFreeParticle; //Index of last free particle
	int m_maxParticles;
	std::function<void(Particle2D&, float deltaTime)> m_updateFunc;
};