#pragma once
#include "../Render/GameTimer.h"
class InputSystem;
class SceneManager;
class Render_App;

class GameManager
{
private: 
	static GameManager* m_pInstance;
	static SceneManager* m_pSceneManagerInstance;
	static Render_App* m_pRenderer;
	static GameTimer* m_gameTime;
public:

	GameManager();
	~GameManager() ;
	static GameManager* GetInstance();
	static SceneManager* GetSceneManagerInstance();
	static Render_App* GetRenderer();
	static GameTimer* GetGameTimer();
	void Init();
	void Run();
	void Update(float _dt);
	void Draw();
};