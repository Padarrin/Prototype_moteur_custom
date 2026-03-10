#include "pch.h"
#include "MainScene.h"
#include "../Engine/GameManager.h"

MainScene::~MainScene()
{
	Delete();
}

void MainScene::Init()
{
	srand(time(0));

	std::cout << "Scene Init" << std::endl;
	m_pECS = new ECS;
	m_pRenderSystem = new RenderSystem;
	m_pRenderSystem->SetRenderer(GameManager::GetRenderer());
	m_pTransformSystem = new TransformSystem;
	m_pHealthSystem = new HealthSystem;
	m_pCleanUpSystem = new CleanUpSystem;
	m_pECS->addSystem(m_pRenderSystem);
	m_pECS->addSystem(m_pTransformSystem);
	m_pECS->addSystem(m_pHealthSystem);
	m_pECS->addSystem(m_pCleanUpSystem);

	std::string woodRailName = "woodRail";
	m_pWoodRailMesh = new Mesh;
	m_pWoodRailMesh->SetRenderSystem(m_pRenderSystem);
	m_pWoodRailMesh->CreateQuad(1.75f, 0.1f, WOOD_DEPTH_SIZE, BROWN, woodRailName);

	std::string metalRailName = "metalRail";
	m_pMetalRailMesh = new Mesh;
	m_pMetalRailMesh->SetRenderSystem(m_pRenderSystem);
	m_pMetalRailMesh->CreateQuad(0.15f, 0.15f, RAIL_LENGHT, GREY, metalRailName);

	std::string groundName = "ground";
	m_pGround = new Mesh;
	m_pGround->SetRenderSystem(m_pRenderSystem);
	m_pGround->CreateQuad(5000.f, 0.1f, 5000.f, SAND, groundName);

	std::string cactusName = "cactus";
	m_pCactus = new Mesh;
	m_pCactus->SetRenderSystem(m_pRenderSystem);
	m_pCactus->CreateCylinder(1.f, 5.f, 36, CACTUS, cactusName);

	Entity* ground = m_pECS->createEntity();
	m_pECS->addComponent<MeshRenderer>(ground);
	m_pECS->addComponent<Transform>(ground);

	Transform* transform = m_pECS->GetComponent<Transform>(*ground);
	transform->mWorldMatrix = MathHelper::Identity4x4();
	transform->mPos = { 0.f,-0.1f,0.f };
	transform->mSca.x *= 5;
	transform->mSca.z *= 5;

	MeshRenderer* meshRenderer = m_pECS->GetComponent<MeshRenderer>(*ground);
	meshRenderer->SetMesh(m_pGround);
	meshRenderer->SetType(QUAD);
	meshRenderer->SetMeshName(groundName);
	meshRenderer->SetMaterial("default");
	meshRenderer->SetVisible(true);

	GameMap* newRail = new GameMap;
	newRail->Init(m_pECS, m_pRenderSystem, { 0.f, 0.f, -RAIL_LENGHT / 2 }, m_pWoodRailMesh, m_pMetalRailMesh, m_pCactus);
	m_maps.push_back(newRail);

	MobMap* newMobMap = new MobMap;
	newMobMap->Init(m_pECS, m_pRenderSystem, RAIL_LENGHT);
	m_mobMaps.push_back(newMobMap);

	m_pPlayerEntity = m_pECS->createEntity();
	m_pECS->addComponent<Transform>(m_pPlayerEntity);
	m_pECS->addComponent<Health>(m_pPlayerEntity);
	Health* health = m_pECS->GetComponent<Health>(*m_pPlayerEntity);
	health->maxHealth = 3;
	health->health = health->maxHealth;

	m_pPlayer = new Player;
	m_pPlayer->Init(m_pECS, m_pPlayerEntity, GameManager::GetRenderer()->GetCamera(), m_pRenderSystem);
	m_pPlayer->SetPosition(0.f, 2.f, -RAIL_LENGHT/2.f);
	m_LastPlayerPosZ = -RAIL_LENGHT / 2.f;

	m_pPlayerController = new PlayerController;
	m_pPlayerController->Init(m_pPlayer);

	m_pGameController = new GameController;
	m_pGameController->Init(GAME_SCENE);
}

