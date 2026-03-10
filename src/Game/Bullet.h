#pragma once

class Bullet
{
private:
	ECS* pECS;
	Transform* pTransform;
	XMFLOAT3 m_dir;
	XMFLOAT3 m_InhertieVel;
	Mesh* m_pBulletMesh;

public:
	Entity* pEntity;

public:
	~Bullet();
	void Init(ECS* _ecs, XMFLOAT3 _spawnpos, XMFLOAT3 _dir, XMFLOAT3 _inhertieVel, RenderSystem* _rendersys);
	void Update(float deltaTime);
};