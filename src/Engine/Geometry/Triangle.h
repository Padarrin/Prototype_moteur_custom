#pragma once
#include "../Render/Vertex.h"
#include <vector>
struct Triangle
{
public:
	std::vector<Vertex> v;

	Triangle();

	void TriangleIdentity();
};

