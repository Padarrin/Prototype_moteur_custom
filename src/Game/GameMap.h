#pragma once
#include "pch.h"

class GameMap
{
private :
	ECS* m_pECS = nullptr;

	
	std::string m_metalRailName;
	std::string m_groundName;
	std::string m_cactusName;

	XMFLOAT3 m_spawnPos;

public :

	std::vector<Entity*> m_entities;

	~GameMap();
	void Init(ECS* _ecs, RenderSystem* _rendersys, XMFLOAT3 m_spawnPos,Mesh* _woodMesh, Mesh* _metalMesh, Mesh* _cactusMesh);
	void Update(float _dt, XMFLOAT3 _playerPos);

	bool ToClip(XMFLOAT3 _ObjectPos, XMFLOAT3 _playerPos);
	XMFLOAT3 GetSpawnPos();
};

