#pragma once


struct Velocity : Component
{
public:
	// gravity
	XMFLOAT3 mGravity = VEC3_GRAVITY;
	XMFLOAT3 mSpeed = VEC3_ZERO;
};

