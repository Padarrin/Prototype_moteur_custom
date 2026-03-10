#pragma once

struct TransformSystem : public System
{
private:

	std::vector<Entity*> m_entities;
	std::vector<Transform*> vTransform;
	std::vector<Velocity*> vVelocity;

public:
	TransformSystem();
	~TransformSystem();
	void Init(ECS ecs) override;
	void Update(ECS ecs, float deltatime) override;
};
