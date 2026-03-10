#pragma once
#include "Render_Shader.h"
#include <string>
#include <winstring.h>
#include "DirectXMath.h"
#include "MathHelper.h"
class Render_Material
{
public:
	std::string m_name;
	Render_Shader* m_pShader = nullptr;

	DirectX::XMFLOAT4 m_diffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::XMFLOAT3 m_fresnelR0 = { 0.01f, 0.01f, 0.01f };
	float m_roughness = 0.25f;

	DirectX::XMFLOAT4X4 MatTransform = MathHelper::Identity4x4();

	Render_Material(std::string _name, const std::wstring& _path);
	~Render_Material();
};

