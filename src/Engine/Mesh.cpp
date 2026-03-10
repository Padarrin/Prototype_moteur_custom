#include "pch.h"

Mesh::Mesh()
{
	Clear();
}

Mesh::~Mesh()
{
	
}

void Mesh::Clear()
{
	m_vertices.clear();
	m_indices.clear();
}

void Mesh::CreateTriangle(XMFLOAT3& _a, XMFLOAT3& _b, XMFLOAT3& _c, XMFLOAT4& _color)
{
	m_vertices.reserve(m_vertices.size() + 3);
	Vertex a;
	a.pos = _a;
	a.color = _color;

	Vertex b;
	b.pos = _b;
	b.color = _color;

	Vertex c;
	c.pos = _c;
	c.color = _color;

	m_vertices.push_back(a);
	m_vertices.push_back(b);
	m_vertices.push_back(c);
}

void Mesh::CreateFace(XMFLOAT3& a, XMFLOAT3& b, XMFLOAT3& c, XMFLOAT3& d, XMFLOAT4& color)
{
	m_vertices.reserve(m_vertices.size() + 6);
	CreateTriangle(a, c, b, color);
	CreateTriangle(a, d, c, color);
}

//Object creation
void Mesh::CreateCube(float _halfSize, XMFLOAT4 color, std::string& _name)
{
	Clear();
	const float s = _halfSize;
	height = _halfSize;
	XMFLOAT3 p0 = { -s, -s, -s };
	XMFLOAT3 p1 = { s, -s, -s };
	XMFLOAT3 p2 = { s,  s, -s };
	XMFLOAT3 p3 = { -s,  s, -s };
	XMFLOAT3 p4 = { -s, -s,  s };
	XMFLOAT3 p5 = { s, -s,  s };
	XMFLOAT3 p6 = { s,  s,  s };
	XMFLOAT3 p7 = { -s,  s,  s };
	XMFLOAT2 tl = { 0.0f, 0.0f };	
	XMFLOAT2 tr = { 1.0f, 0.0f };	
	XMFLOAT2 br = { 1.0f, 1.0f };	
	XMFLOAT2 bl = { 0.0f, 1.0f };	
	m_vertices.reserve(m_vertices.size() + 36);
	CreateFace(p0, p1, p2, p3, color);	
	CreateFace(p4, p7, p6, p5, color);	
	CreateFace(p1, p5, p6, p2, color);	
	CreateFace(p4, p0, p3, p7, color);	
	CreateFace(p3, p2, p6, p7, color);	
	CreateFace(p4, p5, p1, p0, color); 

	for (std::uint16_t i = 0; i < 36; i++)
	{
		m_indices.push_back(i);
	}
	CalculNormals();
	m_pRenderSys->Build(m_vertices, m_indices, _name);
	
}

void Mesh::CreateQuad(float _width, float _height, float _depth, XMFLOAT4& _color, std::string& _name)
{
	Clear();
	width = _width;
	height = _height;
	depth = _depth;
	const float w = _width * 0.5f;
	const float h = _height * 0.5f;
	const float d = _depth * 0.5f;
	XMFLOAT3 p0 = { -w, -h, -d };
	XMFLOAT3 p1 = { w, -h, -d };
	XMFLOAT3 p2 = { w,  h, -d };
	XMFLOAT3 p3 = { -w,  h, -d };
	XMFLOAT3 p4 = { -w, -h,  d };
	XMFLOAT3 p5 = { w, -h,  d };
	XMFLOAT3 p6 = { w,  h,  d };
	XMFLOAT3 p7 = { -w,  h,  d };
	m_vertices.reserve(m_vertices.size() + 36);
	CreateFace(p0, p1, p2, p3, _color);                    
	CreateFace(p4, p7, p6, p5, _color);                   
	CreateFace(p1, p5, p6, p2, _color);                    
	CreateFace(p4, p0, p3, p7, _color);                    
	CreateFace(p3, p2, p6, p7, _color);                   
	CreateFace(p4, p5, p1, p0, _color);
	for (std::uint16_t i = 0; i < 36; i++)
	{
		m_indices.push_back(i);
	}
	CalculNormals();
	m_pRenderSys->Build(m_vertices, m_indices, _name);
}							   

