#include "VertexUI.h"

VertexUI::VertexUI()
{
	VertexUIIdentity();
}

void VertexUI::VertexUIIdentity()
{
	pos = XMFLOAT2(0.0f, 0.0f);
	uv = XMFLOAT2(0.0f, 0.0f);
	color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
}
