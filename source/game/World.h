#pragma once

class World
{
public:
	void Init();
	void Close();

	void Update();
	void Draw();

	// ���������/�������� ���������� ������� ������
	void PauseLogic() { m_pause = true; }
	void PlayLogic() { m_pause = false; }

	// ���������/�������� ������ �������� ����
	void EnableRender() { m_isRender = true; }
	void DisableRender() { m_isRender = false; }
private:
	bool m_pause = false;
	bool m_isRender = true;
};