void MainScene::Update(float _dt)
{
	//std::cout << "Scene Update" << std::endl;
	m_pGameController->Update(_dt);
	
	if (m_isFinish == true)
	{
		m_pGameController->m_cursorLock = false;
		return;
	}

	Health* health = m_pECS->GetComponent<Health>(*m_pPlayerEntity);
	if (health->health <= 0)
	{
		Delete();
		m_isFinish = true;
		SceneManager* sm = GameManager::GetSceneManagerInstance();

		DeathMenu* nextScene = new DeathMenu;
		sm->AddScene(nextScene);
		sm->SetCurrentScene(nextScene);
		return;
	}

	Transform* transformPlayer = m_pECS->GetComponent<Transform>(*m_pPlayerEntity);
	float travel = abs(transformPlayer->mPos.z - m_LastPlayerPosZ);
	m_LastPlayerPosZ = transformPlayer->mPos.z;
	m_countToCreateNewMap += travel;
	m_countToCreateNewMobMap += travel;
	if (m_countToCreateNewMap >= HALF_RAIL_LENGT)
	{
		GameMap* newRail = new GameMap;
		newRail->Init(m_pECS, m_pRenderSystem, { 0.f, 0.f, -HALF_RAIL_LENGT + (RAIL_LENGHT * m_nbMaps)}, m_pWoodRailMesh, m_pMetalRailMesh, m_pCactus);
		m_maps.push_back(newRail);

		m_nbMaps++;
		m_countToCreateNewMap = 0.f;
	}

	if (m_countToCreateNewMobMap >= RAIL_LENGHT * 2.0f)
	{
		MobMap* newMobMap = new MobMap;

		float nextChunkZ = RAIL_LENGHT + (RAIL_LENGHT * 2.0f * m_nbMobMaps);

		newMobMap->Init(m_pECS, m_pRenderSystem, nextChunkZ);
		m_mobMaps.push_back(newMobMap);
		m_nbMobMaps++;
		m_countToCreateNewMobMap = 0.f;

		if (m_mobMaps.size() > 3)
		{
			delete m_mobMaps.front();
			m_mobMaps.erase(m_mobMaps.begin());
		}
	}

	for (auto it = m_maps.begin(); it != m_maps.end(); )
	{
		GameMap* map = *it;

		if (MapsToDelete(map) == true)
		{
			for (Entity* entity : map->m_entities)
			{
				m_pECS->deleteEntity(entity);
			}
			delete map; 
			it = m_maps.erase(it); 
		}
		else
		{
			
			map->Update(_dt, transformPlayer->mPos);
			++it; 
		}
	}

	for (auto it = m_mobMaps.begin(); it != m_mobMaps.end(); )
	{
		MobMap* mob = *it;

		if (MobMapsToDelete(mob) == true)
		{
			for (Ally* ally : mob->m_allies)
			{
				m_pECS->deleteEntity(ally->m_pAlly);
			}
			for (Enemy* enemy : mob->m_mobs)
			{
				m_pECS->deleteEntity(enemy->m_pEnemy);
			}
			delete mob;
			it = m_mobMaps.erase(it);
		}
		else
		{
			++it;
		}
	}


	if (m_pPlayer->GetIsReloading() == true)
	{
		m_crosshairColor = GREY;
	}
	else
	{
		m_crosshairColor = WHITE;
	}
	
	m_pPlayerController->Update(_dt);
	m_pPlayer->Update(_dt);
	m_pECS->Update(_dt);
}

void MainScene::Draw()
{
	if (m_isFinish == true)
		return;
	//std::cout << "Scene Draw" << std::endl;
	float wndWidth = GameManager::GetRenderer()->GetWindow()->GetWidth();
	float wndHeight = GameManager::GetRenderer()->GetWindow()->GetHeight();
	XMFLOAT2 center = { wndWidth / 2, wndHeight / 2 };

	if (m_pGameController->m_gameStop == false)
	{
		GameManager::GetRenderer()->DrawTextUI("+", center.x - 16.f, center.y - 16.f, 32.f, m_crosshairColor);
	}
	else
	{
		GameManager::GetRenderer()->DrawTextUI("press ESCAPE to resume", center.x - 204.f, center.y, 32.f, WHITE);
	}

	std::string score = "SCORE : " + std::to_string( m_pPlayer->GetScore());
	GameManager::GetRenderer()->DrawTextUI(score, center.x - 124.f, 10.f, 32.f, WHITE);

	std::string magazine = std::to_string(m_pPlayer->GetMagazine()) + "/6";
	GameManager::GetRenderer()->DrawTextUI(magazine, 50.f, wndHeight - 100.f, 64.f, WHITE);

	std::string health;
	for(int i = 0; i < m_pECS->GetComponent<Health>(*m_pPlayerEntity)->health; i++)
	{
		health += "I";
	}
	GameManager::GetRenderer()->DrawTextUI(health, wndWidth - 150.f, wndHeight - 100.f, 64.f, RED);
}

void MainScene::Delete()
{
	delete m_pECS;
	delete m_pRenderSystem;
	delete m_pTransformSystem;
	delete m_pHealthSystem;
	delete m_pCleanUpSystem;
	m_pECS = nullptr;
	m_pRenderSystem = nullptr;
	m_pTransformSystem = nullptr;
	m_pHealthSystem = nullptr;
	m_pCleanUpSystem = nullptr;

	delete m_pMap;
	delete m_pMobMap;
	m_pMap = nullptr;
	m_pMobMap = nullptr;

	delete m_pPlayer;
	m_pPlayer = nullptr;

	delete m_pPlayerController;
	m_pPlayerController = nullptr;

	for (GameMap* map : m_maps)
	{
		delete map;
	}
	m_maps.clear();

	for (MobMap* mobMap : m_mobMaps)
	{
		delete mobMap;
	}
	m_mobMaps.clear();
}

bool MainScene::MapsToDelete(GameMap* _map)
{
	XMFLOAT3 mapPos = _map->GetSpawnPos();
	Transform* transformPlayer = m_pECS->GetComponent<Transform>(*m_pPlayerEntity);

	if (mapPos.z < transformPlayer->mPos.z - 175.f)
	{
		return true;
	}
	return false;
}

bool MainScene::MobMapsToDelete(MobMap* _mobMap)
{
	float mapPos = _mobMap->m_posZ;
	Transform* transformPlayer = m_pECS->GetComponent<Transform>(*m_pPlayerEntity);

	if (mapPos < transformPlayer->mPos.z - 300.f)
	{
		return true;
	}
	return false;
}
