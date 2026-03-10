#include "pch.h"
#include "SceneManager.h"

void SceneManager::Init()
{
	m_pCurrentScene->Init();
}

void SceneManager::Update(float _dt)
{
	m_pCurrentScene->Update(_dt);
}

void SceneManager::Draw()
{
	m_pCurrentScene->Draw();
}

void SceneManager::AddScene(Scene* _scene)
{
	auto it = std::find(m_pScenes.begin(), m_pScenes.end(), _scene);
	if (it != m_pScenes.end())
	{
		m_pScenes.push_back(_scene);
	}
}

void SceneManager::SetCurrentScene(Scene* _currScene)
{
	m_pCurrentScene = _currScene;
	Init();
}

Scene* SceneManager::GetCurrentScene()
{
	return m_pCurrentScene;
}
