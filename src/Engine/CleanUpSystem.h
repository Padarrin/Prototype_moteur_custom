#pragma once
#include "pch.h"
class CleanUpSystem : public System
{
private:
	std::vector<Entity*> m_entities;
public:
	~CleanUpSystem();
	void Init(ECS ecs) override;
	void Update(ECS ecs, float deltatime) override;
};
