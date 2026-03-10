#include "pch.h"
#include "GameManager.h"
#include "SceneManager.h"

GameManager* GameManager::m_pInstance = nullptr;
SceneManager* GameManager::m_pSceneManagerInstance = nullptr;
Render_App* GameManager::m_pRenderer = nullptr;
GameTimer* GameManager::m_gameTime = nullptr;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

GameManager* GameManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new GameManager;
	}
	return m_pInstance;
}

SceneManager* GameManager::GetSceneManagerInstance()
{
	if (m_pSceneManagerInstance == nullptr)
	{
		m_pSceneManagerInstance = new SceneManager;
	}
	return m_pSceneManagerInstance;
}

Render_App* GameManager::GetRenderer()
{
	if (m_pRenderer == nullptr)
		return nullptr; 

	return m_pRenderer;
}

GameTimer* GameManager::GetGameTimer()
{
	return m_gameTime;
}

void GameManager::Init()
{
	GetSceneManagerInstance();
	InputSystem::Create();
	m_gameTime = new GameTimer;
	m_gameTime->Reset();
	m_pRenderer = new Render_App;
	m_pRenderer->Init();
}

void GameManager::Run()
{
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		m_gameTime->Tick();
		float totalTime = m_gameTime->TotalTime();
		float dt = m_gameTime->DeltaTime();
		Update(dt);
		Draw();
	}
}

void GameManager::Update(float _dt)
{
	InputSystem::HandleInputs();
	m_pSceneManagerInstance->GetCurrentScene()->Update(_dt);
	m_pRenderer->Update();
	
}

void GameManager::Draw()
{
	m_pSceneManagerInstance->GetCurrentScene()->Draw();
	m_pRenderer->Draw();
}


