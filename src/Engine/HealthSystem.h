#pragma once
#include "pch.h"
class HealthSystem : public System
{
private:
	std::vector<Entity*> m_entities;
public:

	~HealthSystem();
	void Init(ECS ecs) override;
	void Update(ECS ecs, float deltatime) override;
};

