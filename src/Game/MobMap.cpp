#include "pch.h"

MobMap::~MobMap()
{
	m_allies.clear();
	m_mobs.clear();
	m_pECS = nullptr;
}

void MobMap::Init(ECS* _ecs, RenderSystem* _rendersys, float _offsetMap)
{
	m_pECS = _ecs;
	
	_offset = (int)_offsetMap;
	int distanceBetweenWaves = (int)((RAIL_LENGHT * 2.0f) / 3.0f);
	m_posZ = _offsetMap;

	for (int s = 0; s < 3; s++)
	{
		for (int k = 0; k < NB_MOBS; k++)
		{
			Enemy* newMob = new Enemy;
			newMob->Init(_ecs, _rendersys);
			m_mobs.push_back(newMob);

			float randomZ = (float)(rand() % 25 + _offset);

			if (s == 2)
				m_mobs[k + (NB_MOBS * s)]->SetPosition((float)(rand() % 10 - 5), 1.f, randomZ);
			else if (s == 1)
				m_mobs[k + (NB_MOBS * s)]->SetPosition((float)(rand() % 10 - 5), 1.f, randomZ);
			else
				m_mobs[k + (NB_MOBS * s)]->SetPosition((float)(rand() % 10 - 5), 1.f, randomZ);
		}
		for (int j = 0; j < NB_ALLIES; j++)
		{
			Ally* newAlly = new Ally;
			newAlly->Init(_ecs, _rendersys);
			m_allies.push_back(newAlly);

			float randomZ = (float)(rand() % 25 + _offset);

			if (s == 2)
				m_allies[j + (NB_ALLIES * s)]->SetPosition((float)(rand() % 10 - 5), 1.f, randomZ);
			else if (s == 1)
				m_allies[j + (NB_ALLIES * s)]->SetPosition((float)(rand() % 10 - 5), 1.f, randomZ);
			else
				m_allies[j + (NB_ALLIES * s)]->SetPosition((float)(rand() % 10 - 5), 1.f, randomZ);
		}

		m_wallName = "wall";
		m_pWall = new Mesh;
		m_pWall->SetRenderSystem(_rendersys);
		m_pWall->CreateQuad(10.f, 5.f, 0.1f, BROWN, m_wallName);

		for (int i = 0; i < 2; i++)
		{
			Entity* wall = m_pECS->createEntity();
			m_pECS->addComponent<Transform>(wall);
			m_pECS->addComponent<MeshRenderer>(wall);
			Transform* transform = m_pECS->GetComponent<Transform>(*wall);
			transform->mWorldMatrix = MathHelper::Identity4x4();
			if (i == 0)
			{
				transform->mPos = { -7.f, 2.5f, (float)(_offset - 5) };
			}
			else
			{
				transform->mPos = { 7.f, 2.5f, (float)(_offset - 5) };
			}

			MeshRenderer* meshRenderer = m_pECS->GetComponent<MeshRenderer>(*wall);
			meshRenderer->SetMesh(m_pWall);
			meshRenderer->SetType(QUAD);
			meshRenderer->SetMeshName(m_wallName);
			meshRenderer->SetMaterial("default");
			meshRenderer->SetVisible(true);
		}

		_offset += distanceBetweenWaves;
	}
}