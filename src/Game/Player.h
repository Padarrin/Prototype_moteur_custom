#pragma once
#include "DirectXMath.h"
class Render_Camera;
struct Entity;
class ECS; 
struct Mesh;
class RenderSystem;
class Gun;

using namespace DirectX;
class Player
{

private : 

	ECS* m_pECS = nullptr;
	RenderSystem* m_pRenderSystem = nullptr;
	Mesh* m_pMeshPlayer = nullptr;
	Entity* m_pPlayer = nullptr;

	Render_Camera* m_pCam = nullptr;

	//Gun* m_pGun = nullptr;

	//float m_minecartSpeed = 15.f;
	int m_score = 0;

public :
	~Player();
	void Init(ECS* _ECS, Entity* _player, Render_Camera* _cam, RenderSystem* _rendersys);
	void Update(float _dt);
	void SetPosition(float x, float y, float z);
	void SetRotation(float _pitch, float _yaw, float _row);
	void Shoot(float _dt);
	void Reload(float _dt);
	void SetScore(int _score) { m_score = _score; }
	int GetScore() const { return m_score; }
	int GetMagazine() const;
	bool GetIsReloading() const;
	void TakeDamage();

	//For test 
	Render_Camera* GetCam();

	XMFLOAT3 GetPosition();
};

