#pragma once


struct MeshSystem : public System
{
public:
	void Init(ECS ecs) override;
	void Update(ECS ecs, float deltatime) override;
};

