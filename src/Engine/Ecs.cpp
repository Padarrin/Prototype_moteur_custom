#include "pch.h"

ECS::ECS()
{
}

ECS::~ECS()
{
	m_entities.clear();
	m_systems.clear();
	m_colliders.clear();
}

Entity* ECS::createEntity()
{
	Entity* p = new Entity;
	p->mIndex = _nbEntities;
	_nbEntities++;

	m_entities.push_back(p);
	return p;
}

void ECS::deleteEntity(Entity* instance)
{
	if (instance == nullptr) return;

	auto mapIt = m_components.find(instance->mIndex);
	if (mapIt != m_components.end())
	{
		for (Component* c : mapIt->second)
		{
			delete c; 
		}
		m_components.erase(mapIt); 
	}

	
	DeleteCollider(instance);

	
	auto it = std::find(m_entities.begin(), m_entities.end(), instance);
	if (it != m_entities.end())
	{
		delete* it;
		m_entities.erase(it);
	}
}

void ECS::addSystem(System* _system)
{
	m_systems.push_back(_system);
}

void ECS::Update(const float deltatime)
{
	for (int i = 0; i < m_systems.size(); i++)
	{
		m_systems[i]->Update(*this, deltatime);
	}

	CheckCollision();
}

void ECS::addCollider(Entity* _entity)
{
	Collider* collider = new Collider;
	collider->pECS = this;
	collider->pEntity = _entity;
	MeshRenderer* meshRenderer = this->GetComponent<MeshRenderer>(*_entity);
	CheckTypeMesh(meshRenderer, collider);
	collider->m_fCollision = [](Collider*, Collider*) {};
	m_colliders.push_back(collider);
}

void ECS::CheckTypeMesh(MeshRenderer* _mesh, Collider* _collider)
{
	Mesh* mesh = _mesh->GetMesh();
	if (_mesh->GetType() == SPHERE)
	{
		_collider->mRadius = mesh->radius;
	}
	else if (_mesh->GetType() == QUAD)
	{
		_collider->mRadius = mesh->height / 2.0f;
	}
	else if (_mesh->GetType() == CYLINDER)
	{
		_collider->mRadius = mesh->radius;
		_collider->mHalfHeight = mesh->height / 2.0f;
	}
}

Collider* ECS::GetCollider(Entity* _entity)
{
	for (int i = 0; i < m_colliders.size(); i++)
	{
		if (m_colliders[i]->pEntity == _entity)
		{
			return m_colliders[i];
		}
	}
	return nullptr;
}

void ECS::DeleteCollider(Entity* _entity)
{
	for (int i = 0; i < m_colliders.size(); i++)
	{
		if (m_colliders[i]->pEntity == _entity)
		{
			// delete m_colliders[i];
			m_colliders.erase(m_colliders.begin() + i);
			return;
		}
	}
}

