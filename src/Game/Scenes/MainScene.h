#pragma once
class Scene;
class PlayerController;
class Player;
class Enemy;
class GameMap;
class MobMap;
class Gun;
class HealthSystem;
class CleanUpSystem;
class GameController;

class MainScene : public Scene
{

private:
	ECS* m_pECS = nullptr;
	Mesh* m_pWoodRailMesh;
	Mesh* m_pMetalRailMesh;
	Mesh* m_pGround;
	Mesh* m_pCactus;
public:
	RenderSystem* m_pRenderSystem = nullptr;
private:
	TransformSystem* m_pTransformSystem = nullptr;
	HealthSystem* m_pHealthSystem = nullptr;
	CleanUpSystem* m_pCleanUpSystem = nullptr;

	GameMap* m_pMap = nullptr;


	std::vector<GameMap*> m_maps;
	MobMap* m_pMobMap = nullptr;
	std::vector<MobMap*> m_mobMaps;

	Entity* m_pPlayerEntity = nullptr;
	Player* m_pPlayer = nullptr;
	PlayerController* m_pPlayerController = nullptr;
	GameController* m_pGameController = nullptr;

	XMFLOAT4 m_crosshairColor = WHITE;


	float m_countToCreateNewMap;
	float m_countToCreateNewMobMap;
	float m_LastPlayerPosZ;
	int m_nbMaps = 1;
	int m_nbMobMaps = 1;
	float m_distanceBetweenMaps = RAIL_LENGHT * 2.0f;

	bool m_isFinish = false;

public :
	~MainScene();
	void Init() override;
	void Update(float _dt) override;
	void Draw() override;
	void Delete() override;
	bool MapsToDelete(GameMap* _map);
	bool MobMapsToDelete(MobMap* _mobMap);

};

