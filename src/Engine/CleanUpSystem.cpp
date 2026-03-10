#include "CleanUpSystem.h"

CleanUpSystem::~CleanUpSystem()
{
	m_entities.clear();
}

void CleanUpSystem::Init(ECS ecs)
{
}

void CleanUpSystem::Update(ECS ecs, float deltatime)
{
	m_entities = ecs.GetEntitiesWith<Health>();

	for (int i = 0; i < m_entities.size(); i++)
	{
		Health* health = ecs.GetComponent<Health>(*m_entities[i]);
		if (health->isDead == true)
		{

			ecs.deleteEntity(m_entities[i]);
		}
	}
}
