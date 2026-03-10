#include "GameController.h"
#include "pch.h"

void GameController::Init(ID _currentSceneID)
{
	m_currentSceneID = _currentSceneID;
	InputSystem::LockMouseCursor();
	InputSystem::HideMouseCursor();
}

void GameController::Update(float _dt)
{

	UpdateKeyboardInputs(_dt);
	UpdateMouseInputs(_dt);

	Render_Camera* cam = GameManager::GetRenderer()->GetCamera();
	Render_Window* wnd = GameManager::GetRenderer()->GetWindow();

	if (m_cursorLock == true)
	{
		InputSystem::LockMouseCursor();
		InputSystem::HideMouseCursor();
	}
	else
	{
		InputSystem::UnlockMouseCursor();
		InputSystem::ShowMouseCursor();
	}

}

void GameController::UpdateKeyboardInputs(float _dt)
{
	if (InputSystem::GetKeyDown(Keyboard::ESCAPE))
	{
		m_cursorLock = !m_cursorLock;
		if (m_gameStop == true)
		{
			GameManager::GetInstance()->GetGameTimer()->Reset();
			m_gameStop = !m_gameStop;
		}
		else
		{
			GameManager::GetInstance()->GetGameTimer()->Stop();
			m_gameStop = !m_gameStop;
		}
	}

	if (m_currentSceneID == MAIN_MENU)
	{
		if (InputSystem::GetKeyDown(Keyboard::ENTER))
		{
			SceneManager* sm = GameManager::GetSceneManagerInstance();

			MainScene* nextScene = new MainScene;
			sm->AddScene(nextScene);
			sm->SetCurrentScene(nextScene);
			m_ToDelete = true;
		}
	}


	if (m_currentSceneID == DEATH_MENU)
	{
		if (InputSystem::GetKeyDown(Keyboard::ENTER))
		{
			SceneManager* sm = GameManager::GetSceneManagerInstance();

			MainScene* nextScene = new MainScene;
			sm->AddScene(nextScene);
			sm->SetCurrentScene(nextScene);
			m_ToDelete = true;
		}
	}
}

void GameController::UpdateMouseInputs(float _dt)
{
}

void GameController::SetSceneID(ID _currentSceneID)
{
	m_currentSceneID = _currentSceneID;
}