void Mesh::CreateCircle(float _radius, int _sliceCount, XMFLOAT4& _color, std::string& _name)
{
	if (_sliceCount < 3)
		return;

	radius = _radius;

	Clear();
	m_vertices.reserve(m_vertices.size() + _sliceCount * 3);

	Vertex center;
	center.pos = { 0.0f, 0.0f, 0.0f };
	center.color = _color;
	m_vertices.push_back(center);

	float step = XM_2PI / _sliceCount;
	float angle = 0.0f;
	for (int i = 0; i <= _sliceCount; i++)
	{
		Vertex v;
		v.pos.x = cosf(angle) * _radius;  
		v.pos.y = 0.0f;
		v.pos.z = sinf(angle) * _radius;
		v.color = _color;
		m_vertices.push_back(v);

		angle += step;
	}
	for (std::uint16_t i = 1; i <= _sliceCount; ++i)
	{
		m_indices.push_back(0);
		m_indices.push_back(i + 1);
		m_indices.push_back(i);
	}
	CalculNormals();
	m_pRenderSys->Build(m_vertices, m_indices, _name);
}

void Mesh::CreateCylinder(float _radius, float _height, int _sliceCount, XMFLOAT4& _color, std::string& _name)
{
	Clear();

    float halfHeight = _height * 0.5f;
    float dTheta = XM_2PI / _sliceCount;

	radius = _radius;
	height = _height;
    
    for (int i = 0; i <= _sliceCount; ++i)
    {
        float x = cosf(i * dTheta) * _radius;
        float z = sinf(i * dTheta) * _radius;

        Vertex vBottom;
        vBottom.pos = { x, -halfHeight, z };
        vBottom.color = _color;
        m_vertices.push_back(vBottom);

        Vertex vTop;
        vTop.pos = { x, halfHeight, z };
        vTop.color = _color;
        m_vertices.push_back(vTop);
    }

    for (std::uint16_t i = 0; i < _sliceCount; ++i)
    {
        std::uint16_t bottomLeft = i * 2;
        std::uint16_t topLeft = i * 2 + 1;
        std::uint16_t bottomRight = (i + 1) * 2;
        std::uint16_t topRight = (i + 1) * 2 + 1;
        
		m_indices.push_back(bottomLeft);
		m_indices.push_back(topLeft);
		m_indices.push_back(topRight);
		m_indices.push_back(bottomLeft);
		m_indices.push_back(topRight);
		m_indices.push_back(bottomRight);
    }

    std::uint16_t baseIndexTop = (std::uint16_t)m_vertices.size();

    Vertex topCenter;
    topCenter.pos = { 0.0f, halfHeight, 0.0f };
    topCenter.color = _color;
    m_vertices.push_back(topCenter);
    for (int i = 0; i <= _sliceCount; ++i)
    {
        Vertex v;
        v.pos = { cosf(i * dTheta) * _radius, halfHeight, sinf(i * dTheta) * _radius };
        v.color = _color;
        m_vertices.push_back(v);
    }

    for (std::uint16_t i = 0; i < _sliceCount; ++i)
    {
		m_indices.push_back(baseIndexTop);
		m_indices.push_back(baseIndexTop + i + 2);
		m_indices.push_back(baseIndexTop + i + 1);
    }

	std::uint16_t baseIndexBottom = (std::uint16_t)m_vertices.size();

    Vertex bottomCenter;
    bottomCenter.pos = { 0.0f, -halfHeight, 0.0f };
    bottomCenter.color = _color;
    m_vertices.push_back(bottomCenter);

    for (int i = 0; i <= _sliceCount; ++i)
    {
        Vertex v;
        v.pos = { cosf(i * dTheta) * _radius, -halfHeight, sinf(i * dTheta) * _radius };
        v.color = _color;
        m_vertices.push_back(v);
    }
    for (std::uint16_t i = 0; i < _sliceCount; ++i)
    {
		m_indices.push_back(baseIndexBottom);
		m_indices.push_back(baseIndexBottom + i + 1);
		m_indices.push_back(baseIndexBottom + i + 2);
    }

	CalculNormals();
    m_pRenderSys->Build(m_vertices, m_indices, _name);
}

