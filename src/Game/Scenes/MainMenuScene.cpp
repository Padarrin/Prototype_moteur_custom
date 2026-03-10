#include "pch.h"
#include "MainMenuScene.h"


void MainMenuScene::Init()
{
	std::cout << "Menu Init" << std::endl;
	m_pGameController = new GameController;
	m_pGameController->Init(MAIN_MENU);
}

void MainMenuScene::Update(float _dt)
{
	//std::cout << "Menu Update" << std::endl;
	m_pGameController->Update(_dt);
	if (m_pGameController->m_ToDelete == true)
	{
		Delete();
	}

}

void MainMenuScene::Draw()
{

	//std::cout << "Menu Draw" << std::endl;
	float wndWidth = (float)GameManager::GetRenderer()->GetWindow()->GetWidth();
	float wndHeight = (float)GameManager::GetRenderer()->GetWindow()->GetHeight();
	XMFLOAT2 center = { (float)wndWidth / 2.f, (float)wndHeight / 2.f };
	GameManager::GetRenderer()->DrawTextUI("LAW ON RAILS", wndWidth/2 - 124.f, 10.f, 32.0f, WHITE);
	GameManager::GetRenderer()->DrawTextUI("press ENTER to play", center.x - 194.f, center.y, 32.f, WHITE);
}

void MainMenuScene::Delete()
{
	delete m_pGameController;
	m_pGameController = nullptr;
}
