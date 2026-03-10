#pragma once

enum ID {
	SCENE_NONE,
	MAIN_MENU,
	DEATH_MENU,
	PAUSE_MENU,
	GAME_SCENE
};

class Scene
{
private:

public:

	virtual void Init() = 0;

	virtual void Update(float _dt) = 0;
	
	virtual void Draw() = 0;

	virtual void Delete() = 0;

};

