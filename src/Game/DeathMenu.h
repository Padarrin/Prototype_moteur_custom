#pragma once
class Scene;
class GameController;

class DeathMenu : public Scene
{
private:
	GameController* m_pGameController = nullptr;
public:
	void Init() override;
	void Update(float _dt) override;
	void Draw() override;
	void Delete() override;
};