void Mesh::CreateSphere(float _radius, int _sliceCount, int _stackCount, XMFLOAT4& _color, std::string& _name)
{
	if (_stackCount < 2) _stackCount = 2;
	if (_sliceCount < 3) _sliceCount = 3;

	Clear();
	Vertex topPole;
	topPole.pos = { 0.0f, _radius, 0.0f };
	topPole.color = _color;
	m_vertices.push_back(topPole);

	radius = _radius;
	
	float phiStep = XM_2PI / _sliceCount;
	float thetaStep = XM_PI / _stackCount;
	
	for (int i = 1; i < _stackCount; ++i)
	{
		float theta = i * thetaStep;

		
		for (int j = 0; j <= _sliceCount; ++j)
		{
			float phi = j * phiStep;

			Vertex v;
			
			v.pos.x = _radius * sinf(theta) * cosf(phi);
			v.pos.y = _radius * cosf(theta);
			v.pos.z = _radius * sinf(theta) * sinf(phi);
			v.color = _color;

			m_vertices.push_back(v);
		}
	}
	Vertex bottomPole;
	bottomPole.pos = { 0.0f, -_radius, 0.0f };
	bottomPole.color = _color;
	m_vertices.push_back(bottomPole);
	for (std::uint16_t i = 1; i <= _sliceCount; ++i)
	{
		m_indices.push_back(0);
		m_indices.push_back(i + 1);
		m_indices.push_back(i);
	}
	std::uint16_t baseIndex = 1; 
	std::uint16_t ringVertexCount = _sliceCount + 1;

	for (int i = 0; i < _stackCount - 2; ++i)
	{
		for (int j = 0; j < _sliceCount; ++j)
		{
			std::uint16_t topLeft = baseIndex + i * ringVertexCount + j;
			std::uint16_t topRight = baseIndex + i * ringVertexCount + j + 1;
			std::uint16_t bottomLeft = baseIndex + (i + 1) * ringVertexCount + j;
			std::uint16_t bottomRight = baseIndex + (i + 1) * ringVertexCount + j + 1;

			
			m_indices.push_back(topLeft);
			m_indices.push_back(topRight);
			m_indices.push_back(bottomLeft);

			
			m_indices.push_back(bottomLeft);
			m_indices.push_back(topRight);
			m_indices.push_back(bottomRight);
		}
	}
	std::uint16_t southPoleIndex = (std::uint16_t)m_vertices.size() - 1;
	baseIndex = southPoleIndex - ringVertexCount; 
	for (std::uint16_t i = 0; i < _sliceCount; ++i)
	{
		m_indices.push_back(southPoleIndex);
		m_indices.push_back(baseIndex + i);
		m_indices.push_back(baseIndex + i + 1);
	}

	
	CalculNormals();
	m_pRenderSys->Build(m_vertices, m_indices, _name);
}

void Mesh::CalculNormals()
{

	for (int i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i].normal = XMFLOAT3(0.f, 0.f, 0.f);
	}

	for (int i = 0; i < m_indices.size(); i += 3)
	{
		int i0 = m_indices[i];
		int i1 = m_indices[i + 1];
		int i2 = m_indices[i + 2];

		XMVECTOR v0 = XMLoadFloat3(&m_vertices[i0].pos);
		XMVECTOR v1 = XMLoadFloat3(&m_vertices[i1].pos);
		XMVECTOR v2 = XMLoadFloat3(&m_vertices[i2].pos);

		XMVECTOR e1 = XMVectorSubtract(v1, v0);
		XMVECTOR e2 = XMVectorSubtract(v2, v0);

		XMVECTOR faceNormal = XMVector3Cross(e1, e2);

		XMVECTOR n0 = XMLoadFloat3(&m_vertices[i0].normal);
		XMVECTOR n1 = XMLoadFloat3(&m_vertices[i1].normal);
		XMVECTOR n2 = XMLoadFloat3(&m_vertices[i2].normal);

		XMStoreFloat3(&m_vertices[i0].normal, XMVectorAdd(n0, faceNormal));
		XMStoreFloat3(&m_vertices[i1].normal, XMVectorAdd(n1, faceNormal));
		XMStoreFloat3(&m_vertices[i2].normal, XMVectorAdd(n2, faceNormal));
	}

	for (int i = 0; i < m_vertices.size(); i++)
	{
		XMVECTOR n = XMLoadFloat3(&m_vertices[i].normal);
		n = XMVector3Normalize(n);
		XMStoreFloat3(&m_vertices[i].normal, n);
	}
}

void Mesh::SetRenderSystem(RenderSystem* _renderSys)
{
	m_pRenderSys = _renderSys;
}
