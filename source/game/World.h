#pragma once

class World
{
public:
	void Init();
	void Close();

	void Update();
	void Draw();

	// отключать/включать выполнение игровой логики
	void PauseLogic() { m_pause = true; }
	void PlayLogic() { m_pause = false; }

	// отключать/включать рендер игрового мира
	void EnableRender() { m_isRender = true; }
	void DisableRender() { m_isRender = false; }
private:
	bool m_pause = false;
	bool m_isRender = true;
};