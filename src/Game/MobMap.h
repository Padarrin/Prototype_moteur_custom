#pragma once

class MobMap
{
private:
	ECS* m_pECS = nullptr;

	float _mapSize = 0.f;
	int _offset = 0;
	

	Mesh* m_pWall = nullptr;
	std::string m_wallName;

public:
	float m_posZ;
	std::vector<Enemy*> m_mobs;
	std::vector<Ally*> m_allies;
	~MobMap();
	void Init(ECS* _ecs, RenderSystem* _rendersys, float _offsetMap);

};