#include "pch.h"

void Enemy::Init(ECS* _ECS, RenderSystem* _rendersys)
{
	m_pECS = _ECS;
	m_pEnemy = m_pECS->createEntity();
	std::string enemyName = "enemy";
	m_pMesh = new Mesh;
	m_pMesh->SetRenderSystem(_rendersys);
	m_pMesh->CreateQuad(0.875f, 1.5f, 1.f, RED, enemyName);
	//m_pMesh->CreateCube(2.f, RED, enemyName);
	m_pECS->addComponent<MeshRenderer>(m_pEnemy);
	m_pECS->addComponent<Transform>(m_pEnemy);
	m_pTransform = m_pECS->GetComponent<Transform>(*m_pEnemy);
	m_pTransform->mWorldMatrix = MathHelper::Identity4x4();
	MeshRenderer* meshRenderer = m_pECS->GetComponent<MeshRenderer>(*m_pEnemy);
	meshRenderer->SetMesh(m_pMesh);
	meshRenderer->SetType(QUAD);
	meshRenderer->SetMaterial("default");
	meshRenderer->SetVisible(true);
	meshRenderer->SetMeshName(enemyName);
	m_pECS->addCollider(m_pEnemy);
	m_pECS->GetCollider(m_pEnemy)->mType = ColliderType::Type::Obstacle;
}

void Enemy::SetPosition(float x, float y, float z)
{
	m_pTransform->mPos = { x, y, z };
}

void Enemy::SetRotation(float _pitch, float _yaw, float _row)
{
	m_pTransform->mRot = { _pitch, _yaw, _row };
}