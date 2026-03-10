#pragma once

#include "DirectXMath.h"

using namespace DirectX;
struct Vertex
{
public :
	XMFLOAT3 pos;
	XMFLOAT4 color;
	XMFLOAT3 normal;

	Vertex();

	void VertexIdentity();

};
