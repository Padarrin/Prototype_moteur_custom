#pragma once
#include "pch.h"
#include "Mesh.h"

enum GEOMETRY_TYPE
{
	TYPE_NONE,
	CUBE,
	SPHERE,
	CYLINDER,
	QUAD
};

struct MeshRenderer : public Component
{
private:
	Mesh* mpMesh = nullptr;
	std::string meshName = "";
	std::string materialName = "";
	GEOMETRY_TYPE m_type = TYPE_NONE;

	bool isVisible = true;

public :
	MeshRenderer();
	~MeshRenderer() = default;

	void SetMesh(Mesh* _mesh);
	Mesh* GetMesh() const;

	void SetType(GEOMETRY_TYPE _type);
	GEOMETRY_TYPE GetType();

	void SetMeshName(const std::string& _meshName);
	std::string GetMeshName() const;

	void SetMaterial(const std::string& _materialName);
	std::string GetMaterial() const;
	
	void SetVisible(bool _visible);
	bool IsVisible() const;

};

