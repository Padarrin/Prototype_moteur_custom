#include "pch.h"

XMFLOAT3 Collider::GetPosition()
{
	Transform* mTransform = pECS->GetComponent<Transform>(*pEntity);
	return mTransform->mPos;
}