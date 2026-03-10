#include "pch.h"
#include "Bullet.h"

Bullet::~Bullet()
{
	pECS = nullptr;
	pTransform = nullptr;
	delete m_pBulletMesh;
	m_pBulletMesh = nullptr;
}

void Bullet::Init(ECS* _ecs, XMFLOAT3 _spawnPos, XMFLOAT3 _dir, XMFLOAT3 _inhertieVel, RenderSystem* _rendersys)
{
	pECS = _ecs;
	m_dir = _dir;
	m_InhertieVel = _inhertieVel;
	m_pBulletMesh = new Mesh;
	std::string meshBullet = "meshBullet";
	m_pBulletMesh->SetRenderSystem(_rendersys);
	m_pBulletMesh->CreateSphere(0.015f, 36, 36, WHITE, meshBullet);

	pEntity = pECS->createEntity();
	pECS->addComponent<Transform>(pEntity);
	pECS->addComponent<MeshRenderer>(pEntity);
	pTransform = pECS->GetComponent<Transform>(*pEntity);
	pTransform->mWorldMatrix = MathHelper::Identity4x4();
	pTransform->mPos = _spawnPos;
	pTransform->mRot.x = XM_PIDIV2;

	MeshRenderer* bulletMeshRenderer = pECS->GetComponent<MeshRenderer>(*pEntity);
	bulletMeshRenderer->SetMesh(m_pBulletMesh);
	bulletMeshRenderer->SetType(SPHERE);
	bulletMeshRenderer->SetMeshName("meshBullet");
	bulletMeshRenderer->SetMaterial("default");
	bulletMeshRenderer->SetVisible(true);

	pECS->addCollider(pEntity);
	pECS->GetCollider(pEntity)->mType = ColliderType::Type::Bullet;
}

void Bullet::Update(float deltaTime)
{
	float speed = 50.0f; 

	pTransform->mPos.x += (m_dir.x * speed + m_InhertieVel.x) * deltaTime;
	pTransform->mPos.y += (m_dir.y * speed + m_InhertieVel.y) * deltaTime;
	pTransform->mPos.z += (m_dir.z * speed + m_InhertieVel.z) * deltaTime;
}