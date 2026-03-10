#include "pch.h"

Player::~Player()
{
	//delete m_pGun;
	//delete m_pMineCart;
	delete m_pPlayer;
	m_pECS = nullptr;
	m_pPlayer = nullptr;
	//m_pMineCart = nullptr;
	//m_pGun = nullptr;
}

void Player::Init(ECS* _ECS, Entity* _player, Render_Camera* _cam, RenderSystem* _rendersys)
{
	m_pECS = _ECS;
	m_pCam = _cam;
	m_pRenderSystem = _rendersys;

	std::string minecartName = "minecart";
	m_pMeshPlayer = new Mesh;
	m_pMeshPlayer->SetRenderSystem(_rendersys);
	m_pMeshPlayer->CreateQuad(2.f, 2.f, 2.f, GREY, minecartName);

	/*m_pMineCart = m_pECS->createEntity();
	m_pECS->addComponent<MeshRenderer>(m_pMineCart);
	m_pECS->addComponent<Transform>(m_pMineCart);*/
	m_pPlayer = m_pECS->createEntity();
	m_pECS->addComponent<MeshRenderer>(m_pPlayer);
	m_pECS->addComponent<Transform>(m_pPlayer);

	Transform* tfm = m_pECS->GetComponent<Transform>(*m_pPlayer);
	tfm->mWorldMatrix = MathHelper::Identity4x4();

	MeshRenderer* meshRenderer = m_pECS->GetComponent<MeshRenderer>(*m_pPlayer);
	meshRenderer->SetMesh(m_pMeshPlayer);
	meshRenderer->SetType(QUAD);
	meshRenderer->SetMaterial("default");
	meshRenderer->SetVisible(true);
	meshRenderer->SetMeshName(minecartName);
	/*m_pGun = new Gun;
	m_pGun->Init(m_pECS, _rendersys, m_pPlayer);*/
}

void Player::Update(float _dt)
{
	if (m_pPlayer == nullptr)
		return;
	//SetPosition(0.f, 2.f, GetPosition().z + m_minecartSpeed * _dt);

	//Transform* transformGun = m_pECS->GetComponent<Transform>(*m_pGun->GetEntity());
	//if(transformGun->isScreenSpace == false)
		//transformGun->isScreenSpace = true;

	XMFLOAT3 camRot = m_pCam->GetRotation();
	float pitch = XMConvertToRadians(camRot.x);
	float yaw = XMConvertToRadians(camRot.y);

	XMMATRIX rot = XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
	XMMATRIX trans = XMMatrixTranslation(0.15f, -0.20f, 0.55f);
	XMMATRIX gunMatrix = rot * trans;

	//XMStoreFloat4x4(&transformGun->mWorldMatrix, gunMatrix);

	//m_pGun->Update(_dt);
	
	SetScore(m_pECS->GetScore());
}


void Player::SetPosition(float _x, float _y, float _z)
{
	Transform* transformPlayer = m_pECS->GetComponent<Transform>(*m_pPlayer);
	if (transformPlayer == nullptr)
		return;
	transformPlayer->mPos = { _x, _y, _z };
	
	//Transform* transformMinecart = m_pECS->GetComponent<Transform>(*m_pMineCart);
	//transformMinecart->mPos.z = transformPlayer->mPos.z;

	m_pCam->SetPosition(_x, _y + 5.0f, _z - 5.0f);
}

void Player::SetRotation(float _pitch, float _yaw, float _row)
{
	
	Transform* transform = m_pECS->GetComponent<Transform>(*m_pPlayer);
	if (transform == nullptr)
		return;
	transform->mRot = { 0.f, _yaw, _row };

	m_pCam->SetRotation(_pitch, _yaw, _row);
}

void Player::Shoot(float _dt)
{

	XMFLOAT3 camPos = m_pCam->GetPosition();
	XMFLOAT3 camRot = m_pCam->GetRotation();
	float pitch = XMConvertToRadians(camRot.x);
	float yaw = XMConvertToRadians(camRot.y);

	XMMATRIX rot = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);

	XMVECTOR localGunTip = XMVectorSet(0.30f, -0.20f, 0.95f + 0.15f, 0.0f); 
	XMVECTOR worldGunTip = XMVector3TransformCoord(localGunTip, rot);

	XMVECTOR camPosV = XMLoadFloat3(&camPos);
	XMVECTOR spawnPos = camPosV + worldGunTip;

	XMFLOAT3 finalSpawn;
	XMStoreFloat3(&finalSpawn, spawnPos);

	//XMFLOAT3 inhertieVel = { 0.f,0.f,m_minecartSpeed };

	//m_pGun->CreateBullet(m_pCam, finalSpawn, inhertieVel, _dt);
}

void Player::Reload(float _dt)
{
	//m_pGun->Reload(_dt);
}

Render_Camera* Player::GetCam()
{
	return m_pCam;
}

XMFLOAT3 Player::GetPosition()
{
	Transform* transform = m_pECS->GetComponent<Transform>(*m_pPlayer);
	if (transform == nullptr)
		return XMFLOAT3(0.f, 0.f, 0.f);
	return transform->mPos;
}

int Player::GetMagazine() const
{
	//return m_pGun->GetMagazine();
	return 0;
}

bool Player::GetIsReloading() const
{
	//return m_pGun->m_isReloading;
	return 0;
}

void Player::TakeDamage()
{
	Health* health = m_pECS->GetComponent<Health>(*m_pPlayer);
	if (health == nullptr)
		return;
	health->health--;
	if (health->health < 0)
		health = 0;
}


	