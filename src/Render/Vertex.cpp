#include "Vertex.h"

Vertex::Vertex()
{
	VertexIdentity();
}

void Vertex::VertexIdentity()
{
	pos = XMFLOAT3(0.0f, 0.0f, 0.0f); 
	color = { 1.0f, 1.0f, 1.0f, 1.0f };
}
