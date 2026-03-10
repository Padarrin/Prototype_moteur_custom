#include "PlayerController.h"
#include "pch.h"

PlayerController::~PlayerController()
{
	m_pPlayer = nullptr;
}

void PlayerController::Init(Player* _player)
{
	m_pPlayer = _player;
}

void PlayerController::Update(float _dt)
{

	UpdateKeyboardInputs(_dt);
	UpdateMouseInputs(_dt);

	Render_Camera* cam = GameManager::GetRenderer()->GetCamera();
	Render_Window* wnd = GameManager::GetRenderer()->GetWindow();

	if (m_cursorLock == true && m_gameStop == false)
	{
		

		int mousePos_x = InputSystem::GetMousePosition().x;
		int mousePos_y = InputSystem::GetMousePosition().y;
		XMFLOAT2 mousePos((float)mousePos_x, (float)mousePos_y);
		XMFLOAT2 center(wnd->GetWidth() / 2, wnd->GetHeight() / 2);
		float deltaX = (float)mousePos.x - center.x;
		float deltaY = (float)mousePos.y - center.y;

		m_yaw += deltaX * m_sens;
		m_pitch += deltaY * m_sens;

		if (m_pitch > 89.0f) 
			m_pitch = 89.0f;
		if (m_pitch < -89.0f) 
			m_pitch = -89.0f;

		m_pPlayer->SetRotation(m_pitch, m_yaw, 0.f);
		InputSystem::SetMousePosition(center);
	}
	else
	{
		InputSystem::UnlockMouseCursor();
		InputSystem::ShowMouseCursor();
	}
	
}

void PlayerController::UpdateKeyboardInputs(float _dt)
{
	if (InputSystem::GetKeyDown(Keyboard::ESCAPE))
	{
		m_cursorLock = !m_cursorLock;
		m_gameStop = !m_gameStop;
		
	}

	if (m_gameStop == false)
	{
		if (InputSystem::GetKeyDown(Keyboard::R))
		{
			m_pPlayer->Reload(_dt);
		}
	}

	/*if (InputSystem::GetKey(Keyboard::Z))
	{
		m_pPlayer->GetCam()->SetPosition(m_pPlayer->GetCam()->GetPosition().x, m_pPlayer->GetCam()->GetPosition().y, m_pPlayer->GetCam()->GetPosition().z + 5.f * _dt) ;
	}
	if (InputSystem::GetKey(Keyboard::S))
	{
		m_pPlayer->GetCam()->SetPosition(m_pPlayer->GetCam()->GetPosition().x, m_pPlayer->GetCam()->GetPosition().y, m_pPlayer->GetCam()->GetPosition().z - 5.f * _dt);
	}
	if (InputSystem::GetKey(Keyboard::D))
	{
		m_pPlayer->GetCam()->SetPosition(m_pPlayer->GetCam()->GetPosition().x + 5.f * _dt, m_pPlayer->GetCam()->GetPosition().y, m_pPlayer->GetCam()->GetPosition().z);
	}
	if (InputSystem::GetKey(Keyboard::Q))
	{
		m_pPlayer->GetCam()->SetPosition(m_pPlayer->GetCam()->GetPosition().x - 5.f * _dt, m_pPlayer->GetCam()->GetPosition().y, m_pPlayer->GetCam()->GetPosition().z );
	}*/
}

void PlayerController::UpdateMouseInputs(float _dt)
{
	if (m_gameStop == false)
	{
		if (InputSystem::GetButtonDown(Mouse::LEFT))
		{
			m_pPlayer->Shoot(_dt);
		}
	}
}
