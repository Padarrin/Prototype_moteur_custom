#pragma once

class Enemy
{
private:

	ECS* m_pECS = nullptr;
	Mesh* m_pMesh = nullptr;
	

	Transform* m_pTransform = nullptr;

public:
	Entity* m_pEnemy = nullptr;

	void Init(ECS* _ECS, RenderSystem* _rendersys);
	void SetPosition(float x, float y, float z);
	void SetRotation(float _pitch, float _yaw, float _row);
};