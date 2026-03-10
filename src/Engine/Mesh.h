#pragma once
#include <vector>

class RenderSystem;
struct Mesh
{

private:
	RenderSystem* m_pRenderSys;
public:
	std::vector<Vertex> m_vertices;
	std::vector<uint16_t> m_indices;
	float radius = 0.0f;
	float width = 0.0f;
	float height = 0.0f;
	float depth = 0.0f;
	float halfSize = 0.0f;

	Mesh();
	~Mesh();

	void Clear();
	void CreateTriangle(XMFLOAT3& _a, XMFLOAT3& _b, XMFLOAT3& _c, XMFLOAT4& _color);
	void CreateFace(XMFLOAT3& a, XMFLOAT3& b, XMFLOAT3& c, XMFLOAT3& d, XMFLOAT4& color);
	void CreateCube(float halfSize, XMFLOAT4 color,	std::string& _name);
	void CreateQuad(float _width, float _height, float depth, XMFLOAT4& _color, std::string& _name);
	void CreateCircle(float _radius, int _sliceCount, XMFLOAT4& _color, std::string& _name);
	void CreateCylinder(float _radius, float _height, int _sliceCount, XMFLOAT4& _color, std::string& _name);
	void CreateSphere(float radius, int sliceCount, int stackCount, XMFLOAT4& color, std::string& _name);
	void CalculNormals();
	void SetRenderSystem(RenderSystem* _renderSys);
};