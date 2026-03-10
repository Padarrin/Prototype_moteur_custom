#pragma once

#include "DirectXMath.h"

using namespace DirectX;
struct VertexUI
{
public:
	XMFLOAT2 pos;
	XMFLOAT2 uv;
	XMFLOAT4 color;

	VertexUI();

	void VertexUIIdentity();

};

