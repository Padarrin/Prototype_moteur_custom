#include "pch.h"
#include "DeathMenu.h"

void DeathMenu::Init()
{
	m_pGameController = new GameController;
	m_pGameController->Init(DEATH_MENU);
}

void DeathMenu::Update(float _dt)
{
	m_pGameController->Update(_dt);
}

void DeathMenu::Draw()
{
	float wndWidth = (float)GameManager::GetRenderer()->GetWindow()->GetWidth();
	float wndHeight = (float)GameManager::GetRenderer()->GetWindow()->GetHeight();
	XMFLOAT2 center = { wndWidth / 2, wndHeight / 2 };
	GameManager::GetRenderer()->DrawTextUI("GAME OVER", wndWidth / 2 - 124.f, 10.f, 32.0f, RED);
	GameManager::GetRenderer()->DrawTextUI("press ENTER to play again", center.x - 194.f, center.y, 32.f, WHITE);
}

void DeathMenu::Delete()
{
	delete m_pGameController;
	m_pGameController = nullptr;
}
