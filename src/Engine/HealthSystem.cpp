#include "HealthSystem.h"

HealthSystem::~HealthSystem()
{
	m_entities.clear();
}

void HealthSystem::Init(ECS ecs)
{
}

void HealthSystem::Update(ECS ecs, float deltatime)
{
	m_entities = ecs.GetEntitiesWith<Health>();

	for (int i = 0; i < m_entities.size(); i++)
	{
		Health* health = ecs.GetComponent<Health>(*m_entities[i]);
		if (health->health <= 0)
		{
			health->isDead = true;
		}
	}
}