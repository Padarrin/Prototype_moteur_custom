#include "pch.h"
#include "Bullet.h"

Gun::~Gun()
{
	delete m_pEntity;
	delete m_pTransform;
	m_pECS = nullptr;
	m_pEntity = nullptr;
	m_pPlayerEntity = nullptr;
	m_pTransform = nullptr;

	m_bullets.clear();
}

void Gun::Init(ECS* _ecs, RenderSystem* _renderSystem, Entity* _player)
{
	m_pECS = _ecs;
	m_pRenderSystem = _renderSystem;
	m_pMesh = new Mesh;
	std::string meshName = "meshName";
	m_pMesh->SetRenderSystem(m_pRenderSystem);
	m_pMesh->CreateQuad(0.05f, 0.05f, 0.15f, BLACK, meshName);
	 
	m_pEntity = m_pECS->createEntity();
	m_pECS->addComponent<Transform>(m_pEntity);
	m_pECS->addComponent<MeshRenderer>(m_pEntity);
	m_pTransform = m_pECS->GetComponent<Transform>(*m_pEntity);
	MeshRenderer* meshRenderer = m_pECS->GetComponent<MeshRenderer>(*m_pEntity);
	meshRenderer->SetMesh(m_pMesh);
	meshRenderer->SetType(QUAD);
	meshRenderer->SetMeshName(meshName);
	meshRenderer->SetMaterial("default");
	meshRenderer->SetVisible(true);

	m_pPlayerEntity = _player;
}


void Gun::Update(float _dt)
{

	if (m_isReloading == true || m_magazine <= 0)
	{
		Reload(_dt);
	}
	else
	{
		m_currentTimeShoot += _dt;
	}

	for (int i = 0; i < m_bullets.size(); i++)
	{
		
		if (ToDestroyBullet(m_bullets[i]->pEntity) == true)
		{
			MeshRenderer* mr = m_pECS->GetComponent<MeshRenderer>(*m_bullets[i]->pEntity);
			if (mr == nullptr)
				continue;
			mr->SetVisible(false);
			auto bullet = std::find(m_bullets.begin(), m_bullets.end(), m_bullets[i]);
			m_bullets.erase(bullet);
			continue;
		}
		m_bullets[i]->Update(_dt);
	}
}

Entity* Gun::GetEntity()
{
	return m_pEntity;
}

void Gun::CreateBullet(Render_Camera* _cam, XMFLOAT3 _spawnPos, XMFLOAT3 _inhertieVel, float _dt)
{
	if (m_isReloading == true || m_magazine <= 0)
		return;

	if (m_currentTimeShoot <= m_timeBeforeShoot)
		return;

	Bullet* bullet = new Bullet;
	XMFLOAT3 camPos = _cam->GetPosition();
	XMFLOAT3 camLook = _cam->GetLookVector();
	XMFLOAT3 camRight = _cam->GetRightVector();
	XMFLOAT3 camUp = _cam->GetUpVector();

	XMVECTOR startPos = XMLoadFloat3(&_spawnPos);
	
	XMVECTOR _camPos = XMLoadFloat3(&camPos);
	XMVECTOR _camLook = XMLoadFloat3(&camLook);
	XMVECTOR targetPos = _camPos + (_camLook * 100.0f);
	XMVECTOR bulletDir = XMVector3Normalize(targetPos - startPos);

	XMFLOAT3 finalDir;
	XMStoreFloat3(&finalDir, bulletDir);

	bullet->Init(m_pECS, _spawnPos, finalDir, _inhertieVel, m_pRenderSystem);
	m_bullets.push_back(bullet);
	m_currentTimeShoot = 0.f;
	m_magazine--;
}

bool Gun::ToDestroyBullet(Entity* _bullet)
{
	if (_bullet == nullptr) return true;
	Transform* bulletTransform = m_pECS->GetComponent<Transform>(*_bullet);
	
	Transform* playerTransform = m_pECS->GetComponent<Transform>(*m_pPlayerEntity);
	if (!bulletTransform || !playerTransform) 
		return true;
	


	float dx = abs(bulletTransform->mPos.x - playerTransform->mPos.x);
	float dy = abs(bulletTransform->mPos.y - playerTransform->mPos.y);
	float dz = abs(bulletTransform->mPos.z - playerTransform->mPos.z);
	float distance = sqrt(dx * dx + dy * dy + dz * dz);

	if (distance >= 100.f)
	{
		return true;
	}
	return false;
}

void Gun::Reload(float _dt)
{
	m_currentRealoadTime += _dt;
	m_isReloading = true;
	if (m_currentRealoadTime >= m_reloadDuration)
	{
		m_currentRealoadTime = 0.f;
		m_magazine = 6;
		m_isReloading = false;
	}
}

int Gun::GetMagazine()
{
	return m_magazine;
}
