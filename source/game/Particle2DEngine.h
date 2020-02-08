#pragma once

class IParticleBatch2D;
class SpriteBatch;
class Camera2D;

class Particle2DEngine
{
public:
	void AddParticleBatch(IParticleBatch2D *particleBatch);

	void Update(float deltaTime);

	void Draw(SpriteBatch &spriteBatch);

private:
	std::vector<IParticleBatch2D*> m_particleBatches;
};