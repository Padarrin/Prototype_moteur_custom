#pragma once

struct Transform : public Component
{
public:
	// position
	XMFLOAT3 mPos = VEC3_ZERO;

	
	// scaling
	XMFLOAT3 mSca = VEC3_ONE;

	// rotation
	XMFLOAT3 mRot = VEC3_ZERO;
	XMFLOAT3 mForward = VEC3_FORWARD;
	XMFLOAT3 mUp = VEC3_UP;
	XMFLOAT3 mRight = VEC3_RIGHT;
	XMFLOAT4 mQuat = VEC4_QUAT_ZERO;
	

	XMFLOAT4X4 mWorldMatrix = VEC4X4_IDENTITY;

	bool isScreenSpace = false;
};