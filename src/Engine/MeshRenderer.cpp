#include "MeshRenderer.h"

MeshRenderer::MeshRenderer()
{
}

void MeshRenderer::SetMesh(Mesh* _mesh)
{
	mpMesh = _mesh;
}

Mesh* MeshRenderer::GetMesh() const
{
	return mpMesh;
}

void MeshRenderer::SetType(GEOMETRY_TYPE _type)
{
	m_type = _type;
}

GEOMETRY_TYPE MeshRenderer::GetType()
{
	return m_type;
}

void MeshRenderer::SetMeshName(const std::string& _meshName)
{
	meshName = _meshName;
}

std::string MeshRenderer::GetMeshName() const
{
	return meshName;
}

void MeshRenderer::SetMaterial(const std::string& _materialName)
{
	materialName = _materialName;
}

std::string MeshRenderer::GetMaterial() const
{
	return materialName;
}

void MeshRenderer::SetVisible(bool _visible)
{
	isVisible = _visible;
}

bool MeshRenderer::IsVisible() const
{
	return isVisible;
}
