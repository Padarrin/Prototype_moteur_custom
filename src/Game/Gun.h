#pragma once
class Bullet;

class Gun
{
private:
	ECS* m_pECS;
	Entity* m_pEntity;
	Entity* m_pPlayerEntity;
	Transform* m_pTransform;
	RenderSystem* m_pRenderSystem;

	Mesh* m_pMesh;
	

	std::vector<Bullet*> m_bullets;

	float m_reloadDuration = 1.5f;
	float m_currentRealoadTime = 0.f;
	
	
	float m_timeBeforeShoot = 0.25f;
	float m_currentTimeShoot = 0.25f;
	int m_magazine = 6;

public :
	~Gun();
	bool m_isReloading = false;
	void Init(ECS* _ecs, RenderSystem* _renderSystem, Entity* _player);
	void Update(float deltaTime);
	Entity* GetEntity();

	void CreateBullet(Render_Camera* _cam, XMFLOAT3 _spawnPos, XMFLOAT3 _inhertieVel, float _dt);
	bool ToDestroyBullet(Entity* _bullet);

	void Reload(float _dt);
	int GetMagazine();
};