void ECS::CheckCollision()
{
	if(m_colliders.size() < 2)
		return;

	for (int i = 0; i < m_colliders.size(); i++)
	{
		for (int j = i + 1; j < m_colliders.size(); j++)
		{
			if (m_colliders[i] == m_colliders[j])
				continue;

			if (m_colliders[i]->mIsActive == false || m_colliders[j]->mIsActive == false)
				continue;

			if(m_colliders[i] == nullptr || m_colliders[j] == nullptr)
				continue;

			XMFLOAT3 pos1 = m_colliders[i]->GetPosition();
			XMFLOAT3 pos2 = m_colliders[j]->GetPosition();

			XMFLOAT3 dir = pos2 - pos1;
			float dist = 0.0f;
			float minDist = 0.0f;

			// sphere-sphere collision
			if(GetComponent<MeshRenderer>(*(m_colliders[i]->pEntity))->GetType() == SPHERE && GetComponent<MeshRenderer>(*(m_colliders[j]->pEntity))->GetType() == SPHERE)
			{
				dist = sqrt((pos1.x - pos2.x) * (pos1.x - pos2.x) + (pos1.y - pos2.y) * (pos1.y - pos2.y) + (pos1.z - pos2.z) * (pos1.z - pos2.z));
				minDist = m_colliders[i]->GetRayon() + m_colliders[j]->GetRayon();

				if (dist <= minDist)
				{
					if (m_colliders[i]->m_fCollision)
						m_colliders[i]->m_fCollision(m_colliders[i], m_colliders[j]);
					if (m_colliders[j]->m_fCollision)
						m_colliders[j]->m_fCollision(m_colliders[j], m_colliders[i]);

					float overlap = 0.5f * (minDist - dist);
					dir = dir / dist;

					pos1 -= dir * overlap;
					pos2 += dir * overlap;

					GetComponent<Transform>(*(m_colliders[i]->pEntity))->mPos = pos1;
					GetComponent<Transform>(*(m_colliders[j]->pEntity))->mPos = pos2;
				}
			}
			
			// cube-cube collision
			if(GetComponent<MeshRenderer>(*(m_colliders[i]->pEntity))->GetType() == QUAD && GetComponent<MeshRenderer>(*(m_colliders[j]->pEntity))->GetType() == QUAD)
			{
				dist = std::max(std::abs(pos1.x - pos2.x), std::max(std::abs(pos1.y - pos2.y), std::abs(pos1.z - pos2.z)));
				minDist = m_colliders[i]->GetRayon() + m_colliders[j]->GetRayon();

				if (dist <= minDist)
				{
					if (m_colliders[i]->m_fCollision)
						m_colliders[i]->m_fCollision(m_colliders[i], m_colliders[j]);
					if (m_colliders[j]->m_fCollision)
						m_colliders[j]->m_fCollision(m_colliders[j], m_colliders[i]);

					float overlap = 0.5f * (minDist - dist);
					dir = dir / dist;

					pos1 -= dir * overlap;
					pos2 += dir * overlap;

					GetComponent<Transform>(*(m_colliders[i]->pEntity))->mPos = pos1;
					GetComponent<Transform>(*(m_colliders[j]->pEntity))->mPos = pos2;
				}
			}

			// Check if one collider is a cube and the other is a sphere
			bool isI_Cube = GetComponent<MeshRenderer>(*(m_colliders[i]->pEntity))->GetType() == QUAD;
			bool isJ_Cube = GetComponent<MeshRenderer>(*(m_colliders[j]->pEntity))->GetType() == QUAD;
			bool isJ_Sphere = GetComponent<MeshRenderer>(*(m_colliders[j]->pEntity))->GetType() == SPHERE;
			bool isI_Sphere = GetComponent<MeshRenderer>(*(m_colliders[i]->pEntity))->GetType() == SPHERE;
			bool isJ_Cylinder = GetComponent<MeshRenderer>(*(m_colliders[j]->pEntity))->GetType() == CYLINDER;
			bool isI_Cylinder = GetComponent<MeshRenderer>(*(m_colliders[i]->pEntity))->GetType() == CYLINDER;
			
			// cube-sphere collision
			if ((isI_Cube && isJ_Sphere) || (isI_Sphere && isJ_Cube))
			{
				Collider* cubeCol = isI_Cube ? m_colliders[i] : m_colliders[j];
				Collider* sphereCol = isI_Cube ? m_colliders[j] : m_colliders[i];

				XMFLOAT3 cubePos = cubeCol->GetPosition();
				XMFLOAT3 spherePos = sphereCol->GetPosition();

				float cubeHalfSize = cubeCol->GetRayon();
				float sphereRadius = sphereCol->GetRayon();

				XMFLOAT3 closestPoint;
				closestPoint.x = std::max(cubePos.x - cubeHalfSize, std::min(spherePos.x, cubePos.x + cubeHalfSize));
				closestPoint.y = std::max(cubePos.y - cubeHalfSize, std::min(spherePos.y, cubePos.y + cubeHalfSize));
				closestPoint.z = std::max(cubePos.z - cubeHalfSize, std::min(spherePos.z, cubePos.z + cubeHalfSize));

				dir = spherePos - closestPoint;

				float distSq = (dir.x * dir.x) + (dir.y * dir.y) + (dir.z * dir.z);

				if (distSq <= (sphereRadius * sphereRadius))
				{
					if (m_colliders[i]->m_fCollision) m_colliders[i]->m_fCollision(m_colliders[i], m_colliders[j]);
					if (m_colliders[j]->m_fCollision) m_colliders[j]->m_fCollision(m_colliders[j], m_colliders[i]);

					dist = std::sqrt(distSq);

					float overlap = sphereRadius - dist;
					dir /= dist;

					cubePos -= dir * (overlap * 0.5f);
					spherePos += dir * (overlap * 0.5f);

					if (sphereCol->mType == ColliderType::Type::Bullet)
					{
						if (cubeCol->mType == ColliderType::Type::Obstacle)
						{
							cubeCol->mIsActive = false;

							auto it = std::find(m_colliders.begin(), m_colliders.end(), cubeCol);
							if (it != m_colliders.end())
							{
								m_colliders.erase(it);
								deleteEntity(cubeCol->pEntity);
							}

							m_score += 10;
						}
						else if (cubeCol->mType == ColliderType::Type::Ally)
						{
							cubeCol->mIsActive = false;

							auto it = std::find(m_colliders.begin(), m_colliders.end(), cubeCol);
							if (it != m_colliders.end())
							{
								m_colliders.erase(it);
								deleteEntity(cubeCol->pEntity);
							}

							m_score -= 10;

							for(Entity* entity : m_entities)
							{
								Health* health = GetComponent<Health>(*entity);
								if (health != nullptr)
								{
									health->health -= 1;
								}
							}
						}

						sphereCol->mIsActive = false;

						if (m_colliders.size() > 0)
						{
							auto it = std::find(m_colliders.begin(), m_colliders.end(), sphereCol);
							if (it != m_colliders.end())
							{
								m_colliders.erase(it);
								deleteEntity(sphereCol->pEntity);
							}
						}
					}
					else
					{
						GetComponent<Transform>(*(cubeCol->pEntity))->mPos = cubePos;
						GetComponent<Transform>(*(sphereCol->pEntity))->mPos = spherePos;
					}
				}
			}

			// cube-cylinder collision
			if(isI_Cube && isJ_Cylinder || isI_Cylinder && isJ_Cube)
			{
				Collider* cubeCol = isI_Cube ? m_colliders[i] : m_colliders[j];
				Collider* cylCol = isI_Cube ? m_colliders[j] : m_colliders[i];

				XMFLOAT3 cubePos = cubeCol->GetPosition();
				XMFLOAT3 cylPos = cylCol->GetPosition();

				float cubeHalfSize = cubeCol->GetRayon();
				float cylRadius = cylCol->GetRayon();
				float cylHalfHeight = cylCol->mHalfHeight;

				float distY = std::abs(cylPos.y - cubePos.y);
				float minYDist = cubeHalfSize + cylHalfHeight;

				if (distY <= minYDist)
				{
					XMFLOAT3 closestPoint;
					closestPoint.x = std::max(cubePos.x - cubeHalfSize, std::min(cylPos.x, cubePos.x + cubeHalfSize));
					closestPoint.z = std::max(cubePos.z - cubeHalfSize, std::min(cylPos.z, cubePos.z + cubeHalfSize));

					float dirX = cylPos.x - closestPoint.x;
					float dirZ = cylPos.z - closestPoint.z;

					float distSq2D = (dirX * dirX) + (dirZ * dirZ);

					if (distSq2D <= (cylRadius * cylRadius))
					{
						if (m_colliders[i]->m_fCollision) m_colliders[i]->m_fCollision(m_colliders[i], m_colliders[j]);
						if (m_colliders[j]->m_fCollision) m_colliders[j]->m_fCollision(m_colliders[j], m_colliders[i]);

						float overlapY = minYDist - distY;

						float dist2D = std::sqrt(distSq2D);
						if (dist2D == 0.0f) { dirX = 1.0f; dirZ = 0.0f; dist2D = 0.001f; }

						float overlapXZ = cylRadius - dist2D; 


						if (overlapXZ < overlapY)
						{
							dirX /= dist2D;
							dirZ /= dist2D;

							cubePos.x -= dirX * (overlapXZ * 0.5f);
							cubePos.z -= dirZ * (overlapXZ * 0.5f);

							cylPos.x += dirX * (overlapXZ * 0.5f);
							cylPos.z += dirZ * (overlapXZ * 0.5f);
						}
						else
						{
							float signY = (cylPos.y > cubePos.y) ? 1.0f : -1.0f;

							cubePos.y -= signY * (overlapY * 0.5f);
							cylPos.y += signY * (overlapY * 0.5f);
						}

						if (cylCol->mType == ColliderType::Type::Bullet)
						{
							if(cubeCol->mType == ColliderType::Type::Obstacle)
							{
								cubeCol->mIsActive = false;

								auto it = std::find(m_colliders.begin(), m_colliders.end(), cubeCol);
								if (it != m_colliders.end())
								{
									m_colliders.erase(it);
									deleteEntity(cubeCol->pEntity);
								}

								m_score += 10;
							}
							else if (cubeCol->mType == ColliderType::Type::Ally)
							{
								cubeCol->mIsActive = false;

								auto it = std::find(m_colliders.begin(), m_colliders.end(), cubeCol);
								if (it != m_colliders.end())
								{
									m_colliders.erase(it);
									deleteEntity(cubeCol->pEntity);
								}

								m_score -= 10;
							}

							cylCol->mIsActive = false;

							if (m_colliders.size() > 0)
							{
								auto it = std::find(m_colliders.begin(), m_colliders.end(), cylCol);
								if (it != m_colliders.end())
								{
									m_colliders.erase(it);
									deleteEntity(cylCol->pEntity);
								}
							}							
						}
						else
						{
							GetComponent<Transform>(*(cubeCol->pEntity))->mPos = cubePos;
							GetComponent<Transform>(*(cylCol->pEntity))->mPos = cylPos;
						}
					}
				}
			}

			// sphere-cylinder collision
			if (isI_Sphere && isJ_Cylinder || isI_Cylinder && isJ_Sphere)
			{
				Collider* sphereCol = isI_Sphere ? m_colliders[i] : m_colliders[j];
				Collider* cylCol = isI_Sphere ? m_colliders[j] : m_colliders[i];

				XMFLOAT3 spherePos = sphereCol->GetPosition();
				XMFLOAT3 cylPos = cylCol->GetPosition();

				float sphereRadius = sphereCol->GetRayon();
				float cylRadius = cylCol->GetRayon();
				float cylHalfHeight = cylCol->mHalfHeight;

				float distY = std::abs(spherePos.y - cylPos.y);
				float minYDist = sphereRadius + cylHalfHeight;

				if (distY <= minYDist)
				{
					float dirX = spherePos.x - cylPos.x;
					float dirZ = spherePos.z - cylPos.z;

					float distSq2D = (dirX * dirX) + (dirZ * dirZ);
					float minDist2D = sphereRadius + cylRadius;

					if (distSq2D <= (minDist2D * minDist2D))
					{
						if (m_colliders[i]->m_fCollision) m_colliders[i]->m_fCollision(m_colliders[i], m_colliders[j]);
						if (m_colliders[j]->m_fCollision) m_colliders[j]->m_fCollision(m_colliders[j], m_colliders[i]);

						float dist2D = std::sqrt(distSq2D);
						if (dist2D == 0.0f) { dirX = 1.0f; dirZ = 0.0f; dist2D = 0.001f; }

						float overlap2D = minDist2D - dist2D;
						float overlapY = minYDist - distY;

						dirX /= dist2D;
						dirZ /= dist2D;

						spherePos.x += dirX * (overlap2D * 0.5f);
						spherePos.z += dirZ * (overlap2D * 0.5f);
						cylPos.x -= dirX * (overlap2D * 0.5f);
						cylPos.z -= dirZ * (overlap2D * 0.5f);

					}
				}
			}
		}
	}
}