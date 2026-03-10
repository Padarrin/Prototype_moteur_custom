#pragma once
class Scene;
class GameController;
class MainMenuScene : public Scene
{
private:
	ECS* m_pECS = nullptr;
	GameController* m_pGameController = nullptr;

public:
	void Init() override;
	void Update(float _dt) override;
	void Draw() override;
	void Delete() override;
};

