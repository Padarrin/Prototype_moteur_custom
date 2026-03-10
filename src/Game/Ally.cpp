#include "pch.h"

void Ally::Init(ECS* _ECS, RenderSystem* _rendersys)
{
	m_pECS = _ECS;
	m_pAlly = m_pECS->createEntity();
	std::string allyName = "ally";
	m_pMesh = new Mesh;
	m_pMesh->SetRenderSystem(_rendersys);
	m_pMesh->CreateQuad(0.875f, 1.5f, 1.f, BLUE, allyName);
	//m_pMesh->CreateCube(2.f, BLUE, allyName);
	m_pECS->addComponent<MeshRenderer>(m_pAlly);
	m_pECS->addComponent<Transform>(m_pAlly);
	m_pTransform = m_pECS->GetComponent<Transform>(*m_pAlly);
	m_pTransform->mWorldMatrix = MathHelper::Identity4x4();
	MeshRenderer* meshRenderer = m_pECS->GetComponent<MeshRenderer>(*m_pAlly);
	meshRenderer->SetMesh(m_pMesh);
	meshRenderer->SetType(QUAD);
	meshRenderer->SetMaterial("default");
	meshRenderer->SetVisible(true);
	meshRenderer->SetMeshName(allyName);
	m_pECS->addCollider(m_pAlly);
	m_pECS->GetCollider(m_pAlly)->mType = ColliderType::Type::Ally;
}

void Ally::SetPosition(float x, float y, float z)
{
	m_pTransform->mPos = { x, y, z };
}

void Ally::SetRotation(float _pitch, float _yaw, float _row)
{
	m_pTransform->mRot = { _pitch, _yaw, _row };
}