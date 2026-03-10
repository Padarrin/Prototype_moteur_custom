#pragma once

struct System
{
public:
	virtual void Init(ECS ecs) = 0;
	virtual void Update(ECS ecs, float deltatime) = 0;
};