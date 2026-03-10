#include "pch.h"
#include "main.h"
#include "../Engine/GameManager.h"
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")

//MAIN GAME
int main()
{
    GameManager::GetInstance()->Init();
    SceneManager* pSceneManagerInstance = GameManager::GetSceneManagerInstance();
    MainMenuScene* menuScene = new MainMenuScene;
    pSceneManagerInstance->AddScene(menuScene);
    pSceneManagerInstance->SetCurrentScene(menuScene);
    GameManager::GetInstance()->Run();
    return 0;
}