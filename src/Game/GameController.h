#pragma once
#include "../Engine/Scene.h"
class GameController
{
private:
	
	ID m_currentSceneID = SCENE_NONE;

public:
	bool m_gameStop = false;
	bool m_cursorLock = true;
	bool m_ToDelete = false;


public :
	void Init(ID _currentSceneID);
	void Update(float _dt);
	void UpdateKeyboardInputs(float _dt);
	void UpdateMouseInputs(float _dt);
	void SetSceneID(ID _currentSceneID);

};

