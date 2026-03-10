#pragma once

struct ColliderType
{
	enum  Type {
		Default,
		Bullet,
		Player,
		Obstacle,
		Ally
	};
};


class Collider
{
public:
	float mRadius = 0.0f;
	float mHalfHeight = 0.0f;

	int mType = ColliderType::Default;

	ECS* pECS = nullptr;
	Entity* pEntity = nullptr;

	bool mBounce = false;
	bool mIsStatic = true;
	bool mIsActive = true;

	std::vector<int> m_vNoCollideWith;

	std::function<void(Collider* us, Collider* other)> m_fCollision;

	XMFLOAT3 GetPosition();

	float GetRayon() { return mRadius; }
	bool IsStatic() { return mIsStatic; }
	bool IsBounce() { return mBounce; }
};