#pragma once

class Player;

class PlayerController
{

private:
	Player* m_pPlayer = nullptr;

	bool m_cursorLock = true;
	bool m_gameStop = false;
	float m_pitch = 0.0f;
	float m_yaw = 0.0f;
	float m_sens = 0.1f;

public:
	~PlayerController();
	void Init(Player* _player);
	void Update(float _dt);
	void UpdateKeyboardInputs(float _dt);
	void UpdateMouseInputs(float _dt);
};

