#include "GameMap.h"

GameMap::~GameMap()
{
	m_entities.clear();
	m_pECS = nullptr;
}

void GameMap::Init(ECS* _ecs, RenderSystem* _rendersys, XMFLOAT3 _spawnPos, Mesh* _woodMesh, Mesh* _metalMesh, Mesh* _cactusMesh)
{
	m_pECS = _ecs;
	std::string woodRailName = "woodRail";
	m_spawnPos = _spawnPos;

	for (int i = 0; i < WOOD_NUMBER; i++)
	{
		Entity* woodPart = m_pECS->createEntity();
		m_pECS->addComponent<MeshRenderer>(woodPart);
		m_pECS->addComponent<Transform>(woodPart);

		Transform* transform = m_pECS->GetComponent<Transform>(*woodPart);
		transform->mWorldMatrix = MathHelper::Identity4x4();
		transform->mPos = _spawnPos;
		_spawnPos.z += OFFSET_BETWEEN_WOODS;

		if (i % 5 == 0)
		{
			_spawnPos.x = -0.05f;
		}
		else if (i % 7 == 0)
		{
			_spawnPos.x = 0.05f;
		}
		else
		{
			_spawnPos.x = 0.f;
		}

		MeshRenderer* meshRenderer = m_pECS->GetComponent<MeshRenderer>(*woodPart);
		meshRenderer->SetMesh(_woodMesh);
		meshRenderer->SetType(QUAD);
		meshRenderer->SetMeshName(woodRailName);
		meshRenderer->SetMaterial("default");
		meshRenderer->SetVisible(true);

		m_entities.push_back(woodPart);
	}

	

	for (int i = 0; i < 2; i++)
	{
		Entity* metalPart = m_pECS->createEntity();
		m_pECS->addComponent<MeshRenderer>(metalPart);
		m_pECS->addComponent<Transform>(metalPart);

		Transform* transform = m_pECS->GetComponent<Transform>(*metalPart);
		transform->mWorldMatrix = MathHelper::Identity4x4();
		if (i == 0)
		{
			transform->mPos = { -0.5f,0.3f, _spawnPos.z - HALF_RAIL_LENGT };
		}
		else
		{
			transform->mPos = { 0.5f,0.3f,_spawnPos.z - HALF_RAIL_LENGT };
		}


		MeshRenderer* meshRenderer = m_pECS->GetComponent<MeshRenderer>(*metalPart);
		meshRenderer->SetMesh(_metalMesh);
		meshRenderer->SetType(QUAD);
		meshRenderer->SetMeshName("metalRail");
		meshRenderer->SetMaterial("default");
		meshRenderer->SetVisible(true);

		m_entities.push_back(metalPart);
	}


	for (int i = 0; i < RAIL_LENGHT / 20; i++)
	{
		Entity* cactus = m_pECS->createEntity();
		m_pECS->addComponent<Transform>(cactus);
		m_pECS->addComponent<MeshRenderer>(cactus);

		Transform* transform = m_pECS->GetComponent<Transform>(*cactus);
		transform->mWorldMatrix = MathHelper::Identity4x4();

		int randomOffset = rand() % (int)RAIL_LENGHT - (int)(RAIL_LENGHT / 2); 

		if (i % 3 == 0)
		{
			transform->mPos = { -15.f, 2.5f, (float)randomOffset };
		}
		else if (i % 5 == 0)
		{
			transform->mPos = { 20.f, 2.5f, (float)randomOffset };
		}
		else if (i % 7 == 0)
		{
			transform->mPos = { -20.f, 2.5f, (float)randomOffset };
		}
		else
		{
			transform->mPos = { 15.f, 2.5f, (float)randomOffset };
		}

		MeshRenderer* meshRenderer = m_pECS->GetComponent<MeshRenderer>(*cactus);
		meshRenderer->SetMesh(_cactusMesh);
		meshRenderer->SetType(CYLINDER);
		meshRenderer->SetMeshName("cactus");
		meshRenderer->SetMaterial("default");
		meshRenderer->SetVisible(true);
		m_entities.push_back(cactus);
	}
}

void GameMap::Update(float _dt, XMFLOAT3 _playerPos)
{
	for (int i = 0; i < m_entities.size(); i++)
	{

		MeshRenderer* meshRenderer = m_pECS->GetComponent<MeshRenderer>(*m_entities[i]);
		if (meshRenderer == nullptr)
			continue;
		if (meshRenderer->GetMeshName() == "ground" || meshRenderer->GetMeshName() == "metalRail")
			continue;

		Transform* tfm = m_pECS->GetComponent<Transform>(*m_entities[i]);

		if (ToClip(tfm->mPos, _playerPos) == true)
		{
			
			meshRenderer->SetVisible(false);
		}
		else
		{
			meshRenderer->SetVisible(true);
		}
	}
}

bool GameMap::ToClip(XMFLOAT3 _ObjectPos, XMFLOAT3 _playerPos)
{

	float dx = abs(_ObjectPos.x - _playerPos.x);
	float dy = abs(_ObjectPos.y - _playerPos.y);
	float dz = abs(_ObjectPos.z - _playerPos.z);
	float distance = sqrt(dx * dx + dy * dy + dz * dz);

	if (distance >= 100.f)
	{
		return true;
	}
	return false;
}
XMFLOAT3 GameMap::GetSpawnPos()
{
	return m_spawnPos;
}
;
